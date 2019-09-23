/*
  main.cpp
  5/1/19
  Andrue Peters

  This is the main file for the program.
  Better description later.
*/
#include <algorithm>
#include <cctype>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <iterator>
#include <csignal>

#include <RF24/RF24.h>
#include <wiringPi.h>

#include <curses.h>
// specific files projectc++c
#include <node.h>
#include <packet.h>
#include <joystick/controller_map.h>
#include <joystick/js.h>

#include <yaml-cpp/yaml.h>
#include <main.h>

using namespace controller;


// global radio instance
// using wiringPi, so wiringPi pin scheme is also used
// this is equivalent to radio(22, 0) in the generic gettingstarted.cpp
RF24 radio(3, 10);
JS_State js(0);
node_list_t node_list;

void ctrlCHandler(sig_atomic_t s)
{
  endwin();
  radio.printDetails();
  exit(1);
}

int main()
{
  // initialize wiringPi and RF24
  wiringPiSetup();
  rf24_init();
  signal (SIGINT,ctrlCHandler);
  // load config file and store items in node_list
  load_config("config.yaml", node_list);
  node_list_it curr_node = node_list.begin();

  init_display();
  while (1) {
    clear();
    display_status(curr_node);
    addstr("\n\n");
    process_input(node_list, curr_node);
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  return 0;
}

/*
 *  Initialize rf24 module to defaults.
 */
void rf24_init()
{
  radio.begin();
  //radio.setAutoAck(true);
  //radio.setRetries(2, 15);
  radio.setChannel(1);
  radio.openWritingPipe(ADDRESSES[1]);
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
 *  Sends a packet.
 */
void send_packet(const nrf2401_prop &n, const packet &p)
{
  //match_node_radio(n);

  // First, stop listening so we can talk.
  radio.stopListening();
  // attempt to send packet
  if (!radio.write( &p, sizeof(packet))) {

  }
}

/*
 *  Sets the raspberry pi write address to the arduino node listening address
 */
 void set_rf24_write_addr(const address_e listening_addr)
 {
   std::cout << "listening_addr: " << listening_addr << "\n";
   radio.openWritingPipe(ADDRESSES[listening_addr]);
   radio.openReadingPipe(1, ADDRESSES[0]);
 }

 /*
  * Handle and process the input of a joystick
  */
void process_input(node_list_t &nl, node_list_it &it)
 {
   if (!js.isConnected()) {
     addstr("Joystick is not connected. Please connect it to continue.\n\n");
     js.waitUntilConnected();
    }

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
     print_packet(p);
     send_packet((*it)->rf, p);
   }

   // now time to process IR stuff
   // this is all done on the right stick, and L2 must be pressed to toggle
   // between zoom and focus
   // *there's gotta be a better way to factor this logic, check back when not tired
   if (js.getAxisMagnitude(DS4::RS_Y) >= 50.0f) {
     if (js.isBtnPressedRaw(DS4::L2)) {
       // we're going to focus
       if (js.getAxisPos(DS4::RS_Y) >= 50.0f) {
         create_ir_packet(it, p, node_get_focus_in(*it));
       } else if (js.getAxisPos(DS4::RS_Y) <= -50.0f) {
         create_ir_packet(it, p, node_get_focus_out(*it));
       }
     } else {
       // now we're going to zoom zoom zoom
       if (js.getAxisPos(DS4::RS_Y) >= 50.0f) {
         create_ir_packet(it, p, node_get_zoom_in(*it));
       } else if (js.getAxisPos(DS4::RS_Y) <= -50.0f) {
         create_ir_packet(it, p, node_get_zoom_out(*it));
       }
     }
     // if we made it here, then we need to send IR packet
     print_packet(p);
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

/*
 *  Creates motor packet.
 */
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

/*
 *  Creates the IR packet.
 */
void create_ir_packet(node_list_it &it, packet &p, uint32_t ir_code)
{
  p.packet_type = 1;
  p.payload_used = 5;
  p.id = node_get_id(*it);
  p.payload[0] = node_get_ir_prot((*it));
  p.payload[1] = ir_code >> 24;
  p.payload[2] = ir_code >> 16;
  p.payload[3] = ir_code >> 8;
  p.payload[4] = ir_code;
}

/*
 *  Dumps packet information to screen;
 */
void print_packet(const packet &p)
{
  std::stringstream ss;
  ss << "\nPacket Type: " << (unsigned)p.packet_type
     << "\nPayload Used: " << (unsigned)p.payload_used
     << "\nPacket ID: " << (unsigned)p.id;
  for (int i = 0; i < p.payload_used; ++i) {
    ss << "\nPayload[" << i << "]: " << std::hex << (unsigned)p.payload[i];
  }
  addstr(ss.str().c_str());
}

/*
 *  Reads and parses a yaml file (file) and stores nodes in nl
 */
void load_config(std::string file, node_list_t &nl)
{
  node_prop *np = NULL;
  // verify correct loading of file in future.
  YAML::Node base = YAML::LoadFile(file.c_str());
  YAML::Node arr = base["camera_nodes"];
  std::cout << "\nLoaded file\n";

  for (auto it = arr.begin(); it != arr.end(); ++it) {
    np = new node_prop;
    node_init(np);
    node_set_id(np, (*it)["id"].as<unsigned>()); // yaml-cpp has issue directly converting to uint8_t
    node_set_color(np, str_to_clr((*it)["color"].as<std::string>()));

    // configure rf properties
    node_set_power_level(np,  str_to_pwr( (*it)["power_level"].as<std::string>()) );
    node_set_data_rate(np,    str_to_datarate( (*it)["data_rate"].as<std::string>() ));
    node_set_crc_length(np,   str_to_crclen( (*it)["crc_length"].as<std::string>() ));
    node_set_channel(np,      (uint8_t)(*it)["channel"].as<unsigned>() ); // yaml-cpp has issue converting to uint8_t
    node_set_address(np,      str_to_addr( (*it)["address"].as<std::string>() ));

    // configure ir properties
    node_set_ir_prot(np,      str_to_irprot( (*it)["ir_proto"].as<std::string>() ));
    node_set_zoom_in(np,      (*it)["zoom_in"].as<uint32_t>() );
    node_set_zoom_out(np,     (*it)["zoom_out"].as<uint32_t>() );
    node_set_focus_in(np,     (*it)["focus_in"].as<uint32_t>() );
    node_set_focus_out(np,    (*it)["focus_out"].as<uint32_t>() );

    // now store this created node
    nl.push_back(np);
  }
}

/*
 *  Dumps information for the current node.
 */
void print_curr_node(node_list_it &it)
{
  std::stringstream ss;
  node_prop *np = (*it);
  ss << "\nColor:\t" << (unsigned)node_get_color(np)
     << "\nID:\t" << (unsigned)node_get_id(np)
     << "\nZoomIn:\t" << node_get_zoom_in(np)
     << "\nZoomOut:\t" << node_get_zoom_out(np)
     << "\nFocus In:\t" << node_get_focus_in(np)
     << "\nFocus Out: " << node_get_focus_out(np)
     << "\n\n";
  addstr(ss.str().c_str());
}


/*
 *  Returns the color_e version of color.
 *  Valid string values: OFF, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, WHITE
 */
color_e str_to_clr(std::string color)
{
  color_e ret;
  // transform string to upper for comparison
  std::transform(color.begin(), color.end(), color.begin(), ::toupper);
  if (color == "BLUE") {
    ret = BLUE;
  } else if (color == "GREEN") {
    ret = GREEN;
  } else if (color == "CYAN") {
    ret = CYAN;
  } else if (color == "RED") {
    ret = RED;
  } else if (color == "MAGENTA") {
    ret = MAGENTA;
  } else if (color == "YELLOW") {
    ret = YELLOW;
  } else if (color == "WHITE") {
    ret = WHITE;
  } else {
    ret = OFF;
  }

  return ret;
}

/*
 * Converts power to rf24_pa_dbm_e
 * Valid string values: MIN, LOW, HIGH, MAX
 */
rf24_pa_dbm_e str_to_pwr(std::string power)
{
  rf24_pa_dbm_e ret;
  std::transform(power.begin(), power.end(), power.begin(), ::toupper);

  if (power == "MIN") {
    ret = RF24_PA_MIN;
  } else if (power == "LOW") {
    ret = RF24_PA_LOW;
  } else if (power == "HIGH") {
    ret = RF24_PA_HIGH;
  } else if (power == "MAX") {
    ret = RF24_PA_MAX;
  } else {
    ret = RF24_PA_ERROR;
  }

  return ret;
}


/*
 * Converts string dr to rf24_datarate_e
 * Valid string values: 250KBPS, 1MBPS, 2MBPS
 */
rf24_datarate_e str_to_datarate(std::string dr)
{
  rf24_datarate_e ret;
  std::transform(dr.begin(), dr.end(), dr.begin(), ::toupper);

  if (dr == "250KBPS") {
    ret = RF24_250KBPS;
  } else if (dr == "1MBPS") {
    ret = RF24_1MBPS;
  } else if (dr == "2MBPS") {
    ret = RF24_2MBPS;
  } else {
    ret = RF24_250KBPS;
  }

  return ret;
}

/*
 *  Converts string crc to rf24_crclength_e
 *  Valid string values: DISABLED, CRC_8, CRC_16
 */
rf24_crclength_e str_to_crclen(std::string crc)
{
  rf24_crclength_e ret;
  std::transform(crc.begin(), crc.end(), crc.begin(), ::toupper);

  if (crc == "CRC_8") {
    ret = RF24_CRC_8;
  } else if (crc == "CRC_16") {
    ret = RF24_CRC_16;
  } else {
    ret = RF24_CRC_DISABLED;
  }

  return ret;
}

/*
 * Converts string addr to address_e
 * Valid string values: A1, A2, A3, A4, A5
 */
address_e str_to_addr(std::string addr)
{
  address_e ret;
  std::transform(addr.begin(), addr.end(), addr.begin(), ::toupper);

  if (addr == "A1") {
    ret = A1_ADDR;
  } else if (addr == "A2") {
    ret = A2_ADDR;
  } else if (addr == "A3") {
    ret = A3_ADDR;
  } else if (addr == "A4") {
    ret = A4_ADDR;
  } else if (addr == "A5") {
    ret = A5_ADDR;
  } else {
    ret = A1_ADDR;
  }
  return ret;
}

/*
 * Convert string ir to ir_prot_e
 * Valid string values: NEC, SONY, RC5, RC6, PANASONIC_OLD, JVC, NECX, SAMSUNG36, GICABLE, DIRECTV, RCMM, CKYM
 */
ir_prot_e str_to_irprot(std::string ir)
{
  ir_prot_e ret;
  std::transform(ir.begin(), ir.end(), ir.begin(), ::toupper);

  if (ir == "NEC") {
    ret = IR_NEC;
  } else if (ir == "SONY") {
    ret = IR_SONY;
  } else if (ir == "RC5") {
    ret = IR_RC5;
  } else if (ir == "RC6") {
    ret = IR_RC6;
  } else if (ir == "PANASONIC_OLD") {
    ret = IR_PANASONIC_OLD;
  } else if (ir == "JVC") {
    ret = IR_JVC;
  } else if (ir == "NECX") {
    ret = IR_NECX;
  } else if (ir == "SAMSUNG36") {
    ret = IR_SAMSUNG36;
  } else if (ir == "GICABLE") {
    ret = IR_GICABLE;
  } else if (ir == "DIRECTV") {
    ret = IR_DIRECTV;
  } else if (ir == "RCMM") {
    ret = IR_RCMM;
  } else if (ir == "CKYM") {
    ret = IR_CYKM;
  } else {
    ret = IR_UNKNOWN;
  }

  return ret;
}

void init_display()
{
  initscr();
  noecho();
  cbreak();
}

void display_status(node_list_it &it)
{
  std::stringstream ss;
  ss << "Current Node: " << (unsigned)node_get_id(*it) << std::endl;
  addstr(ss.str().c_str());
}
