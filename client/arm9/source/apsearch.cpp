#include <nds.h>
#include <stdio.h>
#include <dsiwifi9.h>
#include "apsearch.h"

namespace D2K {
	namespace AP {
		int count = -1;
		bool searching = false;
		int Update() {
			if(searching) {
				count = DSiWifi_GetNumAP();
			}
			else {
				count = -1;
			}
			return count;
		}
		void Init() {
			DSiWifi_ScanMode();
			searching = true;
			Update();
		}
		Wifi_AccessPoint* getAP(int i) {
			Update();
			if(i <= count)
			{
				DSiWifi_GetAPData(i, &ap);
				return &ap;
			}
			return (Wifi_AccessPoint*)NULL;
		}
		void Print(int i) {
			getAP(i);
			// display the name of the AP
			iprintf("%.29s\n  Wep:%s Sig:%i\n",
				ap.ssid,
				ap.flags & WFLAG_APDATA_WEP ? "Yes " : "No ",
				ap.rssi * 100 / 0xD0);
		}
	}
}
