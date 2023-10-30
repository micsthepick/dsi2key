// UDP Communication

#if !defined(D2KCLIENT) && !defined(D2KSERVER)
#error "Must define D2KCLIENT or D2KSERVER"
#endif

#include <cstdlib> // atoi
#include <sstream> // std::stringstream
#include <cstring> // strerror

// system specific includes
#if defined(_NDS)
#include <nds.h>
#include <sys/socket.h>
#include <dsiwifi9.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>
#elif defined(__3DS__)
#include <3ds.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#elif defined(_WIN32)
#include <ws2tcpip.h>     // socklength_t
#elif defined(__linux__)
#include <unistd.h>       // close
#include <arpa/inet.h>    // inet_ntoa
#include <sys/ioctl.h>    // ioctl
#endif

// shared (linux, nds, 3ds) defines
#if defined(__linux__) || defined(_NDS) || defined(__3DS__)
#define NETerrno errno
#define NETEADDRINUSE EADDRINUSE
#define NETEWOULDBLOCK EWOULDBLOCK
#define NETioctlsocket ioctl
#define INVALID_SOCKET -1
#endif

// system specific defines
#if defined(_WIN32)
#define NETerrno WSAGetLastError()
#define NETEADDRINUSE WSAEADDRINUSE
#define NETEWOULDBLOCK WSAEWOULDBLOCK
#define NETclosesocket closesocket
#define NETioctlsocket ioctlsocket
#elif defined(__linux__)
#define NETclosesocket close
#define SOCKET_ERROR -1
#define D2K_FLAG_BROADCAST SO_BROADCAST
#elif defined(_NDS)
#define SOCKET_ERROR -1
#define NETclosesocket close
#define D2K_FLAG_BROADCAST SOF_BROADCAST
#elif defined(__3DS__)
#define SOCKET_ERROR -1
#define NETclosesocket closesocket
#define D2K_FLAG_BROADCAST SOF_BROADCAST
#endif

#include "easylogging++Wrapper.h"

#if defined(_NDS) || defined(__3DS__)
#include "../client/source/core.h"
#include "../client//source/windows/commandWindow.h"
#include "../client//source/windows/configWindow.h"
#endif

#include "udp.h"
#include "misc.h"

