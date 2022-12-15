/*
 * Universal print functor
 *
 * It imitates the look of a python print function
 * with its unnecesarry parameters 'sep', 'end',
 * 'file' and 'flush', which has no particular 
 * place.
 *
 * The feature is to move as much instructions to
 * a compilation time, as possible, and make this
 * printer fast as standard ostream.
 *
 * It can print types with overloaded operator 
 * extraction.
 */
#include <uniprint.hpp>

#include <iostream>
#include <sstream>
#include <cassert>
#include <chrono>
#include <thread>

int main() {
  uniprint::printer<std::cout> print;
  using namespace uniprint::options;

  print("Hello from test!");

  // test 1
  {
    std::ostringstream ostream;

    print("Hello", "World", end{"!"}, file{ostream});
    assert(ostream.str() == "Hello World!");
  }

  // test 2
  {
    std::ostringstream ostream;

    print("Hello", "World", end{"!"}, sep{"_"}, file{ostream});
    assert(ostream.str() == "Hello_World!");
  }

  // test 3
  {
    std::ostringstream ostream;

    print(file{ostream}, "Hello", sep{"_"}, "World", end{"!"});
    assert(ostream.str() == "Hello_World!");
  }

  // test 4
  {
    std::ostringstream ostream;

    print(1, 2, 3, "Integer sequence", 
        1.41, 2.71, 3.14, "Floating point sequence", file{ostream});
    assert(ostream.str() ==
        "1 2 3 Integer sequence 1.41 2.71 3.14 Floating point sequence\n");
  }

  // test 5
  {
    for (int i = 1; i < 11; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(300));
      print(i, end{""}, flush{});
      for (int j = 0; j < 3; ++j) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        print('.', end{""}, flush{});
      }
    }
    print();
    print("All tests are passed!");
  }

  return 0;
}
