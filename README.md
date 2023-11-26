# Uniprint

C++ Python-like print function with keyword arguments. Main idea of this thing
is the keyword arguments implementation in pure C++11.

To make things short, I'll give some code examples of Python's `print` function
and compare it with Uniprint.

Python:

```python
>>> print("Hello", "world", sep=', ', end='!\n')
Hello, world!
>>> print(1, 2, 3, sep='\n')
1
2
3
>>> from sys import stderr
>>> print("Message", file=stderr)

```

C++ Uniprint:

```cpp
uniprint::print print(std::cout);
using namespace uniprint::args;

print("Hello", "world", sep{", "}, end{"!\n"});
print(1, 2, 3, sep{"\n"});
print(file{std::cerr}, "Message"); // yep, kwarg before positional
```

So, does it look similar?

As I said earlier, the main idea of Uniprint is
[keyword arguments](https://docs.python.org/3/glossary.html#term-argument)
implementation in C++.

Uniprint also has `flush` and `file` arguments as the original `print` function.

## How it is made

The implementation of Uniprint includes the `gfp.hpp` header. GFP means Get From
Pack, and it does what it says: it takes the pack of values and extracts the
values with specific types from it.

This is how it looks:

```cpp
class print {
public:
  template <typename... Types> void operator()(Types &&...args) {
   // ...
     auto sep = gfp::get_from_pack<args::sep>{}(std::forward<Types>(args)...);

     gfp::call_match(
        sep,
        [&print_args](args::sep a) { print_args.sep = a.get(); },
        [](gfp::none_type) {}
     );
   // ...
  }
}
```

This code example is taken from the `uniprint.hpp` header. `gfp::get_from_pack`
returns value of some (unknown) type, which is assigned to `sep`. Then the
`gfp::call_match` is used to handle this value based on its type. If the type
of `sep` is `args::sep`, then the separator's value is assigned to
`print_args.sep`, otherwise (the type if `sep` is `gfp::none_type`) nothing
happens. `call_match` calls the functor, which argument type matches the type of
passed value.

This project is written just for fun, because I was wondering: "is it possible
in C++?". The reason why it is written in C++11 is the same, because it was
interesting to implement this without handy new tools like `constexpr if`
and so on. It may be used in real-world projects, though I don't know, will it
be a good idea.

Licensed under MIT License.
