#ifndef _MAIN_H_
#define _MAIN_H_
/*
  Contains pin numbers for the motor.
*/
typedef struct {
  uint8_t speed;
  uint8_t right;
  uint8_t left;
  uint8_t up;
  uint8_t down;
} motor_pins;

/*
  Contains pins for the rgb led
*/
typedef struct {
  uint8_t red;
  uint8_t grn;
  uint8_t blu;
} rgb_pins;


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
void process_packet();
void process_admin_pack();
void process_telem_pack();
void process_motor_pack();
void process_ir_pack();
#endif