namespace D2K {namespace UDP {

bool non_blocking = true;
bool connected = false;
uint16_t port = DEFAULT_PORT;
SOCKET socket_id = INVALID_SOCKET;
#ifdef D2KCLIENT
DS2KeyPacket packet = DS2KeyPacket{};
uint8_t profile = 0;
std::string remote_ip = "";
#endif
#if defined(D2KSERVER)
struct sockaddr_in local_sockaddr{};
#endif
struct sockaddr_in remote_sockaddr{};

void Init()
{
#ifdef _WIN32
	WSADATA wsa_data;
	// Startup with Winsock version 2.2
	WSAStartup(0x0202, &wsa_data);
#endif
}
void DeInit()
{
	Disconnect();
#ifdef _WIN32
	WSACleanup();
#endif
}

bool IsConnected()
{
	return connected;
}

int Connect()
{
	return Connect(UDP::non_blocking, UDP::port);
}

int Connect(uint16_t _port)
{
	return Connect(UDP::non_blocking, _port);
}

int Connect(bool _non_blocking, uint16_t _port)
{
	if(EMULATOR)          // Skip if emulating
	{
		LOG_N_TIMES(1, ERROR) << "Wifi disabled for emulator.\n";
		return 1;
	}

#if defined(__3DS__)
	static bool wifi_not_connected_log{}; // Helps us not spam the log
	if(wifi_status == WIFI_NOT_CONNECTED)
	{
		if(!wifi_not_connected_log)
		{
			LOG(ERROR) << "Error: Wifi not connected. Is it enabled?\n";
			wifi_not_connected_log = true;
		}
		return 1;
	}
	wifi_not_connected_log = false;
#endif

	if(IsConnected())     // If already connected
		Disconnect(); // Disconnect first

	SetConfigPort(_port); // Set port

	UDP::non_blocking = _non_blocking;

	remote_sockaddr.sin_family = AF_INET;
	remote_sockaddr.sin_port = htons(GetPort());
#if defined(D2KSERVER)
	int sockaddrlength = sizeof(struct sockaddr_in);
	local_sockaddr = sockaddr_in{};
	local_sockaddr.sin_family = AF_INET;
	local_sockaddr.sin_addr.s_addr = INADDR_ANY;

	local_sockaddr.sin_port = htons(GetPort());
#endif
	socket_id = socket(PF_INET, SOCK_DGRAM, 0); // create a socket
	if(socket_id == INVALID_SOCKET)
	{
		int err = NETerrno;
		LOG_EVERY_N(300, ERROR) << "Error #" << err << " (socket): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}

#ifdef D2KSERVER
	// bind a local address and port
	if(bind(socket_id, (struct sockaddr*)&local_sockaddr, sockaddrlength) == SOCKET_ERROR)
	{
		int err = NETerrno;
		if(err == NETEADDRINUSE)
			LOG(ERROR) << "Error (bind) port #" << GetPort() << " currently in use\n";
		else
			LOG(ERROR) << "Error #" << err << " (bind): " << strerror(err) << "\n";
		Disconnect();

		return err;
	}
#endif

#ifdef __3DS__
	int flags = fcntl(socket_id, F_GETFL, 0);
	if(UDP::non_blocking && fcntl(socket_id, F_SETFL, flags | O_NONBLOCK))
	{
		int err = NETerrno;
		LOG(ERROR) << "Error #" << err << " (fcntl): " << strerror(err) << "\n";
#else
	// set blocking mode
	if(NETioctlsocket(socket_id, FIONBIO, (unsigned long*)&UDP::non_blocking) == SOCKET_ERROR)
	{
		int err = NETerrno;
		LOG(ERROR) << "Error #" << err << " (NETioctlsocket): " << strerror(err) << "\n";
#endif
		Disconnect();

		return err;
	}
	connected = true;
	LOG(INFO) << "Connected on UDP port #" << GetPort() << "\n";

	return 0;
}

int Disconnect()
{
	if(EMULATOR) // Skip if emulating
		return 0;

	if(IsConnected())
	{
		// udp system disconnects even if NETclosesocket returns error
		connected = false;
		if(NETclosesocket(socket_id) == SOCKET_ERROR)
		{
			int err = NETerrno;
			LOG(ERROR) << "Error #" << err << " (NETclosesocket): " << strerror(err) << "\n";

			return err;
		}
		LOG(INFO) << "UDP Disconnected\n";
	}

	return 0;
}

int Send(const void* buffer, unsigned int length)
{
	if(!IsConnected())
	{
		LOG(ERROR) << "Error (UDP::Send): Not connected\n";
		return -1;
	}
	else if(length <= 0)
	{
		LOG(ERROR) << "Error (UDP::Send) length: #" << length << "\n";
		return -2;
	}
	else if(buffer == nullptr)
	{
		LOG(ERROR) << "Error (UDP::Send) Invalid pointer\n";
		return -3;
	}
	else // Successful
	{
		int flags = 0;
		if (!((remote_sockaddr.sin_addr.s_addr & 0xff) ^ 0xff)) {
			flags |= D2K_FLAG_BROADCAST;
		}
		int sockaddrlength = sizeof(struct sockaddr_in);
		if(sendto(socket_id, (const char*)buffer, length, flags, (struct sockaddr*)&remote_sockaddr, sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			LOG(ERROR) << "Error #" << err << " (sendto): " << strerror(err) << "\n";
			return err;
		}
		//LOG_EVERY_N(300, TRACE) << "Sent " << length << " bytes";
	}

	return 0;
}

int Recv(void* buffer, unsigned int length)
{
	return Recv(buffer, length, (struct sockaddr*)&remote_sockaddr);
}

int Recv(void* buffer, unsigned int length, struct sockaddr* _remote_sockaddr)
{
	if(!IsConnected())
	{
		LOG(ERROR) << "Error (UDP::Recv): Not connected\n";
		return -1;
	}
	else if(length <= 0)
	{
		LOG(ERROR) << "Error (UDP::Recv) length: #" << length << "\n";
		return -2;
	}
	else if(buffer == nullptr)
	{
		LOG(ERROR) << "Error (UDP::Recv) Invalid pointer\n";
		return -3;
	}
	else if(_remote_sockaddr == nullptr)
	{
		LOG(ERROR) << "Error (UDP::Recv) _remote_sockaddr is NULL\n";
		return -4;
	}
	else // Successful
	{
		socklen_t sockaddrlength = sizeof(struct sockaddr_in);
		if(recvfrom(socket_id, (char*)buffer, length, 0, _remote_sockaddr, &sockaddrlength) == SOCKET_ERROR)
		{
			int err = NETerrno;
			if(err != NETEWOULDBLOCK)
				LOG(ERROR) << "Error #" << err << " (recvfrom): " << strerror(err) << "\n";

			return err;
		}
		//LOG_EVERY_N(300, TRACE) << "Received " << length << " bytes";
	}

	return 0;
}

unsigned long GetLocalIP()
{
#if defined(_NDS)
	return DSiWifi_GetIP();
#elif defined(__3DS__)
	return gethostid();
#elif defined(_WIN32) || defined(__linux__)
	return local_sockaddr.sin_addr.s_addr;
#endif
}
std::string GetLocalIPString()
{
	struct in_addr sin_addr;
	sin_addr.s_addr = GetLocalIP();
	std::string IP = inet_ntoa(sin_addr);

	return IP;
}

unsigned long GetRemoteIP()
{
	return remote_sockaddr.sin_addr.s_addr;
}
std::string GetRemoteIPString()
{
	std::string IP = inet_ntoa(remote_sockaddr.sin_addr);
	return IP;
}

void SetRemoteIP(const std::string& text)
{
	remote_sockaddr.sin_addr.s_addr = inet_addr(text.c_str());
}
void SetRemoteIP(unsigned long ip)
{
	remote_sockaddr.sin_addr.s_addr = ip;
}

uint16_t GetPort()
{
	return UDP::port;
}
std::string GetPortString()
{
	return D2K::ltos(GetPort());
}

void SetConfigPort(const std::string& _port_string)
{
	long _port = D2K::stol(_port_string);
	if(_port > UINT16_MAX) // if _port is invalid
		_port = 0;     // set to 0, SetConfigPort will take care of it
	SetConfigPort((uint16_t)_port);
}
void SetConfigPort(const char* _port_char)
{
	int _port = atoi(_port_char);
	if(_port > UINT16_MAX) // if _port is invalid
		_port = 0;     // set to 0, SetConfigPort will take care of it
	SetConfigPort((uint16_t)_port);
}
void SetConfigPort(uint16_t _port)
{
	// If port is 0, Use default port 9501
	UDP::port = _port == 0 ? DEFAULT_PORT : _port;
	LOG(INFO) << "UDP Port set to #" << UDP::GetPort() << ".\n";
}

#if defined(D2KCLIENT)

void SendNormalSetting(DS2KeySingleInputSettingPacket setting)
{
	Send(&setting, sizeof(DS2KeySingleInputSettingPacket));
	LOG(TRACE) << "SendNormalSetting()\n";
}
void SendCommand(uint8_t command)
{
	if(command >= UDP::SETTINGS_PACKET_MAX_BUTTONS) // Valid range is 0 - 11
		return;

	packet = DS2KeyPacket{};                       // Clear the packet
	packet.type = UDP::PACKET::COMMAND;
	packet.profile = GetProfile();
	packet.keys = command;

	Send(&packet, sizeof(DS2KeyPacket));            // Send packet
	LOG(TRACE) << "SendCommand(" << (int)command << ")\n";
}

void Update(uint32_t keys, uint32_t keysTurbo, const touchPosition* touch_position,
            const circlePosition* circle_position, const circlePosition* cstick_position, 
            const accelVector* accel_status, const angularRate* gyro_status,
            const uint8_t* slider_volume, const float* slider_3d, uint16_t keyboard)
{
	if(EMULATOR)                                  // Skip if emulating
		return;

	if(!UDP::IsConnected()                        // Skip if not connected
	&& Connect())                                 // and can't connect
		return;

	ListenForServer();                            // Listen for the server

	packet = DS2KeyPacket{};                      // Clear the packet
	packet.type = UDP::PACKET::NORMAL;
	packet.profile = GetProfile();
	packet.keys = keys;
	packet.keys_turbo = keysTurbo;
	if(touch_position != nullptr)                  // Touch status is active
	{
		packet.touch_x = touch_position->px;   // Update x
		packet.touch_y = touch_position->py;   // Update y
	}
	else                                           // Touch status is inactive
	{
		packet.keys &= ~KEY_TOUCH;             // Clear touch status
	}
	if(circle_position != nullptr)                 // Circle status is active
	{
		packet.circle_x = circle_position->dx; // Update x
		packet.circle_y = circle_position->dy; // Update y
	}
	if(cstick_position != nullptr)                 // Cstick status is active
	{
		packet.cstick_x = cstick_position->dx; // Update x
		packet.cstick_y = cstick_position->dy; // Update y
	}
	if(accel_status    != nullptr)                 // Accel status is active
	{
		packet.accel_x = accel_status->x;      // Update x
		packet.accel_y = accel_status->y;      // Update y
		packet.accel_z = accel_status->z;      // Update z
	}
	if(gyro_status     != nullptr)                 // Gyro status is active
	{
		packet.gyro_x = gyro_status->x;        // Update x
		packet.gyro_y = gyro_status->y;        // Update y
		packet.gyro_z = gyro_status->z;        // Update z
	}
	if(slider_volume   != nullptr)                 // Volume slider is active
	{
		// raw value is 0-63, we convert to 0-100
		packet.slider_volume = (uint8_t)((*slider_volume * 100) / 63);
	}
	if(slider_3d       != nullptr)                 // 3D slider is active
	{
		// raw value is 0.0f-1.0f, we convert to 0-100
		packet.slider_3d = (uint8_t)(*slider_3d * 100);
	}
	packet.keyboard = keyboard;

	Send(&packet, sizeof(DS2KeyPacket));           // Send packet
	//LOG_EVERY_N(300, TRACE) << "Sent UDP::Update()";
}

void SendLookupPacket()
{
	packet = DS2KeyPacket{};             // Clear the packet
	packet.type = UDP::PACKET::LOOKUP;   // Set as a lookup packet

	Send(&packet, sizeof(DS2KeyPacket)); // Send the packet out
	//LOG_EVERY_N(10, TRACE) << "SendLookupPacket()";
}

void RequestSettingsCommand()
{
	packet = UDP::DS2KeyPacket{};                   // Clear the packet
	packet.type = UDP::PACKET::COMMAND_SETTINGS;    // Set as command settings packet
	packet.profile = UDP::GetProfile();             // Set profile

	UDP::Send(&packet, sizeof(UDP::DS2KeyPacket));  // Send the packet out
	LOG(TRACE) << "RequestSettingsCommand()\n";
}

void RequestInputSettings()
{
	packet = UDP::DS2KeyPacket{};                   // Clear the packet
	packet.type = UDP::PACKET::INPUT_SETTINGS;      // Set as input settings packet
	packet.profile = UDP::GetProfile();             // Set profile

	UDP::Send(&packet, sizeof(UDP::DS2KeyPacket));  // Send the packet out
	LOG(TRACE) << "RequestInputSettings()\n";
}

void ServerLookup()
{
	if(EMULATOR)                                        // Skip if emulating
		return;

	if(!UDP::IsConnected()                              // Skip if not connected
	&& Connect())                                       // and can't connect
		return;

	unsigned long saved_remote_ip = GetRemoteIP();      // Save the remote IP
	unsigned long LocalIP = GetLocalIP();               // Get the local IP
	SetRemoteIP(((LocalIP) & 0xFF) |                    // Setup the broadcast IP
		   (((LocalIP >> 8) & 0xFF) << 8) |
		   (((LocalIP >> 16) & 0xFF) << 16) |
		   (0xFF << 24));

	SendLookupPacket();                                 // Send the lookup packet

	// wait for 1 second
	for(int i = 0; i < 60; i++)
	{
		WaitForVBlank();
	}

	if(Recv((char*)&packet, sizeof(DS2KeyPacket)) != 0) // Didn't receive anything
	{
		SetRemoteIP(saved_remote_ip);               // Reset the remote IP
	}
	else if(packet.type == UDP::PACKET::LOOKUP)         // Received a lookup packet
	{
		if(GetLocalIP() == GetRemoteIP())           // If it's from the local IP
			SetRemoteIP(saved_remote_ip);       // Reset the remote IP
	}
}

void ListenForServer()
{
	if(EMULATOR)                                // Skip if emulating
		return;

	if(!UDP::IsConnected()                      // Skip if not connected
	&& Connect())                               // and can't connect
		return;

	DS2KeyCommandSettingsPacket command_settings_packet = DS2KeyCommandSettingsPacket{}; // Large packet
	DS2KeyInputSettingsPacket* input_settings_packet_pointer = (DS2KeyInputSettingsPacket*)&command_settings_packet;
	
	// Received something
	if(UDP::Recv(&command_settings_packet, sizeof(DS2KeyCommandSettingsPacket)) == 0)
	{
		switch(command_settings_packet.type)
		{
		case UDP::PACKET::ALIVE:            // Received a status query
			SendLookupPacket();         // Send the lookup packet
			break;
		case UDP::PACKET::COMMAND_SETTINGS: // Received Command Settings
			GUI::Command::ProcessCommandSettingsPacket(command_settings_packet);
			LOG(TRACE) << "Received UDP::PACKET::COMMAND_SETTINGS\n";
			break;
		case UDP::PACKET::INPUT_SETTINGS:  // Received Input Settings
			GUI::ConfigWindow::ProcessInputSettingsPacket(*input_settings_packet_pointer);
			LOG(TRACE) << "Received UDP::PACKET::INPUT_SETTINGS\n";
			break;
		default:
			break;
		}
	}
}

uint8_t GetProfile()
{
	return profile;
}
std::string GetProfileString()
{
	return ltos(GetProfile());
}

void SetProfile(const std::string& _profile)
{
	SetProfile(D2K::stol(_profile));
}
void SetProfile(const char* _profile)
{
	SetProfile(atoi(_profile));
}
void SetProfile(uint8_t _profile)
{
	UDP::profile = _profile;
}
#elif defined(D2KSERVER)
void SendCommandSettings(DS2KeyCommandSettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeyCommandSettingsPacket));
	LOG(TRACE) << "SendCommandSettings()\n";
}
void SendInputSettings(DS2KeyInputSettingsPacket settings)
{
	Send(&settings, sizeof(DS2KeyInputSettingsPacket));
	LOG(TRACE) << "SendInputSettings()\n";
}
#endif

}} // namespace D2K::UDP
