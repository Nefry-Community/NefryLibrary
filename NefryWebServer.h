#ifndef NefryWebServer_h
#define NefryWebServer_h

#include "./inc/WiFi/src/WiFi.h"
#include "./inc/DNSServer/src/DNSServer.h"
#include <Arduino.h>
#include "./inc/ESP32WebServer/src/ESP32WebServer.h"
#include "Nefry.h"

class Nefry_webserver
{
public:
	void
		begin(),
		run(),
		resetTimer(int delaySec);
	ESP32WebServer* getWebServer();
	String escapeParameter(String param);
private:
	bool resetFlg;
	int countdown;
};
extern Nefry_webserver NefryWebServer;
#endif
