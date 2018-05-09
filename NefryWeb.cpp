/**
Nefry lib

Copyright (c) 2016 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "./NefryWeb.h"
//web
String indexlink;

void Nefry_Web::setIndexLink(const char title[32], const char url[32])
{
	indexlink += "<li><a href='";
	indexlink += url;
	indexlink += "'>";
	indexlink += title;
	indexlink += "</a>";
}

String Nefry_Web::createHtml(String title, String head, String body)
{
	String content = F(
		"<!DOCTYPE HTML><html><head><meta charset=\"UTF-8\">"
		"<link rel=\"stylesheet\"type=\"text/css\"href=\"/nefry_css\">"
		"<script type=\"text/javascript\"src=\"jsform\"></script>"
		"<title>");
	content += title;
	content += F("</title>");
	content += head;
	content += F("</head><body><div>");
	content += body;
	content += F("</div><body></html>");
	/*Serial.println(title);
	Serial.println(head);
	Serial.println(body);*/
	return content;
}

void Nefry_Web::run()
{
	_dnsServer.processNextRequest();
}

void Nefry_Web::beginWeb() {

	NefryWebServer.getWebServer()->on("/", [&]() {
		String content = F(
			"<h1>Hello from Nefry!</h1>"
			"<div>Wifi Spot: "
		);
		content += WiFi.SSID();
		content += F("</div><div>IP Address: ");
		content += Nefry.getAddressStr(WiFi.localIP());
		content += F("</div><div>Module ID: ");
		content += Nefry.getModuleID();
		content += F("</div><div>");
		if (Nefry.getWriteMode())content += F("WriteMode");
		content += F(
			"</div><ul>"
			"<li><a href='/wifi_conf'>Setup WiFi</a>"
			"<li><a href='/config'>Data Store</a>"
			"<li><a href='/module'>Module Config</a>"
			//"<li><a href='/web_update'>Web Sketch Download</a>"
			"<li><a href='/update'>Upload Sketch</a>"
			"<li><a href='/console'>Web Console</a>"
		);
		content += indexlink;
		content += F("</ul><p>Nefry library:");
		content += Nefry.getVersion();
		content += F("</br>Running Program Name:");
		content += Nefry.getProgramName();
		content += F("</p>");
		NefryWebServer.getWebServer()->send(200, "text/html", createHtml(F("Nefry Menu"), "", content));
	});

	NefryWebServer.getWebServer()->on("/nefry_css", [&]() {
		NefryWebServer.getWebServer()->send(200, "text/css", F(
			"@charset \"UTF-8\"; *, :after, : before{ box - sizing:border - box }body{ font:16px / 1.65 \"Noto Sans\",\"HelveticaNeue\",\"Helvetica Neue\",Helvetica,Arial,sans-serif;margin:0;padding:0 20px;color:#555}"
			"ol,ul{padding-left:20px;margin:0}a{color:#54AFBA}a:hover{text-decoration:none}body>div{background:#fff;margin:20px auto;padding:20px 24px;box-shadow:0 0 1px 1px rgba(0,0,0,.1);border-radius:4px;max-width:540px}"
			"body>div input,body>div li{word-wrap:break-word}body>div>h1{font-size:1.4em;line-height:1.3;padding-bottom:4px;border-bottom:1px solid #efefef;margin-top:0;margin-bottom:20px}input,select,textarea{font:inherit inherit inherit}"
			"input{background:rgba(0,0,0,0);padding:.4em .6em;border:1px solid rgba(0,0,0,.12);border-radius:3px;-webkit-appearance:none;-moz-appearance:none;appearance:none}input:focus{border:1px solid #6E5F57;outline:0}"
			"input[type=submit],input[type=button],button[type=button]{margin:6px;cursor:pointer;line-height:2.6;display:inline-block;padding:0 1.2rem;text-align:center;vertical-align:middle;color:#FFF;border:0;border-radius:3px;background:#6E5F57;-webkit-appearance:none;-moz-appearance:none;appearance:none}"
			".row,.row>div,.row>label{display:block}input[type=submit]:hover{color:#FFF;background:#372F2A}input[type=submit]:focus{outline:0}input[type=file]{width:100%}.row{margin-bottom:14px}"
			".row>label{float:left;width:110px;font-size:14px;position:relative;top:8px}.row>div{margin-left:120px;margin-bottom:12px}.row>div>input{width:100%;display:inline-block}.footer{text-align:right;margin-top:16px}"
			".psrow{text-align: center;}.psrow>div{display:inline-block;margin:10px;}.writemode{color:#ff0000}input[type=\"checkbox\"] {-webkit-appearance: checkbox;appearance: checkbox;}"
		));
	});

	NefryWebServer.getWebServer()->on("/Reset", [&]() {
		NefryWebServer.getWebServer()->send(200, "text/html", createHtml(F("Nefry Reset"), "", F("<h1>Nefry Reset</h1><p>Reset start!</p><a href=\"/\">Back to top</a>")));
		NefryWebServer.resetTimer(2);	
	});

	NefryWebServer.getWebServer()->on("/jsform", [&]() {
		NefryWebServer.getWebServer()->send(200, "text/javascript", F("function jsSubmit(formN){formN.submit();}"));
	});

	NefryWebServer.getWebServer()->on("/onreset", [&]() {
		NefryDataStore.setBootSelector(1);
		NefryWebServer.getWebServer()->send(200, "text/html", createHtml(F("Nefry Write mode"), "",
			F("<h1>Nefry Write mode</h1><p>Reset start!</p><a href=\"/\">Back to top</a>")));
		NefryWebServer.resetTimer(2);
	});
}

void Nefry_Web::begin()
{
	beginWeb();
	setupCaptivePortal();
}

void Nefry_Web::setupCaptivePortal() {
	_dnsServer.start(53, "*", IPAddress(192, 168, 4, 1));
	NefryWebServer.getWebServer()->onNotFound([&]() {
		NefryWebServer.getWebServer()->send(200, "text/html", createHtml(F("CaptivePortal"), (String)F("<meta http-equiv=\"Refresh\" content=\"10; URL = http://") + Nefry.getAddressStr(WiFi.localIP()) + (String)F("\">"),
			(String)F("<h1>Move to main page!</h1><p>お探しのページは見つかりませんでした。<br>このままの画面で動作させた場合、<br>予期しない動作をする可能性があります。<br>できれば、別ブラウザで開くことを推奨します。<br>そのときブラウザに『192.168.4.1』とURL欄に入力してください。</p><p>Please wait...10sec</p><a href=\"http://") + Nefry.getAddressStr(WiFi.localIP()) + (String)F("\">Move to main page!</a>")));
	});
}
Nefry_Web NefryWeb;
