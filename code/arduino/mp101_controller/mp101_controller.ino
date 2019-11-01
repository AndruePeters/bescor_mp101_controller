#include <RF24.h>
#include <SPI.h>
#include <IRLibSendBase.h>
#include <IRLib_P01_NEC.h>
#include <IRLib_P02_Sony.h>
#include <IRLib_P03_RC5.h>
#include <IRLib_P04_RC6.h>
#include <IRLib_P05_Panasonic_Old.h>
#include <IRLib_P06_JVC.h>
#include <IRLib_P07_NECx.h>
#include <IRLib_P08_Samsung36.h>
#include <IRLib_P09_GICable.h>
#include <IRLib_P10_DirecTV.h>
#include <IRLib_P11_RCMM.h>
#include <IRLib_P12_CYKM.h>
#include <IRLibCombo.h>

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
IRsend ir_send;

void setup() 
{
  // put your setup code here, to run once:
  config_motor_pins();
  init_rf24();
  init_pins();
  current_state = IDLE;
}

void loop() 
{
  if (radio.available()) {
      radio.read(&input_packet, sizeof(input_packet));
      if (input_packet.id == ARDUINO_ID) {
        process_packet(input_packet);
      }
  }
  delay(10); ///< delay 10ms
}

void erase_packet(packet& p)
{
    memset(&p, 0, sizeof(packet));
}

// packet types defined in packet.h
void process_packet(packet &p)
{
    switch (p.packet_type) {
    case TELEMETRY: break;
    case ADMIN: break;
    case MOTOR: process_motor_packet(p); break;
    case IR:    process_ir_packet(p); break;
    default: break;
    }
}

void init_rf24()
{
  radio.begin();
  delay(20); // delay 20ms
  radio.startListening();
}

void init_pins()
{
    pinMode(led_pins.red, OUTPUT);
    pinMode(led_pins.grn, OUTPUT);
    pinMode(led_pins.blu, OUTPUT);
}

void process_ir_packet(packet &p)
{
    uint32_t reconstructed_ir_code = (p.payload[1] << 24) | (p.payload[2] << 16) |
            (p.payload[3] << 8)  | (p.payload[4]);

    uint8_t protocol = p.payload[0];
    ir_send.send(protocol, reconstructed_ir_code, 0);
    erase_packet(p);
}

void config_motor_pins()
{
  pinMode(motor_pins.speed, OUTPUT);
  pinMode(motor_pins.right, OUTPUT);
  pinMode(motor_pins.left, OUTPUT);
  pinMode(motor_pins.up, OUTPUT);
  pinMode(motor_pins.down, OUTPUT);
}

/*
  Interprets a motor packet, and sets proper pins.
*/
void process_motor_packet(packet &p)
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
  erase_packet(p);
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