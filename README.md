# uniprint

Print functor with keyword arguments, that imitates Python's print function.
Written in C++ 11.

As you know, in Python you can write

```python
print("Hello", "world")
```

and `print` function will add spaces and newline symbol for you, so the output
will be `Hello world`. Also, you can override the default function options.
For example, print underscores instead spaces and an exclamation instead a
newline:

```python
print("Hello", "world", sep='_', end='!')
print(" No newline")
```

The result will be `Hello_World! No newline`.

This feature is called the
[keyword arguments](https://docs.python.org/3/glossary.html#term-argument).
There are also two arguments of a [python's print
function](https://docs.python.org/3/library/functions.html#print): `file`,
that specifies a specific output stream to write and `flush`, that tells to a
function to flush the stream's buffer after each write.

It looks naturally in Python, which is a lite dynamic language without a
strict typing. But in C++? Is it possible? I tell you: Yes, it is possible.
And I can do it using black magic of template metaprogramming.

With the uniprint, that means the Universal Printer, you can write the
following code:

```cpp
uniprint::print print(std::cout);
using namespace uniprint::args;

print("Hello", "world", sep{"_"}, end{"!"});
print(" No newline");
```

And it will produce the same result as the example of Python's print function.
See more in examples directory.

## Additional

There are two header files in include directory: `uniprint.hpp`, which is a
main header of a project, and `gfp.hpp`. "GFP" means Get From Pack. It is a
utility, that allows to extract an argument of the specific type from
the template parameter pack.

If an argument with the specific type appeared in the list of template arguments,
gfp will return its value. Also gfp does not ommit references and cv-qualifiers,
so if the argument is an lvalue, gfp will return the reference.

```cpp
const int value = 10;

using t = decltype(gfp::get_from_pack<int>{}('a', "hello", value));

assert((std::is_same<const int&, t>::value));
```

In case if argument is not appeared, gfp will return an object of
`gfp::none_type`. Gfp provides a way to easily handle this case:
`gfp::call_match` template function, that takes a value and an arbitrary number
of callable objects (for each possible type of a value) and tries to call each
callable with this value.

You can see the usage of `gfp::call_match` in `uniprint.hpp` header:

```cpp
auto sep = gfp::get_from_pack<args::sep> {}(std::forward<Types>(args)...);

gfp::call_match(
  sep,
  [&print_args](args::sep a) {
    print_args.sep = a.get();
  },
  [](gfp::none_type) {});
```

Here `sep` can be of type either the `args::sep` or `gfp::none_type`, and there
are a lambda for each type.

The key concept of "Get From Pack" itself is a recursive iteration through
the template parameter pack. It takes arbitrary arguments of some types,
and checks, if the type of a current argument is the desired type. If so,
it returns its value. Otherwise, it recursively finds the desired type in the
tail of a pack.
