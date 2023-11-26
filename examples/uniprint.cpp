#include "uniprint.hpp"
#include <iostream>

int main() {
  uniprint::print print(std::cout);
  using namespace uniprint::args;

  print("Hello", "world", sep{", "}, end{"!\n"});
  print(1, 2, 3, sep{"\n"});
  print(file{std::cerr}, "Message"); // yep, kwarg before positional

  return 0;
}
