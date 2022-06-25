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

const int RF24_CHANNEL = 1;

/// speed, right, left, up, down
const motor_pin_config motor_pins{6, A2, 4, A1, 2};
const radio_pin_config radio_pins{9, 10};
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

void setup() {
    // put your setup code here, to run once:
    config_motor_pins();
    init_rf24();
    Serial.begin(9600);
    Serial.write("Finished setup.\n");
}

void loop() {
    if (radio.available()) {
        radio.read(&input_packet, sizeof(input_packet));
        dump_packet(input_packet);
        //Serial.write("Radio is available.\n");
        if (input_packet.id == ARDUINO_ID) {
            Serial.write("Read packet for this Arduino\n");
            process_packet(input_packet);
        }
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
    set_motor_tilt(p.payload[3], p.payload[4]);
    set_motor_pan(p.payload[1], p.payload[2]);
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

void set_motor_up(uint8_t value) {
    analogWrite(motor_pins.up, value);
}

void set_motor_down(uint8_t value) {
    analogWrite(motor_pins.down, value);
}

void set_motor_left(uint8_t value) {
    analogWrite(motor_pins.left, value);
}

void set_motor_right(uint8_t value) {
    analogWrite(motor_pins.right, value);
}


void set_motor_tilt(uint8_t dir, uint8_t value) {
    // set tilt direction
    if (dir == 0) {
        set_motor_down(LOW);
        set_motor_up(LOW);
    } else if (dir == 1) {
        set_motor_down(value);
        set_motor_up(LOW);
    } else if (dir == 2) {
        set_motor_down(LOW);
        set_motor_up(value);
    }
}

void set_motor_pan(uint8_t dir, uint8_t value) {
    if (dir == 0) {
        set_motor_left(LOW);
        set_motor_right(LOW);
    }
    if (dir == 1) {
        set_motor_left(value);
        set_motor_right(LOW);
    } else if (dir == 2) {
        set_motor_left(LOW);
        set_motor_right(value);
    }
}
