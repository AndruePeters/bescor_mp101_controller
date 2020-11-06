/*
  packet.h
  Andrue Peters
  Rewritten on 4/4/19

  Basic structure for a packet for the nrf2401+.
  Refactored from C++ to c. This is mostly because I'm still learning
  to use OOP concepts responsibly and not make everything a class.
  Part of this is also using C extensively the past year and falling in love all over again.

*/

#ifndef _DP_NRF2401_PACKET_H_
#define _DP_NRF2401_PACKET_H_

#include <stdint.h>
#include <stddef.h>

#define PAYLOAD_MAX_SIZE (28)

typedef enum {
    TELEMETRY,
    ADMIN,
    MOTOR,
    IR,
    NUM_PACKET_TYPES
} PACKET_TYPES;

typedef uint8_t packet_type_t;
/*
  32 bytes max for nrf2401+
*/
struct packet {
    packet_type_t packet_type;
    uint8_t payload_used;
    uint8_t id;
    uint8_t payload[PAYLOAD_MAX_SIZE];
};

/* Function prototypes */
/*
  Initialiazes all values to 0 in packet.
*/
void packet_init(struct packet* p);

/*
  Returns true if packets are equal. False otherwise.
*/
bool operator==(const packet& p1, const packet& p2);


bool operator!=(const packet& p1, const packet& p2);

/*
  Sets packet.packet_type.
*/
void packet_set_type(struct packet* p, packet_type_t pt);

/*
  Sets the node an ID number to be used.
*/
void packet_set_ID(struct packet* p, uint8_t id);


/*
  The payload has a max size of PAYLOAD_MAX_SIZE.
  This allows you to say how much of the payload you're using.
  This must be greater than or equal to 0, but less than PAYLOAD_MAX_SIZE.

  Setting to 5 would cause you to use 5 bytes out of the 28 allotted.
*/
void packet_set_payload_used(struct packet* p, uint8_t size);

/*
  Copies size amount of bytes from data into p->payload;
  If size is zero, then it uses packet->payload_used;

  Aborts if size <= PAYLOAD_MAX_SIZE or if size <= p->payload_used
*/
void packet_set_payload_data(struct packet* p, const void* data, uint8_t size);

#endif
