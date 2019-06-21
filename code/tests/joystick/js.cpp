#include <iostream>
#include <js.h>


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
}

/*
  Updates stick information.
*/
void JS_State::update()
{
  sf::Joystick::update();
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

  if ( !isConnected() ) {
    std::cerr << "Joystick #" << js_num << " is not connected!" << std::endl;
  }

  return true;
}

/*
  Waits for the current joystick to be connected.
  Be careful with use as it can lock up the program.
*/
void JS_State::waitUntilConnected()
{
  std::cout << "Waiting until Joystick #" << js_num << " is connected!" << std::endl;
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
  if ( isConnected() ) {
    return sf::Joystick::getButtonCount(js_num);
  } else {
    printJsNotConnected();
    std::cerr << "Can't return the number of buttons." << std::endl;
    return 0;
  }
}

/*
  Returns true if joystick is connected, and false otherwise.
*/
bool JS_State::isConnected() const
{

  return sf::Joystick::isConnected(js_num);
}

/*
  Returns the value of btn. True if pressed, false if not.
  Warns user if joystick not connected and returns false.
*/
bool JS_State::isBtnPressed(Button btn) const
{
  if ( isConnected() ) {
    return sf::Joystick::isButtonPressed(js_num, btn);
  } else {
    printJsNotConnected();
    std::cerr << "Cant' get button value." << std::endl;
    return 0;
  }
}

/*
  Returns value at the given axis.
*/
float JS_State::getAxisPos(Axis axis) const
{
  if ( isConnected() ) {
    return sf::Joystick::getAxisPosition(js_num, axis) * inv_axes_state[axis];
  } else {
    printJsNotConnected();
    std::cerr << "Cant' get axis value." << std::endl;
    return 0;
  }
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

std::string JS_State::getName()
{
  if ( isConnected() ) {
    return sf::Joystick::getIdentification(js_num).name.toAnsiString();
  } else {
    printJsNotConnected();
    return std::string("no joystick connected");
  }
}


unsigned JS_State::getVendorID()
{
  if ( isConnected() ) {
    return sf::Joystick::getIdentification(js_num).vendorId;
  } else {
    printJsNotConnected();
    return 0;
  }
}

unsigned JS_State::getProductID()
{
  if ( isConnected() ) {
    return sf::Joystick::getIdentification(js_num).productId;
  } else {
    printJsNotConnected();
    return 0;
  }
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
