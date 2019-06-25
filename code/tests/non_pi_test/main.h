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
  unsigned id;
};

typedef std::list<node_prop *> node_list_t;
typedef std::list<node_prop *>::iterator node_list_it;



#endif
