#include "uniprint.hpp"
#include <cassert>
#include <iostream>
#include <sstream>

int main() {
  uniprint::print print(std::cout);

  using namespace uniprint::args;

  print("Hello", "world!");
  print(sep{"_"}, "Underscores", "instead", "of", "spaces");
  print(end{"!"}, "Hello", "world", "again");
  print(" WOW!", "No EOF");

  print();

  print(3, sep{"."}, 14, end{" - "});
  print("This is PI", end{""});
  print(end{"!"});

  print();

  {
    std::ostringstream s;

    print(file{s}, "Hello", "World", '!');

    assert(s.str() == std::string{"Hello World !\n"});
  }

  {
    std::ostringstream s;

    print(file{s}, "Hello", "World", end{"!"});

    assert(s.str() == std::string{"Hello World!"});
  }

  return 0;
}
