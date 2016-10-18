#include <ESP8266WiFi.h>

// buffer for serial
char s_buff[32] = {};
char s_index = 0;

//SSID of your network 
char ssid[] = "TP-LINK_28D578";
//password of your WPA Network 
char pass[] = "50044801";

IPAddress rpi1(192,168,0,104); 

// Initialize the client library
WiFiClient client;

void scanNetworks() {
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


void connectToWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  // attempt to connect to Wifi network:
  while ( WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WAP network, SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    // wait 5 seconds for connection:
    delay(5000);
  }
  
}

void setup() {
  Serial.begin(115200);
  Serial.print('\n');
  Serial.println("Program on esp-07 starting");
  Serial.println("Scanning available networks...");
  scanNetworks();
  connectToWiFi();
  Serial.println(WiFi.localIP());
  WiFi.printDiag(Serial);

  /* get request
   if (client.connect(server, 80)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("GET /");
      client.println();
    }
    */

}

void http_get()
{
   if (client.connect(server, 8888)) {
      Serial.println("connected");
      // Make a HTTP request:
      client.println("GET /");
    //  client.println();
    }

}

void read_client()
{
    for (int i=0;i<500;++i)
    {
        if (client.available()) {
            char c = client.read();
            Serial.print(c);
        }
    }

}

void read_distance()
{
    pinMode(5,OUTPUT);
    pinMode(4,INPUT);
    digitalWrite(5,HIGH);
    delayMicroseconds(10);
    digitalWrite(5,LOW);
    unsigned long duration = pulseIn(4,HIGH);
    Serial.print("Distance: ");
    Serial.print(duration);
    Serial.print('\n');

}

void test_gpio(const char * p)
{
    String p_s(p);
    pinMode(p_s.toInt(),OUTPUT);
    if (p_s.endsWith("l")){

        digitalWrite(p_s.toInt(),LOW);
    }
    if (p_s.endsWith("h")){

        digitalWrite(p_s.toInt(),HIGH);
    }

}



const char * cmp_input(const char * input, const char * cmp)
{
    if (*cmp == '\0') {
       if (*input == '\0') return input;
       if (*input == 32) return ++input;
       return nullptr;
    }
    if (*input == '\0' || *input == 32) return nullptr;
    if (*input++ == *cmp++) return  cmp_input(input,cmp);
    return nullptr;
}

void handle_serial_input(const char * input)
{
    const char * parameter;
    if (cmp_input(input,"http_get")) { Serial.println("Runs http_get()"); http_get();}
    if (cmp_input(input,"read_client")) { Serial.println("Runs read_client()"); read_client();}
    if (cmp_input(input,"read_distance")) { Serial.println("Runs read_distance()"); read_distance();}
    if (parameter = cmp_input(input,"test_gpio")) 
    {
        Serial.print("Runs test_gpio("); Serial.print(parameter); Serial.print(")\n"); 
        test_gpio(parameter);
    }
}





void loop() {
    /* print result
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  */
 // send data only when you receive data:
        if (Serial.available() > 0) {
            char c = Serial.read();
            //Serial.println(c);
            
            if(c== '\n' || s_index > 32-1)
            {
                s_buff[s_index] = '\0';
                Serial.print("I received: ");
                Serial.println(s_buff);
                /*
                for(int i=0; i<s_index; ++i)
                {
                    Serial.println(s_buff[i]);
                }
                */
                s_index = 0;
                handle_serial_input(s_buff);
            } else
            {
                s_buff[s_index++] = c;
                s_buff[s_index] = '\0';
            }
        }
  
  // put your main code here, to run repeatedly:

}
