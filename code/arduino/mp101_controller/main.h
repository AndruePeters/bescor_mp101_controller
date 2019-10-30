#ifndef _MAIN_H_
#define _MAIN_H_
#include "node/node.h"
#include "packet/packet.h"
/*
  Contains pin numbers for the motor.
*/
typedef struct {
  uint8_t speed;
  uint8_t right;
  uint8_t left;
  uint8_t up;
  uint8_t down;
} motor_pin_config;

/*
  Contains pins for the rgb led
*/
typedef struct {
  uint8_t red;
  uint8_t grn;
  uint8_t blu;
  uint8_t ir;
} led_pin_config;


typedef struct {
  uint8_t ce;
  uint8_t cs;
} radio_pin_config;


/*
  State machine for Arduino
*/
typedef enum {
  INIT = 0x00,
  IDLE,
  READ,
  PROCESS
} state_e;

void init();
void init_pins();
void init_rf24();
void read_packet();
void process_packet(packet &p);
void process_admin_packet(packet &p);
void process_telem_packet(packet &p);
void process_motor_packet(packet &p);
void process_ir_packet(packet &p);

void config_motor_pins();
void set_motor_speed(uint8_t speed);
void set_motor_up(bool value);
void set_motor_down(bool value);
void set_motor_left(bool value);
void set_motor_right(bool value);
void set_motor_tilt(uint8_t value);
void set_motor_pan(uint8_t value);

void erase_packet(packet& p);

#endif
