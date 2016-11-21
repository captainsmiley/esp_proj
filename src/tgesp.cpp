/*
 * tgesp.cpp
 *
 *  Created on: Sep 12, 2016
 *      Author: captainsmiley
 */

#include "tgesp.h"
//#include "../lib/StandardCplusplus/StandardCplusplus.h"
//include "../lib/StandardCplusplus/vector"
//#include <vector>



tgesp::tgesp() : server(80),
    client_connected(false),
    cmds()
{
    ssid = "TP-LINK_28D578";
    //ssid = "TeliaGateway9C-97-26-49-11-55";
    //password of your WPA Network
    pass = "50044801";
    //pass = "179A1021DD";
    server.begin();
}
void tgesp::listen_for_clients()
{
    WiFiClient client = server.available();
    if (client) {
        client_connected = true;
        connected_client = client;
        Serial.println("new client");
        read_client(client);
        delay(1);
        client.stop();
        Serial.println("client closed");
        client_connected = false;
    }

}

void tgesp::scanWifi() {
    // scan for nearby networks:
    Serial.println("** Scan Networks **");
    byte numSsid = WiFi.scanNetworks();

    // print the list of networks seen:
    Serial.print("SSID List:");
    Serial.println(numSsid);
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet<numSsid; thisNet++) {
        Serial.print(thisNet);
        Serial.print(") Network: ");
        Serial.println(WiFi.SSID(thisNet));
    }

}

void tgesp::connectToWifi()
{
    int connect_time = 1000;
    int time_out = 10000;
    int time = 0;
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to WAP network, SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    do {
        delay(connect_time);
        time += connect_time;
        Serial.print(".\n");
    } while ( WiFi.status() != WL_CONNECTED && time < time_out);
    if (time < time_out)
        Serial.println("Connected");
    else Serial.println("Time out");


    WiFi.printDiag(Serial);
    Serial.println(WiFi.localIP());
}

	bool tgesp::connectToWifi(const char * ssid_p, const char * pass_p)
  {
    int connect_time = 2000;
    int time_out = 16000;
    int time = 0;
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to WAP network, SSID: ");
    Serial.println(ssid_p);
    WiFi.begin(ssid_p, pass_p);
    do {
        delay(connect_time);
        time += connect_time;
        Serial.print(".\n");
    } while ( WiFi.status() != WL_CONNECTED && time < time_out);
    if (time < time_out)
    {
        Serial.println("Connected");
        return true;
      }
    else {
       Serial.println("Time out");
       return false;
    }


    WiFi.printDiag(Serial);
    Serial.println(WiFi.localIP());

  }

const char * tgesp::cmp_input(const char * input, const char * cmp)
{

    if (*cmp == '\0') {
        if (*input == '\0') return input;
        if (*input == 32) return ++input;
        if (*input == '?') return ++input;
        return nullptr;
    }
    if (*input == '\0' || *input == 32) return nullptr;
    if (*input++ == *cmp++) return  cmp_input(input,cmp);
    return nullptr;
}

void tgesp::handle_command(const char * input)
{

    output("handling command: "); output(input); output("\n");
       const char * parameter;
       if (cmp_input(input,"http_get")) {
           Serial.println("Runs http_get()"); cmds.http_get();}
       if (cmp_input(input,"read_client")) {
           Serial.println("Runs read_client()"); cmds.read_client();}
       if (cmp_input(input,"read_distance")) {
           Serial.println("Runs read_distance()"); cmds.read_distance();}
       if (cmp_input(input,"servo_h")) {
           Serial.println("Runs servo_h()"); cmds.servo_h();}
       if (cmp_input(input,"servo_l")) {
           Serial.println("Runs servo_l()"); cmds.servo_l();}
       if ((parameter = cmp_input(input,"test_gpio")))
       {
       Serial.print("Runs test_gpio("); Serial.print(parameter); Serial.print(")\n");
       Commands::test_gpio(parameter);
       }

       if ((parameter = cmp_input(input,"test_servo")))
       {
       Serial.print("Runs test_servo("); Serial.print(parameter); Serial.print(")\n");
       cmds.test_servo(parameter);
       }

       if ((parameter = cmp_input(input,"set_servo_h_pos")))
       {
           Serial.print("Runs set_servo_h_pos("); Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_h_pos(parameter);
       }
       if ((parameter = cmp_input(input,"set_servo_l_pos")))
       {
           Serial.print("Runs set_servo_l_pos("); Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_l_pos(parameter);
       }
       if ((parameter = cmp_input(input,"set_servo_delay")))
       {
           Serial.print("Runs set_servo_delay("); Serial.print(parameter); Serial.print(")\n");
           cmds.set_servo_delay(parameter);
       }
}

void tgesp::output(const char * out_put)
{
    Serial.print(out_put);
    if (client_connected)
    {
        response_content += out_put;
    }
}

void tgesp::handle_get(String & cm)
{
    handle_command(cm.substring(1).c_str());
}

void tgesp::handle_http_request(String & rq)
{
    if (rq.substring(0,3) == "GET")
    {
        String cm = rq.substring(4,rq.indexOf(' ',4));
        handle_get(cm);
    }
}


void tgesp::read_client(WiFiClient & client)
{
    boolean currentLineIsBlank = true;
    //std::vector<String> lines = {};
    String lines[10] ={};
    String line = {};
    uint8_t curr_line = 0;
    response_content = "";
    while (client.connected()) {
        if (client.available()) {
            char c = client.read();
            Serial.write(c);
            // if you've gotten to the end of the line (received a newline
            // character) and the line is blank, the http request has ended,
            // so you can send a reply
            if (c == '\n' && currentLineIsBlank) {

                response_content += "<!DOCTYPE HTML>";
                response_content += "<html>";
                response_content += "<body>";
                handle_http_request(lines[0]);
                response_content += "</body>";
                response_content += "</html>";
                send_response();

                /*
                   client.println("<!DOCTYPE HTML>");
                   client.println("<html>");
                   client.println("<body>");
                   handle_http_request(lines[0]);
                   for (int i=0; i<curr_line;++i)
                   {

                //client.println("<p>");
                //  client.println(lines[i]);
                //client.println("</p>");
                }
                client.println("</body>");
                client.println("</html>");
                */
                break;
            }
            if (c == '\n') {
                // you're starting a new line
                lines[curr_line++] = line;
                line = "";
                currentLineIsBlank = true;
            }
            else if (c != '\r') {
                // you've gotten a character on the current line
                currentLineIsBlank = false;
                line += c;
            }
        }
    }
}

void tgesp::send_response()
{
    // send a standard http response header
    connected_client.print("HTTP/1.1 200 OK\r\n");
    connected_client.print("Content-Type: text/html\r\n");
    connected_client.print("Connection: close\r\n");
    //          client.print("Cache-Control: no-cache\r\n");
    connected_client.print("Content-Length: " + String(response_content.length()) + "\r\n");
    //client.println("Refresh: 5");  // refresh the page automatically every 5 sec
    connected_client.print("\r\n");
    connected_client.print(response_content);
}

tgesp::~tgesp() {
}
