#include <iostream>

#include <wiringPi.h>
#include <RF24.h>

// global radio instance
// using wiringPi, so wiringPi pin scheme is also used
// this is equivalent to radio(22, 0) in the generic gettingstarted.cpp
RF24 radio(3, 10);

int main()
{
  std::cout << "Ran successfully.\n";
  return 0;
}
