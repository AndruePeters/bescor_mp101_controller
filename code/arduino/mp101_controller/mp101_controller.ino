#include <AceButton.h>
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>

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

using namespace ace_button;

uint8_t ARDUINO_ID = 0;
color_e ARDUINO_COLOR = OFF;

const int RF24_CHANNEL = 1;
const motor_pin_config motor_pins {6, 5, 4, 9, 2};
const led_pin_config led_pins {16, 15, 14}; // A2, A1, A0
const radio_pin_config radio_pins{7, 8}; 
state_e current_state = IDLE;
packet input_packet;
RF24 radio(radio_pins.ce, radio_pins.cs);
IRsend ir_send; ///< pin 3

AceButton button(17);


void button_even_handler()
{
    if (ARDUINO_ID == 4) {
       ARDUINO_ID = 0;
    } else {
        ++ARDUINO_ID;
    }

    set_color(ARDUINO_COLOR, ARDUINO_ID);
    turn_on_leds(ARDUINO_COLOR, led_pins);
}

void handleButtonEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  switch (eventType) {
    case AceButton::kEventReleased:
      button_even_handler();
      break;
    default: break;
  }
}

void setup() 
{
  // put your setup code here, to run once:
  config_motor_pins();
  init_rf24();
  init_pins();
  set_color(ARDUINO_COLOR, ARDUINO_ID);
  turn_on_leds(ARDUINO_COLOR, led_pins);
  current_state = IDLE;
  button.setEventHandler(handleButtonEvent);
  Serial.begin(9600);
  Serial.write ("Finished setup");
}

void loop() 
{
  if (radio.available()) {
      radio.read(&input_packet, sizeof(input_packet));
      if (input_packet.id == ARDUINO_ID) {
        process_packet(input_packet);
      }
  }
  button.check();
  delay(10); ///< delay 10ms
}

void set_color(color_e &c, uint8_t id)
{
    switch(id) {
    case 0: c = RED; break;
    case 1: c = GREEN; break;
    case 2: c = BLUE; break;
    case 3: c = YELLOW; break;
    case 4: c = WHITE; break;
    default: c = OFF;
    }
}

void turn_on_leds(uint8_t r, uint8_t g, uint8_t b, led_pin_config p_cfg)
{
    uint8_t r_ = r > 0 ? HIGH : LOW;
    uint8_t g_ = g > 0 ? HIGH : LOW;
    uint8_t b_ = b > 0 ? HIGH : LOW;

    digitalWrite(p_cfg.red, r_);
    digitalWrite(p_cfg.grn, g_);
    digitalWrite(p_cfg.blu, b_);
}

void turn_on_leds(color_e c, led_pin_config l)
{
    switch (c) {
    case OFF:       turn_on_leds(LOW, LOW, LOW, l); break;
    case BLUE:      turn_on_leds(LOW, LOW, HIGH, l); break;
    case GREEN:     turn_on_leds(LOW, HIGH, LOW, l); break;
    case CYAN:      turn_on_leds(LOW, HIGH, HIGH, l); break;
    case RED:       turn_on_leds(HIGH, LOW, LOW, l); break;
    case MAGENTA:   turn_on_leds(HIGH, LOW, HIGH, l); break;
    case YELLOW:    turn_on_leds(HIGH, HIGH, LOW, l); break;
    case WHITE:     turn_on_leds(HIGH, HIGH, HIGH, l); break;
    default:        turn_on_leds(LOW, LOW, LOW, l); break;
    }
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
    pinMode(17, INPUT_PULLUP);
}

void process_ir_packet(packet &p)
{
    uint32_t reconstructed_ir_code =    (static_cast<uint32_t>(p.payload[1]) << 24) | 
                                        (static_cast<uint32_t>(p.payload[2]) << 16) |
                                        (static_cast<uint32_t>(p.payload[3]) << 8)  | 
                                        (static_cast<uint32_t>(p.payload[4]));
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
