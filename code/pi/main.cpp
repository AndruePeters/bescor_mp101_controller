/*
  main.cpp
  5/1/19
  Andrue Peters

  This is the main file for the program.
  Better description later.
*/
#include <iostream>
#include <sstream>
#include <iterator>

#include <RF24/RF24.h>
#include <wiringPi.h>

#include <curses.h>
// specific files projectc++c
#include <node.h>
#include <packet.h>
#include <joystick/controller_map.h>
#include <joystick/js.h>

#include <unistd.h> // for sleep

#include <main.h>

using namespace controller;
// global radio instance
// using wiringPi, so wiringPi pin scheme is also used
// this is equivalent to radio(22, 0) in the generic gettingstarted.cpp
RF24 radio(3, 10);
JS_State js(0);
node_list_t node_list;

int main()
{
  wiringPiSetup();
  rf24_init();




  while (0) {
    sleep(1);
  }

  return 0;
}


void rf24_init()
{
  radio.begin();
  radio.setAutoAck(true);
  radio.setRetries(2, 15);
  radio.openWritingPipe(ADDRESSES[2]);
  radio.openReadingPipe(1, ADDRESSES[0]);
}

/*
 * Configures radio to match node n's properties needed for sending/receiving messages.
 */
void match_node_radio(const nrf2401_prop &n)
{
  radio.setDataRate(n.data_rate);
  radio.setCRCLength(n.crc_length);
  radio.setChannel(n.channel);
  set_rf24_write_addr(n.listening_addr);
}


/*
 *
 */
void send_packet(const nrf2401_prop &n, const packet &p)
{
  match_node_radio(n);

  // First, stop listening so we can talk.
  radio.stopListening();
  // attempt to send packet
  if (!radio.write( &p, sizeof(p))) {

  }
}


/*
 *  Sets the raspberry pi write address to the arduino node listening address
 */
 void set_rf24_write_addr(const address_e listening_addr)
 {
   radio.openWritingPipe(ADDRESSES[listening_addr]);
   radio.openReadingPipe(1, ADDRESSES[0]);
 }

 /*
  * Handle and process the input of a joystick
  */
void process_input(node_list_t &nl, node_list_it &it)
 {
   if (!js.isConnected()) return;
   packet p;

   // get current joystick state
   js.update();

   // first check and cycle node
   if (js.isBtnPressed(DS4::L1)) {
     cycle_node_left(nl, it);
   } else if (js.isBtnPressed(DS4::R1)) {
     cycle_node_right(nl, it);
   }

   // if magnitude of left stick is greater than 0, then form motor packet
   if (js.getAxisMagnitude(DS4::LS_X, DS4::LS_Y)) {
     create_motor_packet(it, p);
     send_packet((*it)->rf, p);
   }

   // now time to process IR stuff
   // this is all done on the right stick, and L2 must be pressed to toggle
   // between zoom and focus
   // *there's gotta be a better way to factor this logic, check back when not tired
   if (js.getAxisMagnitude(DS4::RS_Y) >= 10.0f) {
     if (js.isBtnPressedRaw(DS4::L2)) {
       // we're going to focus
       if (js.getAxisPos(DS4::RS_Y) >= 10.0f) {
         create_ir_packet(it, p, node_get_focus_in(*it));
       } else if (js.getAxisPos(DS4::RS_Y) <= -10.0f) {
         create_ir_packet(it, p, node_get_focus_out(*it));
       }
     } else {
       // now we're going to zoom zoom zoom
       if (js.getAxisPos(DS4::RS_Y) >= 10.0f) {
         create_ir_packet(it, p, node_get_zoom_in(*it));
       } else if (js.getAxisPos(DS4::RS_Y) <= -10.0f) {
         create_ir_packet(it, p, node_get_zoom_out(*it));
       }
     }
     // if we made it here, then we need to send IR packet
     send_packet((*it)->rf, p);
   }

 }

 /*
  * Cycle node left
  */
void cycle_node_left( node_list_t &nl, node_list_it &it)
{
  if (it == nl.begin()) {
      it = std::prev(nl.end());
  } else {
    --it;
  }
}

 /*
  *  Cycle node right
*/
void cycle_node_right( node_list_t &nl, node_list_it &it)
{
  ++it;
  if (it == nl.end()) {
    it = nl.begin();
  }
}

void create_motor_packet(node_list_it &it, packet& p)
{
  float x_out, y_out;
  float norm_mag = js.getNormAxis(x_out, y_out, DS4::LS_X, DS4::LS_Y);
  float speed = norm_mag * 255 ;

  p.packet_type = 0;
  p.payload_used = 3;
  p.id = (*it)->id;
  p.payload[0] = (unsigned)speed;

  if (x_out > 0.0f)   p.payload[1] = 2;
  else if (x_out == 0.0f)  p.payload[1] = 0;
  else if (x_out < 0.0f)   p.payload[1] = 1;

  if (y_out > 0.0f) p.payload[2] = 2;
  else if (y_out == 0.0f) p.payload[2] = 0;
  else if (y_out < 0.0f)  p.payload[2] = 1;

  std::stringstream ss;
  ss  << "\nX raw: " << js.getAxisPos(DS4::LS_X)
                       << "\nY raw: " << js.getAxisPos(DS4::LS_Y)
                       << "\nX out: " << x_out
                       << "\nY out: " << y_out
                       << "\nSpeed: " << speed << std::endl;

}

void create_ir_packet(node_list_it &it, packet &p, uint32_t ir_code)
{
  p.packet_type = 1;
  p.payload_used = 5;
  p.id = (*it)->id;
  p.payload[0] = node_get_ir_prot((*it));
  p.payload[1] = ir_code >> 24;
  p.payload[2] = ir_code >> 16;
  p.payload[3] = ir_code >> 8;
  p.payload[4] = ir_code;

  print_packet(p);
}

void print_packet(const packet &p)
{
  std::stringstream ss;
  ss << "Packet Type: " << (unsigned)p.packet_type
     << "Payload Used: " << (unsigned)p.payload_used
     << "\nPacket ID; " << (unsigned)p.id;
  for (int i = 0; i < p.payload_used; ++i) {
    ss << "\nPayload[" << i << "]: " << std::hex << (unsigned)p.payload[i];
  }
}


void load_config(std::string file, node_list_t &nl)
{
  node_prop *np = NULL;

  // verify correct loading of file in future.
  YAML::Node base = YAML::LoadFile(filepat);
  base = base["camera_nodes"];

  for (auto it = base.begin(); it != base.end(); ++it) {
    np = new node_prop;
    node_init(np);
    node_set_id(np, (uint8_t)(*it)["id"].as<unsigned>()); // yaml-cpp has issue directly converting to uint8_t




  }

}
