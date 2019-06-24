#include <js.h>
#include <iostream>
#include <string>
#include <sstream>
#include <curses.h>

JS_State js(0);

void clearScreen()
{
  std::cout << "\033[2J\033[1;1H";
}

void printDiagnostic()
{
  std::stringstream stream;
  js.update();

  if (js.isConnected()) {
    stream << js.getName() << std::endl
              << js.getProductID() << std::endl
              << js.getVendorID() << std::endl;

    stream << "\nX: " << js.isBtnPressed(0)
              << "\nO: " << js.isBtnPressed(1)
              << "\nTri: " << js.isBtnPressed(2)
              << "\nSqr: " << js.isBtnPressed(3)
              << "\nL1: " << js.isBtnPressed(4)
              << "\nR1: " << js.isBtnPressed(5)
              << "\nL2 " << js.isBtnPressed(6)
              << "\nR2 " << js.isBtnPressed(7)
              << "\nShare: " << js.isBtnPressed(8)
              << "\nOptions: " << js.isBtnPressed(9)
              << "\nPS Btn: " << js.isBtnPressed(10)
              << "\nLS: " << js.isBtnPressed(11)
              << "\nRS: " << js.isBtnPressed(12)
              << "\n\nAxes:"
              << "\nX: " << js.getAxisPos(Axis::X)
              << "\nY: " << js.getAxisPos(Axis::Y)
              << "\nZ: " << js.getAxisPos(Axis::Z)
              << "\nR: " << js.getAxisPos(Axis::R)
              << "\nU: " << js.getAxisPos(Axis::U)
              << "\nV: " << js.getAxisPos(Axis::V)
              << "\nPovX: " << js.getAxisPos(Axis::PovX)
              << "\nPovY: " << js.getAxisPos(Axis::PovY) << std::endl;
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
  }

}
