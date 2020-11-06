/*
  node.h
  Andrue Peters
  4/4/19

  The Pi and each Arduino will have this structure containing properties about it.
  The Pi will be able to tell the Arduino to change certain settings like the power and IR code,
  but not the other way around. (Technically it's possible, but why?)

  Note about the address: The actual addressing is stored in the ADDRESSES array.
  address_e stores the index of each actual address. Using this method (variable to store index)
  makes changing and storing the address much simpler and easier to transmit for an embedded system.

  ToDo: Figure out a better way to handle null pointers. Right now, I just assert.
        Figure out how I want to set radio changes.

*/

#ifndef _DP_NODE_H_
#define _DP_NODE_H_

#ifndef ARDUINO

#include <RF24/RF24.h>

#else
#include <RF24.h>
#endif


const uint8_t ADDRESSES[][6] = {"PNode", "1Node", "2Node", "3Node", "4Node", "5Node"};
typedef enum {
    RPI_ADDR = 0x00, A1_ADDR, A2_ADDR, A3_ADDR, A4_ADDR, A5_ADDR
} address_e;
typedef enum {
    OFF = 0x00, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE, NUM_COLORS
} color_e;

/*
  These are defined in IRLIB2 for the Arduino platform, and are
  used here for conistency.
*/
typedef enum {
    IR_UNKNOWN = 0x00, IR_NEC, IR_SONY, IR_RC5, IR_RC6, IR_PANASONIC_OLD, IR_JVC,
    IR_NECX, IR_SAMSUNG36, IR_GICABLE, IR_DIRECTV, IR_RCMM, IR_CYKM
} ir_prot_e;


/*
  Contains data for the nrf2401+ module.
  The top four elements are self-explanatory.

  The last element is the index for the 64 bit address stored in ROM on each device.
*/
struct nrf2401_prop {
    rf24_pa_dbm_e power_level;
    rf24_datarate_e data_rate;
    rf24_crclength_e crc_length;
    uint8_t channel;
    address_e listening_addr;
};

/*
  Stores information for the infrared functionality.
*/
struct ir_prop {
    uint32_t zoom_in;
    uint32_t zoom_out;
    uint32_t focus_in;
    uint32_t focus_out;
    uint32_t ir_bit_width;
    uint8_t ir_prot;
};

/*
  Properties for each node.
*/
struct node_prop {
    color_e color;
    uint8_t id;
    struct nrf2401_prop rf;
    struct ir_prop ir;
    char description[50];
};

/*
  Initialzes node.

  color = BLUE
  id = 0

  default rf properties set for maximum range
  rf.power_level = RF24_PA_MAX
  rf.data_rate = RF24_250KBPS
  rf.crc_length = RF24_CRC_NONE
  rf.channel = 0
  rf.address = A1

  zoom_in = 0
  zoom_out = 0
  focus_in = 0
  focus_out = 0
  ir_prot = 0
*/
void node_init(struct node_prop* np);

/*
  Sets the rgb color of the node.
  If it's not a valid value, then it is set to be OFF.
*/
void node_set_color(struct node_prop* np, color_e c);

/*
  Returns the color of the node.
*/
color_e node_get_color(struct node_prop* np);

/*
  Sets the id of the node.
  Can be any uint8_t value.
*/
void node_set_id(struct node_prop* np, uint8_t id);

/*
  Returns the id of the node.
*/
uint8_t node_get_id(struct node_prop* np);

/******************************************************************************/
/*         The following are for the radio properties of the node.            */
/******************************************************************************/


/*
  Sets power level of node.
  *TODO Look up implementation to see if it automatically sets invalid values.

  typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;
*/
void node_set_power_level(struct node_prop* np, rf24_pa_dbm_e pl);

/*
  Returns the power level.
*/
rf24_pa_dbm_e node_get_power_level(struct node_prop* np);

/*
  Sets the data rate the node transmits at.
  typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;
*/
void node_set_data_rate(struct node_prop* np, rf24_datarate_e dr);

/*
  Returns the data rate.
*/
rf24_datarate_e node_get_data_rate(struct node_prop* np);

/*
  Sets the crc length of the radio.
  typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;
*/
void node_set_crc_length(struct node_prop* np, rf24_crclength_e cl);

/*
  Returns the crc length.
*/
rf24_crclength_e node_get_crc_length(struct node_prop* cp);

/*
  Sets the channel the node operates on.
*/
void node_set_channel(struct node_prop* np, uint8_t c);

/*
  Returns the channel the node is operating on.
*/
uint8_t node_get_channel(struct node_prop* np);

/*
  Sets the address of the node.
*/
void node_set_address(struct node_prop* np, address_e a);

/*
  Returns the address of the node.
*/
address_e node_get_address(struct node_prop* np);


/******************************************************************************/
/*         The following are for the radio properties of the node.            */
/******************************************************************************/

/*
  Sets the zoom in code.
*/
void node_set_zoom_in(struct node_prop* np, uint32_t zi);

/*
  Returns the zoom in code.
*/
uint32_t node_get_zoom_in(struct node_prop* np);

/*
  Sets the zoom out code.
*/
void node_set_zoom_out(struct node_prop* np, uint32_t zo);

/*
  Returns the zoom out code.
*/
uint32_t node_get_zoom_out(struct node_prop* np);

/*
  Sets the focus in code.
*/
void node_set_focus_in(struct node_prop* np, uint32_t fi);

/*
  Returns the focus in code.
*/
uint32_t node_get_focus_in(struct node_prop* np);

/*
  Sets the focus_out code.
*/
void node_set_focus_out(struct node_prop* np, uint32_t fo);

/*
  Returns the focus out code.
*/
uint32_t node_get_focus_out(struct node_prop* np);

/*
  Sets the ir protocol.
*/
void node_set_ir_prot(struct node_prop* np, uint32_t ip);

/*
  Returns the ir protocol.
*/
uint8_t node_get_ir_prot(struct node_prop* np);


/*
    Sets the bit width for the ir protocol.
*/
void node_set_ir_bit_width(struct node_prop* np, uint32_t bw);

/*
    Returns the bit width for the ir protocol.
*/
uint32_t node_get_ir_bit_width(const struct node_prop* np);

#endif
