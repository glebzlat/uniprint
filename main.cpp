/*
 * Copyright (c) 2022 edKotinsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * Universal print functor imitates the look of a python print function
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
