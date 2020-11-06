/*
  Andrue Peters
  packet.c
  4/4/19

  Implementation for packet.h
*/
#include <packet.h>
#include <assert.h>

/*
  Initialzes all value to default.
*/
void packet_init(struct packet *p)
{
  int i;

  assert (p != NULL);
  p->packet_type = TELEMETRY;
  p->payload_used = PAYLOAD_MAX_SIZE;
  p->id = 0;
  for (i = 0; i < PAYLOAD_MAX_SIZE; ++i) {
    p->payload[i] = 0;
  }
}

/*
  Sets packet.packet_type.
*/
void packet_set_type(struct packet *p, packet_type_t pt)
{
  assert (p != NULL);
  p->packet_type = pt;
}

bool operator==(const packet &p1, const packet &p2)
{
  if (p1.packet_type != p2.packet_type) return false;
  if (p1.id != p2.id) return false;
  if (p1.payload_used != p2.payload_used) return false;

  // all packets will have same length and same paylod_used at this point
  for (auto i = 0; i < p1.payload_used; ++i) {
    if (p1.payload[i] != p2.payload[i]) {
      return false;
    }
  }

  return true;
}

bool operator!=(const packet &p1, const packet &p2) 
{
  return ! (p1 == p2);
}



/*
  Sets packet.id equal to id.
*/
void packet_set_ID(struct packet *p, uint8_t id)
{
  assert (p != NULL);
  p->id = id;
}

/*
  The payload has a max size of PAYLOAD_MAX_SIZE.
  This allows you to say how much of the payload you're using.

  No need for greater than 0 check because uin8_t is only unsigned.
*/
void packet_set_payload_used(struct packet *p, uint8_t size)
{
  assert (p != NULL);
  assert (size <= PAYLOAD_MAX_SIZE);
  p->payload_used = size;
}

/*
  Copies size amount of bytes from data into p->payload;
  If size is zero, then it uses packet->payload_used;

  Aborts if size <= PAYLOAD_MAX_SIZE or if size <= p->payload_used
*/
void packet_set_payload_data(struct packet *p, const void *data, uint8_t size)
{
  uint8_t i = 0;
  uint8_t *d = (uint8_t *)data;
  assert (p != NULL);
  assert (data != NULL);
  assert (size <= PAYLOAD_MAX_SIZE);
  assert (size <= p->payload_used);

  for (i = 0; i < size; ++i ) {
    p->payload[i] = d[i];
  }
}
