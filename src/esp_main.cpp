#include <ESP8266WiFi.h>
//#include <Arduino.h>
#include "tgesp.h"

	tgesp esp;
void setup() {
	Serial.begin(115200);
	Serial.println("\nStart of esp program");
	esp.scanWifi();
	bool connected = false;
	do {
		connected = esp.connectToWifi("TeliaGateway9C-97-26-49-11-55","179A1021DD");
		if (!connected){
		connected = esp.connectToWifi("TP-LINK_28D578","50044801");
		}
	} while(!connected);

}

void loop() {
    esp.listen_for_clients();

}
