trace - portable exception backtracing in C++17
====================================================================

[![Build Status](https://api.travis-ci.com/GPMueller/trace.svg?branch=master)](https://travis-ci.com/github/GPMueller/trace)

This library uses
[`std::nested_exception`](http://en.cppreference.com/w/cpp/error/nested_exception)
and
[`std::throw_with_nested`](http://en.cppreference.com/w/cpp/error/throw_with_nested)
and can be applied in order to not lose information while propagating an original
[`std::exception`](https://en.cppreference.com/w/cpp/error/exception)
upwards through a chain of function calls and create a **backtrace** without any
overhead (compare e.g. logging of debug messages). This avoids much of the need for any
debugging and provides a way of ensuring that a library does not crash ungracefully.

Usage
--------------------------------------------------------------------

```C++
#include <trace/trace.hpp>
```

The library revolves around three macros
- `trace::initiate`: can be placed anywhere. Takes a `std::string message` and initiates
  a trace with information about the corresponding function and source file. This is done
  by throwing an exception.
- `trace::propagate`: should be placed in a catch-block. Takes a `std::string message`,
  attaches it to the trace and propagates the trace up the function call stack by
  rethrowing a nested exception
- `trace::handle`: should be placed in a catch-block. Takes a `std::exception &` and
  unwraps it to creates a backtrace

and the function `trace::latest()`, which returns the most recent backtrace, created by
the call to `handle`.