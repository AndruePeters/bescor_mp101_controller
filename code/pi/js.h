#ifndef _BESCOR_MP101_CONTROLLER_JS_
#define _BESCOR_MP101_CONTROLLER_JS_

#include <array>
#include <string>
#include <bitset>
#include <SFML/Window/Joystick.hpp>
const int AXIS_INVERTED = -1;
const int AXIS_NORMAL = 1;

typedef unsigned Button;
typedef sf::Joystick::Axis Axis;

enum INV_AXIS {
  X_INV = 0, Y_INV, Z_INV, R_INV, U_INV, V_INV, PovX_INV, PovY_INV
};

/*
  Wrapper class for SFML
*/
class JS_State {
public:
  JS_State(unsigned js_num);
  void update();
  bool setJSNum(unsigned js_num);
  void waitUntilConnected();
  unsigned getJSNumber() const;
  unsigned getNumBtns() const;
  bool isConnected() const;
  bool isBtnPressed(Button btn, bool onPress=true) const;
  bool isBtnPressedRaw(Button btn) const;
  float getAxisPos(Axis axis) const;
  void invertX(bool inv);
  void invertY(bool inv);
  void invertR(bool inv);
  void invertZ(bool inv);
  void invertU(bool inv);
  void invertV(bool inv);
  void invertPovX(bool inv);
  void invertPovY(bool inv);
  std::string getName();
  unsigned getVendorID();
  unsigned getProductID();

private:
  unsigned js_num;
  std::array <bool, 32> prev_btn_state;
  std::array <bool, 32> curr_btn_state;
  std::array <bool, 32> btn_state;
  std::array <int, 8> inv_axes_state;
  void printJsNotConnected() const;

};






















































#endif
