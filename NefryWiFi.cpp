/**
Nefry lib

Copyright (c) 2016 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

/*
ConnectSSID	: Nefryが接続するWiFiのSSIDを保存するときに使用する。5つまで保存。末端に数字が追加される String型 null文字の時リストに追加しない
ConnectPass	: Nefryが接続するWiFiのパスワードを保存するときに使用する。5つまで保存。末端に数字が追加される String型
*/

#include "NefryWiFi.h"

bool initflgWifi = false;
void Nefry_WiFi::begin() {
	NefryDisplay.clear();
	NefryDisplay.setFont(ArialMT_Plain_16);
	NefryDisplay.drawString(10, 14, "Scanning WiFi");
	NefryDisplay.drawProgressBar(14, 44, 100, 14, 0);
	NefryDisplay.display();
	WiFi.disconnect();
	delay(2000);
	WiFi.persistent(false);
	WiFi.mode(WIFI_AP_STA);
	wifiMulti = WiFiMulti();
	dataCache();
	scanWiFi(true);								//WiFiを検索し、Webページに表示する
	setWifiTimeout(0);						//無制限で自動接続する
	Serial.println("Saved WiFi List");
	Serial.println(getlistWifi());

	wifiMulti.addAP("d", "d");// ダミーデータ
	for (int i = 0; i < 5; i++) {
		if (!_nefryssid[i].equals("")) {
			wifiMulti.addAP(_nefryssid[i].c_str(), _nefrypwd[i].c_str());
		}
	}
	wifiMulti.addAP("NefryWiFi", "NefryWiFi");
	Serial.println(F("WiFi initialized"));
	initflgWifi = true;
	Serial.println(F("Waiting for WiFi to connect"));
	delay(20);
	int loopCounter = 0;
	run();
	/* Nefryが発信するWiFiの設定*/
	if (Nefry.getWriteMode() || NefryDataStore.getModulePass().length() == 0) {
		WiFi.softAP(NefryDataStore.getModuleID().c_str());
		Serial.println(F("\nWaiting for WiFi to connect"));
	}
	else {
		WiFi.softAP(NefryDataStore.getModuleID().c_str(), NefryDataStore.getModulePass().c_str());
	}
	setWifiTimeout(3);//3回目で自動接続をタイムアウトする
	setWifiTimeoutClear();
	delay(100);
}

uint8_t prevWifiStatus = WL_IDLE_STATUS;
int Nefry_WiFi::run() {
	/*
run関数で返す値

-1	:不明なエラー
0	:接続済み
1	:自動接続しない
2	:タイムアウト
3	:ネットワークアイドル中
4	:SSID検出できず
5	:検索完了
6	:接続エラー
7	:切断

*/
	if (initflgWifi == false)return 1;
	if (getWifiTimeout() == -1)return 1;
	if (getWifiTimeout() != 0 && getWifiTimeout() <= _WifiTimeOutCount)return 2;
	uint8_t wifiStatus = wifiMulti.run(Nefry.getBootMode());
	WiFi.setAutoConnect(true);
	WiFi.setAutoReconnect(true);
	if (prevWifiStatus != wifiStatus) {
		prevWifiStatus = wifiStatus;
		if (wifiStatus == WL_CONNECTED) {
			_WifiTimeOutCount = 0;
			Serial.println("WiFi connected");
			Serial.print("SSID: ");
			Serial.println(WiFi.SSID());
			Serial.print("IP address: ");
			Serial.println(WiFi.localIP());
			Nefry.printDeviceInfo();
			return 0;
		}
		prevWifiStatus = wifiStatus;
	}
	_WifiTimeOutCount++;
	//Serial.print(_WifiTimeOutCount);
	//Serial.print(F(" WiFi errorCode : "));
	switch (wifiStatus)
	{
	case WL_CONNECTED:
		_WifiTimeOutCount = 0;
		return 0;
		break;
	case WL_IDLE_STATUS:
		Serial.println(F("Network Idle"));
		return 3;
		break;
	case WL_NO_SSID_AVAIL:
		Serial.println(F("SSID Not Found"));
		WiFi.disconnect();
		return 4;
		break;
	case WL_SCAN_COMPLETED:
		Serial.println(F("Scan Complete"));
		return 5;
		break;
	case WL_CONNECT_FAILED:
		Serial.println(F("Connect Failed"));
		return 6;
		break;
	case WL_CONNECTION_LOST:
		Serial.println(F("Connection Lost"));
		return 7;
		break;
	case WL_DISCONNECTED:
		Serial.println(F("Disconnected"));
		return 7;
		break;
	default:
		break;
	}
	return -1;
}

