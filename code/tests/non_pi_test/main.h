#ifndef _BESCOR_MP101_NONPITEST_MAIN_H_
#define _BESCOR_MP101_NONPITEST_MAIN_H_

// Standard C++
#include <list>
#include <string>

// Custom stuff
#include <packet.h>
#include <js.h>
#include <controller_map.h>


struct node_prop {
  std::string color;
  uint8_t id;
};

typedef std::list<node_prop *> node_list_t;
typedef std::list<node_prop *>::iterator node_list_it;



struct bescor_environment {
  node_list_t node_list;
  node_list_it nl_it;
  JS_State js;

  bescor_environment(unsigned js_num = 0) : js(js_num) {}
};



void create_nodes(node_list_t &nl);
void process_input();
void node_list_cycle_left();
void node_list_cycle_right();
void print_current_node();


#endif
