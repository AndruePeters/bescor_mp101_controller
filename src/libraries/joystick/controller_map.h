/*
  Controller mappings for common controllers.
  Currently only supporting DualShock 4, but this will change in the future.
*/

#ifndef _BESCOR_MP101_PS4_MAP_H_
#define _BESCOR_MP101_PS4_MAP_H_

// this will include any dependencies I need no matter controller backend
#include <joystick/js.h>

namespace controller {
namespace DS4 {
const Button X = 0;
const Button O = 1;
const Button Tri = 2;
const Button Sqr = 3;
const Button L1 = 4;
const Button R1 = 5;
const Button L2 = 6;
const Button R2 = 7;
const Button Share = 8;
const Button Opt = 9;
const Button PS = 10;
const Button LS = 11;
const Button RS = 12;
const Axis LS_X = Axis::X;
const Axis LS_Y = Axis::Y;
const Axis L = Axis::Z;
const Axis R = Axis::R;
const Axis RS_X = Axis::U;
const Axis RS_Y = Axis::V;
const Axis PovX = Axis::PovX;
const Axis PovY = Axis::PovY;
}
}
#endif
