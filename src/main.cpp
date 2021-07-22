#include "ncurses_display.h"
#include "system.h"
// #include "Utils.h"

int main() {
  System system;
//   Utils obj;
//   std::string text = "top";
//   std::cout << text << obj.exec(text.c_str()) << std::endl;

  NCursesDisplay::Display(system);
//   return 0;
  }