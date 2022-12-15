# uniprint

The experimental implementation of Python keyword arguments in C++ using 
templates and fold expressions.

As you know, the Python's print function has the following signature:
`print(arguments, sep=" ", end="\n", file=sys.out, flush=False)`, where 
the last four parameters are unnecessary, has default values, and they
are "named parameters", i.e you need to pass it by name: `sep=" "`. They
called the keyword parameters.

And, as you know, C++ in general does not allow to do such things. However,
you can do this, using fold expressions.

So, I show you, how can you implement this. First, we need to determine,
what is expected. In the code, I wanted to get the value of a separator,
if the separator is passed to a function, and a default separator value,
if not.

	const char* default_sep = " ";
	sep = get_sep(default_sep, args...);

In the code above, the first argument is a default value, and the second 
is a template parameter pack. It is a good idea to take a pack by reference.

	template <typename... Args>
	const char* get_sep(const char* default_value, Args const&... args);

Now we need to expand parameter pack, and, if there is the separator, 
return its value, otherwise return default value. 

	template <typename... Args>
	const char* get_sep(const char* default_value, Args const&... args) {
	    const char* result = default_value;
	    (get_sep_impl(&result, args), ...);
	    return result;
	}

There are two overloads of get_sep_impl. First overload takes a separator
and assigns its value to a target, second overload does nothing.

	void get_sep_impl(const char** target, separator const& sep) {
	    *target = &sep.get_value();
	}

	template <typename T>
	void get_sep_impl(const char**, T const&) {}

Well, that's all.
