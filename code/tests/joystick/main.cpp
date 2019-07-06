#include <js.h>
#include <controller_map.h>

#include <chrono>
#include <iostream>
#include <iterator>
#include <list>
#include <string>
#include <sstream>
#include <thread>


#include <curses.h>
using namespace controller;
JS_State js(0);
std::list<unsigned> testList;
auto it = testList.begin();

int printDiagnostic()
{
  std::stringstream stream;
  js.update();

  if (js.isConnected()) {
    if ( js.isBtnPressedRaw(DS4::L1)) {
      --it;
    } else if (js.isBtnPressedRaw(DS4::R1)) {
      ++it;
    }

    stream << js.getName() << std::endl
              << js.getProductID() << std::endl
              << js.getVendorID() << std::endl;

    stream << "\nX: " << js.isBtnPressedRaw(DS4::X)
              << "\nO: " << js.isBtnPressedRaw(DS4::O)
              << "\nTri: " << js.isBtnPressedRaw(DS4::Tri)
              << "\nSqr: " << js.isBtnPressedRaw(DS4::Sqr)
              << "\nL1: " << js.isBtnPressedRaw(DS4::L1)
              << "\nR1: " << js.isBtnPressedRaw(DS4::R1)
              << "\nL2 " << js.isBtnPressedRaw(DS4::L2)
              << "\nR2 " << js.isBtnPressedRaw(DS4::R2)
              << "\nShare: " << js.isBtnPressedRaw(DS4::Share)
              << "\nOptions: " << js.isBtnPressedRaw(DS4::Opt)
              << "\nPS Btn: " << js.isBtnPressedRaw(DS4::PS)
              << "\nLS: " << js.isBtnPressedRaw(DS4::LS)
              << "\nRS: " << js.isBtnPressedRaw(DS4::RS)
              << "\n\nAxes:"
              << "\nLS_X: " << js.getAxisPos(DS4::LS_X)
              << "\nLS_Y: " << js.getAxisPos(DS4::LS_Y)
              << "\nL: " << js.getAxisPos(DS4::L)
              << "\nR: " << js.getAxisPos(DS4::R)
              << "\nRS_X: " << js.getAxisPos(DS4::RS_X)
              << "\nRS_Y: " << js.getAxisPos(DS4::RS_Y)
              << "\nPovX: " << js.getAxisPos(DS4::PovX)
              << "\nPovY: " << js.getAxisPos(DS4::PovY) << "\n"
              << "Current node: " << *it << std::endl;
      erase();
      addstr(stream.str().c_str());
      refresh();
  }
}

int main()
{
  initscr();
  noecho();
  cbreak();
  int h,w;
  getmaxyx(stdscr, h, w);
  js.update();
  WINDOW *win = newwin(h, w, 0,0);

  wrefresh(win);

  printDiagnostic();
  refresh();

  for (unsigned i = 0; i < 5; ++i) {
    testList.push_back(i);
  }
  it = testList.begin();
  while(1) {
    printDiagnostic();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }

}
