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
  rf.crc_length = RF24_CRC_NONE
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
color_t node_get_color(struct node_prop *np)
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
