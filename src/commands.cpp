

#include "commands.h"
#include <Servo.h>

#define SERVO_H_POS_ADDR 0x00
#define SERVO_L_POS_ADDR 0x01
#define SERVO_DELAY_ADDR 0x02

Commands::Commands() :
    servo_h_pos(SERVO_H_POS),
    servo_l_pos(SERVO_L_POS)
{
  EEPROM.begin(128);
}

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
    sv.write(EEPROM.read(SERVO_H_POS_ADDR));
    uint16_t d_time = (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR);
    delay(d_time);
//    delay( (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR));
    sv.detach();
}
void Commands::set_servo_delay(const char *p)
{
  String p_s(p);
  uint16_t s_delay = uint16_t(p_s.toInt());
  EEPROM.write(SERVO_DELAY_ADDR, uint8_t( s_delay & 0x00FF) );
  EEPROM.write(SERVO_DELAY_ADDR+1, uint8_t( (s_delay & 0xFF00) >> 8 ));
  EEPROM.commit();

}
void Commands::servo_l()
{
    sv.attach(SERVO_PIN);
    sv.write(EEPROM.read(SERVO_L_POS_ADDR));
    uint16_t d_time = (EEPROM.read(SERVO_DELAY_ADDR+1) << 8) | EEPROM.read(SERVO_DELAY_ADDR);
    delay(d_time);
    sv.detach();
}

void Commands::set_servo_h_pos(const char *p)
{
    String p_s(p);
    EEPROM.write(SERVO_H_POS_ADDR,uint8_t(p_s.toInt()));
    EEPROM.commit();
}
void Commands::set_servo_l_pos(const char *p)
{
    String p_s(p);
    EEPROM.write(SERVO_L_POS_ADDR,uint8_t(p_s.toInt()));
    EEPROM.commit();
}
