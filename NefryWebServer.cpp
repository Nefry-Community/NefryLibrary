/**
Nefry lib

Copyright (c) 2016 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/
#include "NefryWebServer.h"
ESP32WebServer _nefryWebServer(80);

void Nefry_webserver::begin() {
	resetFlg = false;
	_nefryWebServer.begin();
}

void Nefry_webserver::resetTimer(int delaySec)
{
	resetFlg = true;
	countdown = delaySec*100;
}

void Nefry_webserver::run() {
	bool deleteFlg;
	_nefryWebServer.handleClient();
	if (resetFlg == true) {
		countdown -= 1;
		if (countdown <= 0) {
			Nefry.reset();
		}
	}
}
ESP32WebServer* Nefry_webserver::getWebServer()
{
	return &_nefryWebServer;
}
String Nefry_webserver::escapeParameter(String param) {
	param.replace("+", " ");
	param.replace("%21", "!");
	param.replace("%22", "\"");
	param.replace("%23", "#");
	param.replace("%24", "$");
	param.replace("%25", "%");
	param.replace("%26", "&");
	param.replace("%27", "'");
	param.replace("%28", "(");
	param.replace("%29", ")");
	param.replace("%2A", "*");
	param.replace("%2B", "+");
	param.replace("%2C", ",");
	param.replace("%2F", "/");
	param.replace("%3A", ":");
	param.replace("%3B", ";");
	param.replace("%3C", "<");
	param.replace("%3D", "=");
	param.replace("%3E", ">");
	param.replace("%3F", "?");
	param.replace("%40", "@");
	param.replace("%5B", "[");
	param.replace("%5C", "\\");
	param.replace("%5D", "]");
	param.replace("%5E", "^");
	param.replace("%60", "'");
	param.replace("%7B", "{");
	param.replace("%7C", "|");
	param.replace("%7D", "}");
	return param;
}

Nefry_webserver NefryWebServer;