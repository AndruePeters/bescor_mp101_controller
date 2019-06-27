#include <main.h>
#include <packet.h>

#include <cmath>
#include <chrono>
#include <sstream>
#include <thread>
#include <iostream>
#include <iterator>
#include <curses.h>
using namespace controller;

void create_load_nodes(node_list_t &nl);
void cycle_node_right( node_list_t &nl, node_list_it &it);
void cycle_node_left( node_list_t &nl, node_list_it &it);
void process_input(node_list_t &nl, node_list_it &it);
void print_curr_node(node_list_it &it);
void create_motor_packet();
float normalize_axis(float &x_out, float &y_out, Axis a, Axis b, JS_State &js);
JS_State js(0);

int main()
{
  js.invertY(false);
  initscr();
  noecho();
  cbreak();
  int h,w;
  getmaxyx(stdscr, h, w);
  WINDOW *win = newwin(h, w, 0, 0);


  node_list_t nl;
  create_load_nodes(nl);
  auto it = nl.begin();
  while (1) {
    process_input(nl, it);
    erase();
    print_curr_node(it);
    create_motor_packet();
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }
}

void create_load_nodes(node_list_t &nl)
{
  node_prop *p = NULL;

  p = new node_prop;
  p->color = "Red    ";
  p->id = 0;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Orange  ";
  p->id = 1;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Yellow";
  p->id = 2;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Green      ";
  p->id = 3;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Blue         ";
  p->id = 4;
  nl.push_back(p);
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
  ss << (*it)->color <<  " : " << (*it)->id << std::endl;
  addstr(ss.str().c_str());
}

void create_motor_packet()
{
  packet p;
  float x_out, y_out;
  float norm_mag = normalize_axis(x_out, y_out, DS4::LS_X, DS4::LS_Y, js);
  float speed = norm_mag * 255 ;

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
