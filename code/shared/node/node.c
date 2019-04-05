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
