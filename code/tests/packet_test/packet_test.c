#include <stdio.h>

#include "packet.h"
#include "packet.c"
int main()
{
  printf("\nBeginning tests\n\n");
  struct packet p;
  uint8_t data[4] = {5, 6, 7, 8};
  packet_init(&p);
  packet_set_type(&p, ADMIN);
  packet_set_ID(&p, 4);
  packet_set_payload_used(&p, 20);
  packet_set_payload_data(&p, data, 6);
  printf("\n\nEnding tests.\n\n");
  return 0;
}
