/*
 * tgesp.h
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#ifndef SRC_TGESP_H_
#define SRC_TGESP_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "commands.h"

class tgesp {
public:
	tgesp();
	void scanWifi();
	void connectToWifi();
    void read_client(WiFiClient & client);
    void listen_for_clients();
    void handle_command(const char *);
    void handle_get(String &);
    void handle_http_request(String &);
    const char * cmp_input(const char * input, const char * cmp);
	virtual ~tgesp();
private:
    void output(const char *);
    void send_response();
    char * ssid;
    char * pass;
    WiFiServer server;
    bool client_connected;
    WiFiClient connected_client;
    String response_content;
    Commands cmds;
};

#endif /* SRC_TGESP_H_ */
