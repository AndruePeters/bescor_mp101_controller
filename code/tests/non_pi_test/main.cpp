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
  float spd_f;
  float ls_x = js.getAxisPos(DS4::LS_X);
  float ls_y = js.getAxisPos(DS4::LS_Y);
  float mag = std::sqrt( ls_x*ls_x + ls_y*ls_y);
  float theta = std::atan2(ls_y, ls_x) * 180 / 3.14159265;
  float theta_norm = 0;
  float dead_high = 0;
  float dead_low = 15;
  float out_x = 0;
  float out_y = 0;
  float range = 0;
  float scale = 0;


float normalizedMag = 0;
  if (mag > dead_low) {
    range = 100 - dead_high - dead_low;
    normalizedMag = std::fmin(1.0, (mag - dead_high) / range);
    scale = normalizedMag / mag;
    out_x = ls_x * scale;
    out_y = ls_y * scale;
  } else {
    out_x = out_y = 0;
  }
  theta_norm = std::atan2(out_y, out_x) * 180 / 3.14159265;
  spd_f = normalizedMag * 255;

  std::stringstream ss;
  ss << "\nRaw X: " << ls_x
     << "\nRaw Y: " << ls_y
     << "\nOut X: " << out_x
     << "\nOut Y: " << out_y
     << "\nMag(x,y): " << mag
     << "\nMag Norm: " << normalizedMag
     << "\nAngle(x,y): " << theta
     << "\nAngle Norm: " << theta_norm
     << "\nSpeed: " << spd_f
     << "\nScale: " << scale
     << std::endl;

  addstr(ss.str().c_str());

}
