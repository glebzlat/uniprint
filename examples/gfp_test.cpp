#include <cassert>
#include <gfp.hpp>
#include <iostream>
#include <type_traits>

int main() {
  std::cout << std::boolalpha;

  {
    using t = decltype(gfp::get_from_pack<int>{}('a', "hello", 10));

    auto val = gfp::get_from_pack<int>{}('a', "hello", 10);

    assert((std::is_same<int, t>::value));
    assert(val == 10);
  }

  {
    auto val = gfp::get_from_pack<int>{}('a', "hello", 3.14);

    assert((std::is_same<gfp::none_type, decltype(val)>::value));
  }

  {
    const int value = 10;
    auto result = gfp::get_from_pack<int>{}('a', "hello", value);

    using t = decltype(gfp::get_from_pack<int>{}('a', "hello", value));

    assert((std::is_same<const int&, t>::value));
    assert((std::is_same<int, decltype(result)>::value));
    assert(result == 10);
  }

  return 0;
}
