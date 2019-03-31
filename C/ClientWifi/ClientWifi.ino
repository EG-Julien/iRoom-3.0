#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <String.h>
#include <stdio.h>

#define DEBUG 1
#define END_CHAR '/'
#define ID 1

#define SERVER "192.168.4.1"

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

        http.begin(SERVER, 80, url);   
    
}