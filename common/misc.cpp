#include "misc.h"
#include <sstream>   // std::stringstream
#include <algorithm> // std::min, std::max

namespace D2K {

#if defined(_NDS)
const std::string VERSION_STRING = "DS2Key (DS) git Build";
#elif defined(_3DS)
const std::string VERSION_STRING = "DS2Key (3DS) git Build";
#elif defined(__linux__)
const std::string VERSION_STRING = "DS2Key (Linux) git Build";
#elif defined(WIN32GUI)
const std::string VERSION_STRING = "DS2Key (GUI) git Build";
#elif defined(_WIN32)
const std::string VERSION_STRING = "DS2Key (Console) git Build";
#endif

std::string ltos(long i)
{
	std::stringstream stream;
	stream << i;

	return stream.str();
}

const char* ltoa(long t)
{
	return ltos(t).c_str();
}

long stol(std::string str)
{
	std::stringstream stream(str);
	long return_value;

	return !(stream >> return_value) ? 0L : return_value;
}

uint8_t string_to_uint8_t(std::string str)
{
	return (uint8_t)D2K::clamp(D2K::stol(str), 0L, (long)UINT8_MAX);
}

uint16_t string_to_uint16_t(std::string str)
{
	return (uint16_t)D2K::clamp(D2K::stol(str), 0L, (long)UINT16_MAX);
}

} // namespace D2K
