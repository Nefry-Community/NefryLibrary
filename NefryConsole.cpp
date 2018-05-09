/**
Nefry lib

Copyright (c) 2016 wami

This software is released under the MIT License.
http://opensource.org/licenses/mit-license.php
*/

#include "NefryConsole.h"
//webConsole

void Nefry_Console::println(float text) { println(String(text)); }
void Nefry_Console::println(double text) { println(String(text)); }
void Nefry_Console::println(char text) { println(String(text)); }
void Nefry_Console::println(int text) { println(String(text)); }
void Nefry_Console::println(long text) { println(String(text)); }
void Nefry_Console::println(unsigned char text) { println(String(text)); }
void Nefry_Console::println(unsigned int text) { println(String(text)); }
void Nefry_Console::println(unsigned long text) { println(String(text)); }
void Nefry_Console::print(float text) { print(String(text)); }
void Nefry_Console::print(double text) { print(String(text)); }
void Nefry_Console::print(char text) { print(String(text)); }
void Nefry_Console::print(int text) { print(String(text)); }
void Nefry_Console::print(long text) { print(String(text)); }
void Nefry_Console::print(unsigned char text) { print(String(text)); }
void Nefry_Console::print(unsigned int text) { print(String(text)); }
void Nefry_Console::print(unsigned long text) { print(String(text)); }
int printcun;
#define max_console 30
char printweb[max_console][50];
int mojicount = 0;
void Nefry_Console::print(String text, int ln) {
	if (printcun >= max_console)printcun = 0;
	Serial.print(text);
	if (ln == 1)text += "<br>";
	text.toCharArray(printweb[printcun++], 50);
	if (mojicount <= max_console)mojicount++;
}

void Nefry_Console::println(String text) {
	print(text, 1);
	Serial.println();
}

int Nefry_Console::available() {
	return input_console.length();
}

String Nefry_Console::read() {
	String read_console = input_console;
	input_console = "";
	return read_console;
}
void Nefry_Console::beginWeb(void) {
	NefryWebServer.getWebServer()->on("/console", [&]() {
		input_console = NefryWebServer.getWebServer()->arg("console");
		if (input_console.length() > 0) {
			println(input_console);
		}
		NefryWebServer.getWebServer()->send(200, "text/html",
			NefryWeb.createHtml(F("Nefry Console"), F("<script type=\"text/javascript\" src=\"consolejs\"></script>"),
				F("<h1>Nefry Console</h1>"
					"<p>It can be used as a terminal.</p>"
					"<form  name='msg' method='post' action='console'><div class=\"row\"> <label for=\"console\">console:</label> <div> <input name=\"console\" id='cos' maxlength=\"100\" value=\"\"></div></div>"
					"<div class=\"footer\"><input type='button' value='Send' onclick='pushDoc();' /></div></form>"
					"<div><div id=\"ajaxDiv\"></div><div class=\"row\"><button type=\"button\" onclick=\"loadDoc()\">reload</button>"
					"<button type = \"button\" onclick=\"reload(500);\">0.5sec reload</button>"
					"<button type = \"button\" onclick=\"reload(2000);\">2sec reload</button>"
					"<button type = \"button\" onclick=\"reload(5000);\">5sec reload</button>"
					"<button type = \"button\" onclick=\"clearInterval(timer);\">stop</button></div>"
					"<div class=\"row\"><button type = \"button\" onclick=\"cclear();\">Clear</button></div>"
					"</div><br><a href=\"/\">Back to top</a>")));
	});
	NefryWebServer.getWebServer()->on("/consolejs", [&]() {
		String content = F(
			"  if (window.XMLHttpRequest) {\n"
			"    xmlhttp = new XMLHttpRequest();\n"
			"  } else {\n"
			"    xmlhttp = new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
			"  }\n"
			"  function pushDoc() {\n"
			"  if (window.XMLHttpRequest) {\n"
			"    xmlhttp2 = new XMLHttpRequest();\n"
			"  } else {\n"
			"    xmlhttp2 = new ActiveXObject(\"Microsoft.XMLHTTP\");\n"
			"  }\n"
			"  xmlhttp2.open(\"POST\",\"console\",true);\n"
			"	xmlhttp2.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');\n"
			"	xmlhttp2.send('console='+document.msg.console.value);\n"
			"	document.getElementById('cos').value='';"
			"  }\n"
			"  function loadDoc() {\n"
			"  xmlhttp.onreadystatechange = function() {\n"
			"    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {\n var newtext=xmlhttp.responseText;if(newtext.match(/ENDUP/)){clearInterval(timer);}if(newtext.match(/UPDNF/)){clearInterval(timer);alert(\"プログラム更新ページが開かれたため、自動更新を停止しました。更新を終えた場合や更新しない場合Clearを押してから間隔を選択してください。\");}"
			"      document.getElementById(\"ajaxDiv\").innerHTML=newtext;\n"
			"    }\n"
			"  }\n"
			"  xmlhttp.open(\"GET\",\"cons\",true);\n"
			"  xmlhttp.send();\n"
			"}\n"
			"  function cclear() {\n"
			"  xmlhttp.open(\"GET\",\"consc\",true);\n"
			"  xmlhttp.send();\n"
			"}\n"
			"var timer; \n"
			"timer = setInterval(\"loadDoc()\",2000);\n"
			"function reload(time) {\n"
			"  clearInterval(timer); \n"
			"  timer = setInterval(\"loadDoc()\",time);\n"
			"}");
		NefryWebServer.getWebServer()->send(200, "text/javascript", content);
	});
	NefryWebServer.getWebServer()->on("/cons", HTTP_GET, [&]() {
		String content;
		int i;
		i = printcun;
		if (mojicount < max_console)i = 0;
		for (int j = 0; j < mojicount; j++, i++) {
			if (i > max_console)i = 0;
			content += printweb[i];
		}
		NefryWebServer.getWebServer()->send(200, "text/html", content);
	});
	NefryWebServer.getWebServer()->on("/consc", HTTP_GET, [&]() {
		clearConsole();
		NefryWebServer.getWebServer()->send(200, "text/html", "");
	});
}

void Nefry_Console::clearConsole() {
	mojicount = 0;
	printcun = 0;
}
Nefry_Console NefryConsole;