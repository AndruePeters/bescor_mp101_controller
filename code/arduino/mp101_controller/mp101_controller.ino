#include <RF24.h>
#include <SPI.h>
#include <IRLibAll.h>

#include "main.h"
#include "node/node.h"
#include "packet/packet.h"

const int ARDUINO_ID = 0;
const int RF24_CHANNEL = 1;


const motor_pin_config motor_pins {6, 5, 4, 9, 2};
const led_pin_config led_pins {16, 15, 14}; // A2, A1, A0
const radio_pin_config radio_pins{7, 8}; 
state_e current_state = IDLE;
packet input_packet;
RF24 radio(radio_pins.ce, radio_pins.cs);

void setup() {
  // put your setup code here, to run once:
  config_motor_pins();
  current_state = IDLE;
}

void loop() {
  
  if (current_state == IDLE) {
    continue;
  } else if (current_state == READ) {

  } else if (current_state == PROCESS) {

  } else {

  }

}

void init_rf24()
{
  radio.begin();

}

void process_ir_packet(packet &p)
{

}

void config_motor_pins()
{
  pinMode(motor_pins.speed, OUTPUT);
  pinMode(motor_pins.right, OUTPUT);
  pinMode(motor_pins.left, OUTPUT);
  pinMode(motor_pins.up, OUTPUT);
  pinMode(motor_pin_config.down, OUTPUT);
}

/*
  Interprets a motor packet, and sets proper pins.
*/
void process_motor_pack(packet &p)
{
  // turn it off before both directional pins are set.
  set_motor_speed(0);

  // set directional values before turning speed back on
  set_motor_tilt(p.payload[1]);
  set_motor_pan(p.payload[2]);
  set_motor_speed(p.payload[0]);

  // let motor run for a short amount of time; 15 ms
  delay(15);

  // turn motor off
  set_motor_speed(0);
  set_motor_tilt(0);
  set_motor_pan(0);
}

/*
  Sets the speed of the motor.
  On the actual remote this is done using a sliding pot.
  However, this is using PWM to simulate.

  Valid values are integers [0, 255].
*/
void set_motor_speed(uint8_t speed)
{
  analogWrite(motor_pins.speed, speed);
}

/*
  Sets the motor pan up.

  Valid values: HIGH or LOW
*/
void set_motor_up(bool value)
{
  digitalWrite(motor_pins.up, value);
}

void set_motor_down(bool value)
{
  digitalWrite(motor_pins.down, value);
}

void set_motor_left(bool value)
{
  digitalWrite(motor_pins.left, value);
}

void set_motor_right(bool value)
{
  digitalWrite(motor_pins.right, value);
}


void set_motor_tilt(uint8_t value)
{
  // set tilt direction
  if (value == 0) {
    set_motor_down(LOW);
    set_motor_up(LOW);
  } else if (value == 1) {
    set_motor_down(HIGH);
    set_motor_up(LOW);
  } else if (value == 2) {
    set_motor_down(LOW);
    set_motor_up(HIGH);
  }
}

void set_motor_pan(uint8_t value)
{
  if (value == 0) {
    set_motor_left(LOW);
    set_motor_right(LOW);
  } if (value == 1) {
    set_motor_left(HIGH);
    set_motor_right(LOW);
  } else if (value == 2) {
    set_motor_left(LOW);
    set_motor_right(HIGH);
  }
}