void Nefry_WiFi::beginWeb(){
	NefryWebServer.getWebServer()->on("/wifi_conf", [&]() {
		String content = F(
			"<h1>Nefry Wifi Set</h1>"
			"<form  name=\"myForm\" method='get' action='set_wifi'><div class=\"row\"> <label for=\"ssid\">SSID: </label> <div> <input name=\"ssid\" id=\"ssid\" maxlength=\"32\"list=\"network_list\" value=\"\"> </div></div>"
			"<div class=\"row\"> <label for=\"pwd\">PASS: </label> <div> <input type=\"password\" name=\"pwd\" id=\"pwd\"maxlength=\"64\"> </div></div>"
			"<div class=\"footer\">");
		if (_WifiTimeOutCount >= _WifiTimeOutCountMax) {
			//println(_WifiTimeOutCountMax);
			//println(_WifiTimeOutCount);
			content += F("<button type=\"button\"onclick=\"location.href='/wifiCount'\">WiFiAutoConnectionCount Clear</button>");
		}
		content += F("<button type=\"button\"onclick=\"location.href='/wifiReload'\">Reload</button><input type=\"button\"value=\"Save\"onclick=\"return jsSubmit(this.form);\"></div></form><a href=\"/\">Back to top</a></div><div>");
		content += network_html;
		content += network_list;
		content += F("</div><div><h1>Saved WiFi List</h1><p>Delete WiFi Select</p><form  name=\"myForm\" method='get' action='delete_wifi'>");
		for (int i = 0; i < 5; i++) {
			if (!_nefryssid[i].equals("")) {
				content += F("<input type=\"checkbox\" value=\"1\"name=\"");
				content += i;
				content += F("\">");
				content += _nefryssid[i];
				content += F("<br>");
			}
		}
		content += F("<input type=\"button\" value=\"Delete\" onclick=\"return jsSubmit(this.form);\"></form>");
		NefryWebServer.getWebServer()->send(200, "text/html", NefryWeb.createHtml(F("Nefry Wifi Confing"), "", content));
	});

	NefryWebServer.getWebServer()->on("/wifiReload", [&]() {
		scanWiFi();
		NefryWebServer.getWebServer()->send(200, "text/html", NefryWeb.createHtml(F("Wifi Reload"),
			(String)F("<meta http-equiv=\"Refresh\" content=\"0; URL = http://") + Nefry.getAddressStr(WiFi.localIP()) + (String)F("/wifi_conf\">"),
			F("<p>Please wait...</p><a href=\"/wifi_conf\">")));
	});

	NefryWebServer.getWebServer()->on("/wifiCount", [&]() {
		setWifiTimeoutClear();
		NefryWebServer.getWebServer()->send(200, "text/html", NefryWeb.createHtml(F("Wifi Count Clear"), (String)F("<meta http-equiv=\"Refresh\" content=\"0; URL = http://") + Nefry.getAddressStr(WiFi.localIP()) + (String)F("/wifi_conf\">"), F("<p>Please wait...</p><a href=\"/wifi_conf\">")));
	});

	NefryWebServer.getWebServer()->on("/set_wifi", [&]() {
		String newssid = NefryWebServer.getWebServer()->arg("ssid");
		String newpwd = NefryWebServer.getWebServer()->arg("pwd");
		Serial.print("newssid : ");
		Serial.println(newssid);
		String content;
		if (newssid.length() > 0) {
			NefryWiFi.addWifi(newssid, newpwd);
			NefryWiFi.saveWifi();
			content = F("Save SSID:");
			content += newssid;
			content += F(" Restart to boot into new WiFi");
		}
		else {
			content = F("Empty SSID is not acceptable.");
		}
		NefryWebServer.getWebServer()->send(200, "text/html", NefryWeb.createHtml(F("Nefry Wifi Set"), "", (String)F("<h1>Nefry Wifi Set</h1><p>") + content + (String)F("</p><a href=\"/\">Back to top</a>")));
		Serial.print("dateSend");
		if (newssid.length() != 0)NefryWebServer.resetTimer(2);
	});

	NefryWebServer.getWebServer()->on("/delete_wifi", [&]() {
		String del = "", ssid;
		bool deleteFlg = false;
		for (int i = 0; i < 5; i++) {
			String data = NefryWebServer.getWebServer()->arg(i);
			if (data.equals("1")) {
				ssid = NefryWiFi.deleteWifi(i);
				if (!ssid.equals("")) {
					del += "<li>";
					del += ssid;
					del += "</li>";
					deleteFlg = true;
				}
			}
		}
		NefryWiFi.saveWifi();
		NefryWebServer.getWebServer()->send(200, "text/html", NefryWeb.createHtml(F("Nefry Wifi Delete"), "", (String)F("<h1>Nefry Wifi Delete</h1><p>Delete List</p><ul>") + del + (String)F("</ul><a href=\"/\">Back to top</a>")));
		if (deleteFlg == true)NefryWebServer.resetTimer(2);
	});
}


