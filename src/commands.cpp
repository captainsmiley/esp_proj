

#include "commands.h"
#include <Servo.h>

Commands::Commands() :
    servo_h_pos(SERVO_H_POS),
    servo_l_pos(SERVO_L_POS)
{}

Commands::~Commands(){}


void Commands::test_gpio(const char * p)
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

void Commands::read_client() {}
void Commands::read_distance() {}
void Commands::http_get() {
Serial.println("runs http_get");
}
Servo Commands::sv = Servo();
void Commands::test_servo(const char * p) 
{
    String p_s(p);
    sv.attach(SERVO_PIN);
    sv.write(p_s.toInt());
}

void Commands::servo_h()
{
    sv.attach(SERVO_PIN);
    sv.write(servo_h_pos);
    delay(SERVO_DELAY);
    sv.detach();
}
void Commands::servo_l()
{
    sv.attach(SERVO_PIN);
    sv.write(servo_l_pos);
    delay(SERVO_DELAY);
    sv.detach();
}

void Commands::set_servo_h_pos(const char *p)
{
    String p_s(p);
    servo_h_pos = uint8_t(p_s.toInt());
}
void Commands::set_servo_l_pos(const char *p)
{
    String p_s(p);
    servo_l_pos = uint8_t(p_s.toInt());
}

