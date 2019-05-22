#ifndef _LOAD_CONFIG_H
#define _LOAD_CONFIG_H
#include <string>
#include <vector>
//#include <node.h>

struct node_prop
{
  int i;
  float j;
};

typedef std::vector<node_prop> node_vect;

node_vect* load_node_config(std::string filepath);
void dump_node(node_vect *nv);



#endif
