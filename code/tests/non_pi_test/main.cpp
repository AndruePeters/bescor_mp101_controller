#include <main.h>
#include <packet.h>
#include <yaml-cpp/yaml.h>

#include <cmath>
#include <chrono>
#include <sstream>
#include <thread>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <curses.h>
using namespace controller;

void create_load_nodes(node_list_t &nl);
void cycle_node_right( node_list_t &nl, node_list_it &it);
void cycle_node_left( node_list_t &nl, node_list_it &it);
void process_input(node_list_t &nl, node_list_it &it);
void print_curr_node(node_list_it &it);
void create_motor_packet(node_list_it &it, packet& p);
void create_ir_packet(node_list_it &it, packet &p, uint32_t ir_code);
void print_packet(const packet &p);
float normalize_axis(float &x_out, float &y_out, Axis a, Axis b, JS_State &js);
JS_State js(0);

int main()
{
  packet p;
  js.invertY(false);
  /*initscr();
  noecho();
  cbreak();
  int h,w;
  getmaxyx(stdscr, h, w);
  WINDOW *win = newwin(h, w, 0, 0);*/


  node_list_t nl;
  create_load_nodes(nl);
  auto it = nl.begin();

  for (auto i = nl.begin(); i != nl.end(); ++i) {
    print_curr_node(i);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  }
  while (0) {
    process_input(nl, it);
    erase();
    print_curr_node(it);
    //create_ir_packet(it, p, 0x12345678);
    create_motor_packet(it, p);
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

void create_load_nodes(node_list_t &nl)
{
  node_prop *p = NULL;
  YAML::Node base = YAML::LoadFile("config.yaml");
  base = base["camera_nodes"];

  for (auto it = base.begin(); it != base.end(); ++it) {
    std::cout << "\nID: " << (unsigned)(*it)["id"].as<unsigned>()
              << "\nColor: " << (*it)["color"].as<std::string>()
              << "\nZoom In: " << std::hex << (*it)["zoom_in"].as<uint32_t>()
              << "\nZoom Out: " << (*it)["zoom_out"].as<uint32_t>()
              << "\nFocus In: " << (*it)["focus_in"].as<uint32_t>()
              << "\nFocus Out: " << (*it)["focus_out"].as<uint32_t>()
              << std::endl;
    p = new node_prop;
    p->id = (uint8_t)(*it)["id"].as<unsigned>();
    p->color = (*it)["color"].as<std::string>();
    p->zoomin = (*it)["zoom_in"].as<uint32_t>();
    p->zoomout = (*it)["zoom_out"].as<uint32_t>();
    p->focusin = (*it)["focus_in"].as<uint32_t>();
    p->focusout = (*it)["focus_out"].as<uint32_t>();
  }
}

void cycle_node_right( node_list_t &nl, node_list_it &it)
{
  ++it;
  if (it == nl.end()) {
    it = nl.begin();
  }
}

void cycle_node_left( node_list_t &nl, node_list_it &it)
{
  if (it == nl.begin()) {
    it = std::prev(nl.end());
  } else {
    --it;
  }
}

void process_input(node_list_t &nl, node_list_it &it)
{
  js.update();
  if (!js.isConnected()) return;
  if (js.isBtnPressed(DS4::L1)) {
    cycle_node_left(nl, it);
  } else if (js.isBtnPressed(DS4::R1)) {
    cycle_node_right(nl, it);
  }
}

void print_curr_node(node_list_it &it)
{
  std::stringstream ss;
  node_prop *np = (*it);
  ss << "\nColor:\t" << np->color
     << "\nID:\t" << (unsigned)np->id
     << "\nZoomIn:\t" << np->zoomin
     << "\nZoomOut:\t" << np->zoomout
     << "\nFocus In:\t" << np->focusin
     << "\nFocus Out: " << np->focusout
     << std::endl;
  //addstr(ss.str().c_str());
  std::cout << ss.str();
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

  print_packet(p);

  std::stringstream ss;
  ss  << "\nX raw: " << js.getAxisPos(DS4::LS_X)
                       << "\nY raw: " << js.getAxisPos(DS4::LS_Y)
                       << "\nX out: " << x_out
                       << "\nY out: " << y_out
                       << "\nSpeed: " << speed << std::endl;

  addstr(ss.str().c_str());
}

float normalize_axis(float &x_out, float &y_out, Axis x, Axis y, JS_State &js)
{
  float ls_x = js.getAxisPos(x);
  float ls_y = js.getAxisPos(y);
  float mag = std::sqrt( ls_x*ls_x + ls_y*ls_y);
  float range, scale, mag_norm = 0.0f;
  unsigned dead_zone_high = 5;
  unsigned dead_zone_low = 5;
  x_out = y_out = 0;

  // normalize, set radial boundaries
  if (mag > dead_zone_low) {
    range = 100 - dead_zone_high - dead_zone_low;
    mag_norm = std::fmin(1.0, (mag - dead_zone_high) / range);
    scale = mag_norm / mag;
    x_out = ls_x * scale;
    y_out = ls_y * scale;
  }

  // return normalized magnitude
  return mag_norm;
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
  addstr(ss.str().c_str());
}

void create_ir_packet(node_list_it &it, packet &p, uint32_t ir_code)
{
  uint8_t mask = 0xFF;

  p.packet_type = 1;
  p.payload_used = 5;
  p.id = (*it)->id;
  p.payload[0] = 1;
  p.payload[1] = ir_code >> 24;
  p.payload[2] = ir_code >> 16;
  p.payload[3] = ir_code >> 8;
  p.payload[4] = ir_code;

  print_packet(p);
}
