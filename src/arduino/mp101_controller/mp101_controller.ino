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

uint8_t ARDUINO_ID = 0;
color_e ARDUINO_COLOR = OFF;

const int RF24_CHANNEL = 1;

/// speed, right, left, up, down
const motor_pin_config motor_pins{6, A2, 4, A1, 2};
const led_pin_config led_pins{5, 9, 10}; // A2, A1, A0
const radio_pin_config radio_pins{7, 8};
packet input_packet;
RF24 radio(radio_pins.ce, radio_pins.cs);
IRsend ir_send; ///< pin 3

void dump_packet(packet& p) {
    for (int i = 0; i < p.payload_used; ++i) {
        Serial.write("payload[");
        Serial.write((int) i);
        Serial.write("]: ");
        int j = (int) p.payload[i];
        Serial.println(j, HEX);
    }

    Serial.println("\n\n\n");
}

void set_pipe(int id) {
    radio.openWritingPipe(ADDRESSES[0]); // write to the Pi
    radio.openReadingPipe(1, ADDRESSES[1]);
}

void button_even_handler() {
    if (ARDUINO_ID == 4) {
        ARDUINO_ID = 0;
    } else {
        ++ARDUINO_ID;
    }


    set_pipe(0);
    set_color(ARDUINO_COLOR, ARDUINO_ID);
    turn_on_leds(ARDUINO_COLOR, led_pins);
}

void setup() {
    // put your setup code here, to run once:
    config_motor_pins();
    init_rf24();
    init_pins();
    set_color(ARDUINO_COLOR, ARDUINO_ID);
    turn_on_leds(ARDUINO_COLOR, led_pins);
    Serial.begin(9600);
    Serial.write("Finished setup.\n");
}

void loop() {
    if (radio.available()) {
        radio.read(&input_packet, sizeof(input_packet));
        Serial.write("Radio is available.\n");
        if (input_packet.id == ARDUINO_ID) {
            Serial.write("Read packet for this Arduino\n");
            process_packet(input_packet);
        }
    }
}

void set_color(color_e& c, uint8_t id) {
    switch (id) {
        case 0:
            c = RED;
            break;
        case 1:
            c = GREEN;
            break;
        case 2:
            c = BLUE;
            break;
        case 3:
            c = YELLOW;
            break;
        case 4:
            c = WHITE;
            break;
        default:
            c = OFF;
    }
}

void turn_on_leds(uint8_t r, uint8_t g, uint8_t b, led_pin_config p_cfg) {
    analogWrite(p_cfg.red, r);
    analogWrite(p_cfg.grn, g);
    analogWrite(p_cfg.blu, b);
}

void turn_on_leds(color_e c, led_pin_config l) {
    switch (c) {
        case OFF:
            turn_on_leds(0, 0, 0, l);
            break;
        case BLUE:
            turn_on_leds(0, 0, 255, l);
            break;
        case GREEN:
            turn_on_leds(0, 255, 0, l);
            break;
        case CYAN:
            turn_on_leds(0, 255, 128, l);
            break;
        case RED:
            turn_on_leds(255, 0, 0, l);
            break;
        case MAGENTA:
            turn_on_leds(128, 0, 128, l);
            break;
        case YELLOW:
            turn_on_leds(128, 128, 0, l);
            break;
        case WHITE:
            turn_on_leds(128, 128, 128, l);
            break;
        default:
            turn_on_leds(0, 0, 0, l);
            break;
    }
}


void erase_packet(packet& p) {
    memset(&p, 0, sizeof(packet));
}

// packet types defined in packet.h
void process_packet(packet& p) {
    switch (p.packet_type) {
        case TELEMETRY:
            break;
        case ADMIN:
            break;
        case MOTOR:
            process_motor_packet(p);
            break;
        case IR:
            process_ir_packet(p);
            break;
        default:
            break;
    }
}

void init_rf24() {
    radio.begin();
    set_pipe(0);
    radio.setChannel(1);
    delay(20); // delay 20ms
    radio.startListening();
}

void init_pins() {
    pinMode(led_pins.red, OUTPUT);
    pinMode(led_pins.grn, OUTPUT);
    pinMode(led_pins.blu, OUTPUT);
    pinMode(17, INPUT_PULLUP); // button
}

void process_ir_packet(packet& p) {
    uint32_t reconstructed_ir_code = (static_cast<uint32_t>(p.payload[1]) << 24) |
                                     (static_cast<uint32_t>(p.payload[2]) << 16) |
                                     (static_cast<uint32_t>(p.payload[3]) << 8) |
                                     (static_cast<uint32_t>(p.payload[4]));
    uint8_t protocol = p.payload[0];
    ir_send.send(protocol, reconstructed_ir_code, p.payload[5]);
    erase_packet(p);
}

void config_motor_pins() {
    pinMode(motor_pins.speed, OUTPUT);
    pinMode(motor_pins.right, OUTPUT);
    pinMode(motor_pins.left, OUTPUT);
    pinMode(motor_pins.up, OUTPUT);
    pinMode(motor_pins.down, OUTPUT);
}

/*
  Interprets a motor packet, and sets proper pins.
*/
void process_motor_packet(packet& p) {
    // turn it off before both directional pins are set.
    set_motor_speed(0);

    // set directional values before turning speed back on
    set_motor_speed(p.payload[0]);
    set_motor_tilt(p.payload[2]);
    set_motor_pan(p.payload[1]);

}

/*
  Sets the speed of the motor.
  On the actual remote this is done using a sliding pot.
  However, this is using PWM to simulate.

  Valid values are integers [0, 255].
*/
void set_motor_speed(uint8_t speed) {
    analogWrite(motor_pins.speed, speed);
}

/*
  Sets the motor pan up.

  Valid values: HIGH or LOW
*/
void set_motor_up(bool value) {
    digitalWrite(motor_pins.up, value);
}

void set_motor_down(bool value) {
    digitalWrite(motor_pins.down, value);
}

void set_motor_left(bool value) {
    digitalWrite(motor_pins.left, value);
}

void set_motor_right(bool value) {
    digitalWrite(motor_pins.right, value);
}


void set_motor_tilt(uint8_t value) {
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

void set_motor_pan(uint8_t value) {
    if (value == 0) {
        set_motor_left(LOW);
        set_motor_right(LOW);
    }
    if (value == 1) {
        set_motor_left(HIGH);
        set_motor_right(LOW);
    } else if (value == 2) {
        set_motor_left(LOW);
        set_motor_right(HIGH);
    }
}
