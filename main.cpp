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

int main() {
	uniprint::printer<std::cout> print;
	using namespace uniprint::options;

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

	return 0;
}
