/*
  node.c
  Andrue Peters
  4/5/19

  Implementation for node.h
*/

#include <node.h>
#include <assert.h>

/*
  Initialzes node.

  color = BLUE
  id = 0

  default rf properties set for maximum range
  rf.power_level = RF24_PA_MAX
  rf.data_rate = RF24_250KBPS
  rf.crc_length = RF24_CRC_DISABLED
  rf.channel = 0
  rf.address = A1

  zoom_in = 0
  zoom_out = 0
  focus_in = 0
  focus_out = 0
  ir_prot = 0
*/
void node_init(struct node_prop *np)
{
  assert(np != NULL);
  node_set_color(np, BLUE);
  node_set_id(np, 0);

  node_set_power_level(np, RF24_PA_MAX);
  node_set_data_rate(np, RF24_250KBPS);
  node_set_crc_length(np, RF24_CRC_DISABLED);
  node_set_channel(np, 0);
  node_set_address(np, A1);

  node_set_zoom_in(np, 0);
  node_set_zoom_out(np, 0);
  node_set_focus_in(np, 0);
  node_set_focus_out(np, 0);
  node_set_ir_prot(np, 0);
}


/*
  Sets the rgb color of the node.
  If it's not a valid value, then it is set to be OFF.
*/
void node_set_color(struct node_prop *np, color_e c)
{
  assert(np != NULL);
  if (c < 0 || c >= NUM_COLORS) {
    np->color = OFF;
  } else {
    np->color = c;
  }
}


/*
  Returns the color of the node.
*/
color_e node_get_color(struct node_prop *np)
{
  assert(np != NULL);
  return np->color;
}

/*
  Sets the id of the node.
  Can be any uint8_t value.
*/
void node_set_id(struct node_prop *np, uint8_t id)
{
  assert (np != NULL);
  np->id = id;
}

/*
  Returns the id of the node.
*/
uint8_t node_get_id(struct node_prop *np)
{
  assert (np != NULL);
  return np->id;
}

/*
  Sets power level of node.
  *TODO Look up implementation to see if it automatically sets invalid values.

  typedef enum { RF24_PA_MIN = 0,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX, RF24_PA_ERROR } rf24_pa_dbm_e ;
*/
void node_set_power_level(struct node_prop *np, rf24_pa_dbm_e pl)
{
  assert (np != NULL);
  if (pl < RF24_PA_MIN || pl > RF24_PA_MAX) {
    // what to do?
    // decide in the future.
  } else {
    np->rf.power_level  = pl;
  }
}

/*
  Returns the power level.
*/
rf24_pa_dbm_e node_get_power_level(struct node_prop *np)
{
  assert (np != NULL);
  return np->rf.power_level;
}

/*
  Sets the data rate the node transmits at.
  typedef enum { RF24_1MBPS = 0, RF24_2MBPS, RF24_250KBPS } rf24_datarate_e;
*/
void node_set_data_rate(struct node_prop *np, rf24_datarate_e dr)
{
  assert (np != NULL);
  if (dr < RF24_1MBPS || dr > RF24_250KBPS) {
    // what to do?
    // decide in future
  } else {
    np->rf.data_rate = dr;
  }
}

/*
  Returns the data rate.
*/
rf24_datarate_e node_get_data_rate(struct node_prop *np)
{
  assert (np != NULL);
  return np->rf.data_rate;
}


/*
  Sets the crc length of the radio.
  typedef enum { RF24_CRC_DISABLED = 0, RF24_CRC_8, RF24_CRC_16 } rf24_crclength_e;
*/
void node_set_crc_length(struct node_prop *np, rf24_crclength_e cl)
{
  assert (np != NULL);
  if (cl < RF24_CRC_DISABLED || cl > RF24_CRC_16) {
    // what to do?
    // decide in future
  } else {
    np->rf.crc_length = cl;
  }
}

/*
  Returns the crc length.
*/
rf24_crclength_e node_get_crc_length(struct node_prop *np)
{
  assert (np != NULL);
  return np->rf.crc_length;
}

/*
  Sets the channel the node operates on.
*/
void node_set_channel(struct node_prop *np, uint8_t c)
{
  assert (np != NULL);
  np->rf.channel = c;
}

/*
  Returns the channel the node is operating on.
*/
uint8_t node_get_channel(struct node_prop *np)
{
  assert (np != NULL);
  return np->rf.channel;
}


/*
  Sets the address of the node.
*/
void node_set_address(struct node_prop *np, address_e a)
{
  assert (np != NULL);
  if ( a < PI || a > A5) {
    // what to do?
    // decide in future
  } else {
    np->rf.listening_addr = a;
  }
}

/*
  Returns the address of the node.
*/
address_e node_get_address(struct node_prop *np)
{
  assert (np != NULL);
  return np->rf.listening_addr;
}

/*
  Sets the zoom in code.
*/
void node_set_zoom_in(struct node_prop *np, uint32_t zi)
{
  assert (np != NULL);
  np->ir.zoom_in = zi;
}

/*
  Returns the zoom in code.
*/
uint32_t node_get_zoom_in(struct node_prop *np)
{
  assert (np != NULL);
  return np->ir.zoom_in;
}

/*
  Sets the zoom out code.
*/
void node_set_zoom_out(struct node_prop *np, uint32_t zo)
{
  assert (np != NULL);
  np->ir.zoom_out = zo;
}

/*
  Returns the zoom out code.
*/
uint32_t node_get_zoom_out(struct node_prop *np)
{
  assert (np != NULL);
  return np->ir.zoom_out;
}

/*
  Sets the focus in code.
*/
void node_set_focus_in(struct node_prop *np, uint32_t fi)
{
  assert (np != NULL);
  np->ir.focus_in = fi;
}

/*
  Returns the focus in code.
*/
uint32_t node_get_focus_in(struct node_prop *np)
{
  assert (np != NULL);
  return np->ir.focus_in;
}

/*
  Sets the focus_out code.
*/
void node_set_focus_out(struct node_prop *np, uint32_t fo)
{
  assert (np != NULL);
  np->ir.focus_out = fo;
}

/*
  Returns the focus out code.
*/
uint32_t node_get_focus_out(struct node_prop *np)
{
  assert (np != NULL);
  return np->ir.focus_out;
}

/*
  Sets the ir protocol.
  Need to determine supported protocols in future and check.
*/
void node_set_ir_prot(struct node_prop *np, uint32_t ip)
{
  assert (np != NULL);
  np->ir.ir_prot = ip;
}

/*
  Returns the ir protocol.
*/
uint8_t node_get_ir_prot(struct node_prop *np)
{
  assert (np != NULL);
  return np->ir.ir_prot;
}
