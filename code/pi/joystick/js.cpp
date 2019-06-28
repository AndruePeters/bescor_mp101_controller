#include <iostream>
#include <cmath>
#include <joystick/js.h>


/*
  Only configuration required is to set the joystick number
*/
JS_State::JS_State(unsigned js_num)
{
    setJSNum(js_num);

    // set all axis to not inverted
    for (auto &x: inv_axes_state) {
      x = AXIS_NORMAL;
    }

    // set all buttons as not pressed
    for (unsigned i = 0; i < 32; ++i) {
      prev_btn_state [i] = 0;
      curr_btn_state [i] = 0;
    }
}

/*
  Updates stick information.
*/
void JS_State::update()
{
  prev_btn_state = curr_btn_state;
  sf::Joystick::update();
  // update current button state
  for (unsigned i = 0; i < getNumBtns(); ++i) {
    curr_btn_state[i] = isBtnPressedRaw(i);
  }
}

/*
  Only checks to see if js_num is less than the max number of buttons supported
  by SFML. Warns user if joystick is not connected.

  Returns true if successful and false otherwise.
*/
bool JS_State::setJSNum(unsigned js_num) {
  if (js_num > sf::Joystick::Count) return false;
  this->js_num = js_num;
  update();

  return true;
}

/*
  Waits for the current joystick to be connected.
  Be careful with use as it can lock up the program.
*/
void JS_State::waitUntilConnected() const
{
  while ( !sf::Joystick::isConnected(js_num) ) {}
}


/*
  Returns the current joystick number.
*/
unsigned JS_State::getJSNumber() const
{
  return js_num;
}


/*
  Returns number of buttons for current joystick number.
*/
unsigned JS_State::getNumBtns() const
{
  if (!isConnected()) return 0;
  return sf::Joystick::getButtonCount(js_num);
}

/*
  Returns true if joystick is connected, and false otherwise.
*/
bool JS_State::isConnected() const
{
  return sf::Joystick::isConnected(js_num);
}

/*
  Returns the status of whether or not the button was pressed.
  Set to true on press.

  Result from a simple boolean expression using a truth table.
  P and C are the previous and current button states

  | P | C | Out
  _____________
  | 0 | 0 | nothing
  | 0 | 1 | on press
  | 1 | 0 | on release
  | 1 | 1 | nothing

  As can be seen, !p and c = on press
                  p and !c = on release
*/
bool JS_State::isBtnPressed(Button btn, bool onPress) const
{
  if (!isConnected()) return false;
  if (btn >= getNumBtns()) return false;

  if (onPress) {
    return !prev_btn_state[btn] && curr_btn_state[btn];
  } else {
    return prev_btn_state[btn] && !curr_btn_state[btn];
  }
}

/*
  Returns the current value of the button without checking states.
*/
bool JS_State::isBtnPressedRaw(Button btn) const
{
  if (!isConnected()) return 0;
  return sf::Joystick::isButtonPressed(js_num, btn);
}

/*
  Returns value at the given axis.
*/
float JS_State::getAxisPos(Axis axis) const
{
  if (!isConnected()) return 0;
  return sf::Joystick::getAxisPosition(js_num, axis) * inv_axes_state[axis];
}

/*
  Sets invert state of X
*/
void JS_State::invertX(bool inv)
{
  inv_axes_state[X_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of Y
*/
void JS_State::invertY(bool inv)
{
  inv_axes_state[Y_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of Z
*/
void JS_State::invertZ(bool inv)
{
  inv_axes_state[Z_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of R
*/
void JS_State::invertR(bool inv)
{
  inv_axes_state[R_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of U
*/
void JS_State::invertU(bool inv)
{
  inv_axes_state[U_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of V
*/
void JS_State::invertV(bool inv)
{
  inv_axes_state[V_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of PovX
*/
void JS_State::invertPovX(bool inv)
{
  inv_axes_state[PovX_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}

/*
  Sets invert state of PovY
*/
void JS_State::invertPovY(bool inv)
{
  inv_axes_state[PovY_INV] = inv ? AXIS_NORMAL : AXIS_INVERTED;
}


/*
 *  Returns the name of the controller.
 *  Returns a blank string if controller is not connected.
 */
std::string JS_State::getName() const
{
  if (!isConnected()) return "";
  return sf::Joystick::getIdentification(js_num).name.toAnsiString();
}

/*
 *  Returns vendor ID for the controller.
 */
unsigned JS_State::getVendorID() const
{
  if (!isConnected()) return 0;
  return sf::Joystick::getIdentification(js_num).vendorId;
}


/*
 *  Returns the product ID for the controller.
 */
unsigned JS_State::getProductID() const
{
  if (isConnected()) return 0;
  return sf::Joystick::getIdentification(js_num).productId;
}


/*
  Normalizes axes X and Y,
  x_out is equal to scaled version of x
  y_out is equal to scaled version of y

  returns the normalized magnitude of X and Y
*/
float JS_State::getNormAxis(float &x_out, float &y_out, Axis x, Axis y) const
{
  float ls_x = getAxisPos(x);
  float ls_y = getAxisPos(y);
  float mag = std::sqrt( ls_x*ls_x + ls_y*ls_y);
  float range, scale, mag_norm = 0.0f;
  unsigned dead_zone_high = 5;
  unsigned dead_zone_low = 5;
  x_out = y_out = 0;

  // normalize, set radial boundaries
  if (mag > dead_zone_low) {
    range = 100 - dead_zone_high - dead_zone_low;
    mag_norm = std::fmin(1.0, (mag - dead_zone_high) / range);
    scale = mag_norm / mag;
    x_out = ls_x * scale;
    y_out = ls_y * scale;
  }

  // return normalized magnitude
  return mag_norm;
}

/******************************************************************************/
/*                      Private Methods                                       */
/******************************************************************************/

/*
  Prints warning that controller is not connected.
*/
void JS_State::printJsNotConnected() const
{
  std::cerr << "Joystick #" << js_num << " is not connected!" << std::endl;
}
