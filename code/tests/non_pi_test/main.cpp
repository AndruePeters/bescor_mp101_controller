#include <main.h>


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
JS_State js(0);

int main()
{
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
