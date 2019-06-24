#include <main.h>


#include <chrono>
#include <sstream>
#include <thread>

#include <curses.h>
using namespace controller;


bescor_environment be;

int main()
{
  create_nodes(be.node_list);
  be.nl_it = be.node_list.begin();

  // set up ncruses environment
  initscr();
  noecho();
  cbreak();
  int h,w;
  getmaxyx(stdscr, h, w);
  WINDOW *win = newwin(h, w, 0,0);
  refresh();


  while(1) {
    be.js.update();
    process_input();
    erase();
    print_current_node();
    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }

}

void create_nodes(node_list_t &nl)
{
  node_prop *np;
  np = new node_prop;
  np->color = "Red";
  np->id = 0;
  nl.push_back(np);
  np = new node_prop;
  np->color = "Orange";
  np->id = 1;
  nl.push_back(np);
  np = new node_prop;
  np->color = "Yellow";
  np->id = 2;
  nl.push_back(np);
  np = new node_prop;
  np->color = "Green";
  np->id = 3;
  nl.push_back(np);
  np = new node_prop;
  np->color = "Blue";
  np->id = 4;
  nl.push_back(np);
}

void process_input()
{
  if (!be.js.isConnected()) {
     addstr("Joystick not connected\n");
     return;
  }

  if (be.js.isBtnPressed(DS4::L1)) {
    node_list_cycle_left();
  } else if (be.js.isBtnPressed(DS4::L2)) {
    node_list_cycle_right();
  }


}

void node_list_cycle_left()
{
  --be.nl_it;
}

void node_list_cycle_right()
{
  ++be.nl_it;
}


void print_current_node()
{
  std::stringstream ss;
  ss << "Color:\t" << (*be.nl_it)->color
     << "ID:\t" << (*be.nl_it)->id << std::endl;
  addstr(ss.str().c_str());
}
