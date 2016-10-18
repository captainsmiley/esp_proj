#include <ESP8266WiFi.h>
//#include <Arduino.h>
#include "tgesp.h"

	tgesp esp;
void setup() {
	Serial.begin(115200);
	Serial.println("\nStart of esp program");
	esp.scanWifi();
	esp.connectToWifi();





}

void loop() {
    esp.listen_for_clients();

}
