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
