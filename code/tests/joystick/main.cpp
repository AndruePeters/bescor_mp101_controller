#include <js.h>
#include <iostream>
#include <string>


int main()
{
  JS_State js(0);
  js.update();
  std::cout << js.getName() << std::endl
            << js.getProductID() << std::endl
            << js.getVendorID() << std::endl;


}
