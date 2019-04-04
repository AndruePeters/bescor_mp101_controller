/*
  node.h
  Andrue Peters
  4/4/19

  The Pi and each Arduino will have this structure containing properties about it.
  The Pi will be able to tell the Arduino to change certain settings like the power and IR code,
  but not the other way around. (Technically it's possible, but why?)

*/

#ifndef _DP_NODE_H_
#define _DP_NODE_H_

const uint8_t ADDRESSES[][6] = { "PNode", "1Node", "2Node", "3Node", "4Node", "5Node"};
typedef enum { Pi, A1, A2, A3, A4, A5} address_t;

/*
  Contains data for the nrf2401+ module.
  The top four elements are self-explanatory.

  The last element is the index for the 64 bit address stored in ROM on each device.
*/
struct nrf2401_prop {
  rf24_pa_dbm_e power_level;
  rff24_datarate_e data_rate;
  rf24_crclength_e crc_length;
  uint8_t channel;
  address_t address;
};

/*
  Stores information for the infrared functionality.
*/
struct ir_props {
  uint32_t zoom_in;
  uint32_t zoom_out;
  uint32_t focus_in;
  uint32_t focus_out;
  uint8_t ir_prot;
};

/*
  Information for each node.
*/
struct node_prop {
  uint8_t color;
  uint8_t id;
  struct nrf2401_prop rf;
  struct ir_prop ir;
}


#endif
