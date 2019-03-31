#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <String.h>
#include <stdio.h>

#define DEBUG 1
#define END_CHAR '/'
#define ID 1

String SERVER = "192.168.4.1";

const char *ssid = "iServer 3.0";
const char *password = "FwNrUjp-+Pe3pjdehDcdSpy8mpQaS";
String cmd_serial = "";

ESP8266WebServer server(80);
HTTPClient http;

void setup() {

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    while(WiFi.status() != WL_CONNECTED){
        delay(500);

        Serial.print('.');
    }

    Serial.println();

    server.on("/setData", handleNewGetRequest);  // setData?deviceID=$ID&command=$cmd
    server.onNotFound ( handleNotFound );
	server.begin();

    Serial.println(WiFi.localIP().toString());
    
}

void loop() {
    server.handleClient();

    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c != END_CHAR) {
            cmd_serial += c;
        } else {
            executeCommand(cmd_serial);
            cmd_serial = "";
        }
    }
}

void handleNewGetRequest() {

    String deviceID = "-1";
    String command = "-1";

    if (server.args() > 0 && server.uri() == "/setData") {

        if (server.argName(0) == "deviceID") {
            deviceID = server.arg(0);
        }
        
        if (server.argName(1) == "command") {
            command = server.arg(1);
        }

        if (deviceID == "-1" || command == "-1") {
            handleNotFound();
            return;
        }

        String message = "NewGetRequest:deviceID=";
        message += deviceID;
        message += "&command=";
        message += command;

        Serial.println(message);

        handleRoot();
        
    } else {
        handleNotFound();
    }
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

    if (DEBUG) {
        server.send ( 404, "text/plain", message );
    } else {
        server.send(404, "text/plain", "{\"message\" : \"request_aborted\"}");
    }
}

void handleRoot() {
    server.send(200, "text/html", "{\"message\" : \"request_success\"}");
}

void executeCommand(String command) {
    char socket[80];

    int buffer_l = command.length() + 1;
    char buffer[buffer_l];

    command.toCharArray(buffer, buffer_l);

    sscanf(buffer, "deviceID=0&cmd=%s", socket);

    String url = "/setData?deviceID=0";
        url += "&command=";
        url += socket;

        sendData(SERVER, url);  
    
}

void sendData(String ip, String url) {

    ip += url;

    WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    Serial.print("[HTTP] URL :: http://");
    Serial.println(ip);

    if (http.begin(client, "http://" + ip)) {


      Serial.print("[HTTP] GET...\n");

      int httpCode = http.GET();

      if (httpCode > 0) {
        
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          Serial.println(payload);
        }
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP] Unable to connect\n");
    }
}