//WiFi

String Nefry_WiFi::deleteWifi(int id)
{
	if (id < 0 || id >= 5)return "";
	String ssid = _nefryssid[id];
	_nefryssid[id] = "";
	_nefrypwd[id] = "";
	return ssid;
}

void Nefry_WiFi::addWifi(String _ssid, String _pwd)
{
	sortWifi();
	for (int i = 0; i < 5; i++) {
		if (_nefryssid[i].equals("")) {
			_nefryssid[i] = _ssid;
			_nefrypwd[i] = _pwd;
			return;
		}
	}
	deleteWifi(0);
	sortWifi();
	_nefryssid[4] = _ssid;
	_nefrypwd[4] = _pwd;
}
int Nefry_WiFi::sortWifi()
{
	int i = 0;
	for (int j = 0; i < 4; i++) {
		if (_nefryssid[i].equals("")) {
			for (j = i + 1; j < 5; j++) {
				if (!_nefryssid[j].equals("")) {
					_nefryssid[i] = _nefryssid[j];
					_nefrypwd[i] = _nefrypwd[j];
					deleteWifi(j);
					break;
				}
			}
			if (j == 5)break;
		}
	}
	return i;
}
void Nefry_WiFi::saveWifi() {
	sortWifi();
	for (int i = 0; i < 5; i++) {
		NefryDataStore.setConnectSSID(_nefryssid[i], i);
		NefryDataStore.setConnectPass(_nefrypwd[i], i);
	}
}
String Nefry_WiFi::getlistWifi() {
	String lisWifi = "";
	for (int i = 0; i < 5; i++) {
		if (!_nefryssid[i].equals("")) {
			lisWifi += "ID : ";
			lisWifi += i;
			lisWifi += " SSID : ";
			lisWifi += _nefryssid[i];
			lisWifi += "\n";
		}
	}
	return lisWifi;
}

void Nefry_WiFi::scanWiFi(bool fastbootMode) {
	if(fastbootMode == false){
		NefryDisplay.clear();
		NefryDisplay.setFont(ArialMT_Plain_16);
		NefryDisplay.drawString(10, 14, "Scanning WiFi");
	}
	NefryDisplay.drawProgressBar(14, 44, 100, 14, 10);
	NefryDisplay.display();
	int founds = WiFi.scanNetworks();
	Serial.println();
	Serial.println(F("scan done"));
	if (founds <= 0) {
		Serial.println(F("no networks found"));
	}
	else {
		Serial.print(founds);
		Serial.println(F(" networks found"));
		network_html = F("<ol>");
		network_list = F("<datalist id=\"network_list\">");
		for (int i = 0; i < founds; ++i) {
			// Print SSID and RSSI for each network found
			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(WiFi.SSID(i));
			Serial.print(" (");
			Serial.print(WiFi.RSSI(i));
			Serial.println(")");
			network_html += F("<li>");
			network_html += WiFi.SSID(i);
			network_html += " (";
			network_html += WiFi.RSSI(i);
			network_html += ")";
			//network_html += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
			network_html += F("</li>");
			network_list += F("<option value=\"");
			network_list += WiFi.SSID(i);
			network_list += F("\"></option>");
		}
		network_html += F("</ol>");
		network_list += F("</datalist>");
	}
	if(fastbootMode == false){
		NefryDisplay.drawProgressBar(14, 44, 100, 14, 100);
	}else{
		NefryDisplay.drawProgressBar(14, 44, 100, 14, 25);
	}
	NefryDisplay.display();
}
void Nefry_WiFi::dataCache()
{
	for (int i = 0; i < 5; i++) {
		_nefryssid[i] = NefryDataStore.getConnectSSID(i);
		_nefrypwd[i] = NefryDataStore.getConnectPass(i);
	}

}
/* 文字置換 */
String Nefry_WiFi::escapeParameter(String param) {
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

/* WiFiのタイムアウトの秒数を設定します。 */

int Nefry_WiFi::getWifiTimeout()
{
	return _WifiTimeOutCountMax;
}
void Nefry_WiFi::setWifiTimeout(int count)
{
	_WifiTimeOutCountMax = count;
}
void Nefry_WiFi::setWifiTimeoutClear()
{
	_WifiTimeOutCount = 0;
}

Nefry_WiFi NefryWiFi;