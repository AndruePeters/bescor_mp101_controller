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

int main()
{
  node_list_t nl;
  create_load_nodes(nl);
  auto it = nl.begin();
  while (1) {
    std::cout << "Color: " << (*it)->color << "\nID: " << (*it)->id << std::endl;
    cycle_node_left(nl, it);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

  }
}

void create_load_nodes(node_list_t &nl)
{
  node_prop *p = NULL;

  p = new node_prop;
  p->color = "Red";
  p->id = 0;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Orange";
  p->id = 1;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Yellow";
  p->id = 2;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Green";
  p->id = 3;
  nl.push_back(p);

  p = new node_prop;
  p->color = "Blue";
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
