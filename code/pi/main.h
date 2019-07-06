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
typedef std::list<node_prop *>::iterator node_list_it;


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
 *  Set write address of the raspberry pi
 */
void set_rf24_write_addr(const address_e listening_addr);

/*
 * Handle and process the input of a joystick
 */
 void process_input(node_list_t &nl, node_list_it &it);


  /*
   * Cycle node left
   */
void cycle_node_left( node_list_t &nl, node_list_it &it);

  /*
   *  Cycle node right
   */
void cycle_node_right( node_list_t &nl, node_list_it &it);

void load_config(node_list_t &nl);
void create_ir_packet(node_list_it &it, packet &P, uint32_t ir_code);
void create_motor_packet(node_list_it &it, packet &p);
void print_packet(const packet &p);
void load_config(std::string file, node_list_t &nl);
void print_curr_node(node_list_it &it);

void init_display();
void display_status(node_list_it &it);
// These are used to convert strings from config.yaml file to the appropriate type
color_e str_to_clr(std::string color);
rf24_pa_dbm_e str_to_pwr(std::string power);
rf24_datarate_e str_to_datarate(std::string dr);
rf24_crclength_e str_to_crclen(std::string crc);
address_e str_to_addr(std::string addr);
ir_prot_e str_to_irprot(std::string ir);

#endif
