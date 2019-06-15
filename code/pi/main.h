#ifndef _MAIN_H_
#define _MAIN_H_

#include <list>

/*
 *  node_list_t is the container to store each camera node
 *  Uses a doubly linked list:
 *            * Index and search time don't matter
 *            * Simpler to cycle in a list, although simple for an array too
 *            * Despite supporting a ton of devices, realistically looking at max of 5
 */
typedef std::list<node_prop *> node_list_t;

/*
 *  node_list is the global variable to store nodes.
 */
extern node_list_t node_list;

/*
 *  radio module for Raspberry Pi
 */
extern RF24 radio;

/*
 * Initializes the nrf24L01+ radio to default values
 */
void rf24_init();

/*
 *  Sets the nrf24L01+ to match node n's properties then sends packet p
 */
void send_packet(const nrf2401_prop &n, const packet &p);

/*
 * Configures radio to match node n's properties.
 */
void match_node_radio(const nrf2401_prop &n);

/*
 *  Set write address
 */
 void set_rf24_write_addr(const address_e listening_addr);

#endif
