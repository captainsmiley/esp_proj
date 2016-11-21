#ifndef SRC_COMMANDS_H
#define SRC_COMMANDS_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>

#define SERVO_H_POS 100
#define SERVO_L_POS 10
#define SERVO_PIN 4
#define SERVO_DELAY 200

#include <Servo.h>

class Commands {
public:
    static Servo sv;
	Commands();
    static void test_gpio(const char * p);
    static void http_get();
    static void read_distance();
    static void read_client();
    static void test_servo(const char *p);
    void servo_h();
    void servo_l();
    void set_servo_h_pos(const char *p);
    void set_servo_l_pos(const char *p);
    void set_servo_delay(const char *p);
	virtual ~Commands
        ();

private:
    uint8_t servo_h_pos;
    uint8_t servo_l_pos;
};

#endif /* SRC_TGESP_H_ */
