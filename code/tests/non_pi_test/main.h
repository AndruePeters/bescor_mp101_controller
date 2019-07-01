#ifndef _BESCOR_MP101_NONPITEST_MAIN_H_
#define _BESCOR_MP101_NONPITEST_MAIN_H_

// Standard C++
#include <list>
#include <string>

// Custom stuff
#include <packet.h>
#include <joystick/js.h>
#include <joystick/controller_map.h>


#define UNKNOWN 0
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define PANASONIC_OLD 5
#define JVC 6
#define NECX 7
#define SAMSUNG36 8
#define GICABLE 9
#define DIRECTV 10
#define RCMM 11
#define CYKM 12

struct node_prop {
  std::string color;
  unsigned id;
  unsigned zoomin;
  unsigned zoomout;
  unsigned focusin;
  unsigned focusout;
};

typedef std::list<node_prop *> node_list_t;
typedef std::list<node_prop *>::iterator node_list_it;



#endif
