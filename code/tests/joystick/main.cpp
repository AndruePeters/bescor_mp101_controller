#include <js.h>
#include <controller_map.h>

#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>


#include <curses.h>
using namespace controller;
JS_State js(0);

int printDiagnostic()
{
  std::stringstream stream;
  js.update();

  if (js.isConnected()) {
    stream << js.getName() << std::endl
              << js.getProductID() << std::endl
              << js.getVendorID() << std::endl;

    stream << "\nX: " << js.isBtnPressed(DS4::X)
              << "\nO: " << js.isBtnPressed(DS4::O)
              << "\nTri: " << js.isBtnPressed(DS4::Tri)
              << "\nSqr: " << js.isBtnPressed(DS4::Sqr)
              << "\nL1: " << js.isBtnPressed(DS4::L1)
              << "\nR1: " << js.isBtnPressed(DS4::R1)
              << "\nL2 " << js.isBtnPressed(DS4::L2)
              << "\nR2 " << js.isBtnPressed(DS4::R2)
              << "\nShare: " << js.isBtnPressed(DS4::Share)
              << "\nOptions: " << js.isBtnPressed(DS4::Opt)
              << "\nPS Btn: " << js.isBtnPressed(DS4::PS)
              << "\nLS: " << js.isBtnPressed(DS4::LS)
              << "\nRS: " << js.isBtnPressed(DS4::RS)
              << "\n\nAxes:"
              << "\nLS_X: " << js.getAxisPos(DS4::LS_X)
              << "\nLS_Y: " << js.getAxisPos(DS4::LS_Y)
              << "\nL: " << js.getAxisPos(DS4::L)
              << "\nR: " << js.getAxisPos(DS4::R)
              << "\nRS_X: " << js.getAxisPos(DS4::RS_X)
              << "\nRS_Y: " << js.getAxisPos(DS4::RS_Y)
              << "\nPovX: " << js.getAxisPos(DS4::PovX)
              << "\nPovY: " << js.getAxisPos(DS4::PovY) << std::endl;
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
  while(1) {
    printDiagnostic();
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
  }

}
