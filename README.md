trace -- portable exception backtracing
====================================================================

This library uses
[`std::nested_exception`](http://en.cppreference.com/w/cpp/error/nested_exception)
and
[`std::throw_with_nested`](http://en.cppreference.com/w/cpp/error/throw_with_nested)
and can be applied in order to not lose information while propagating an original
`std::exception` upwards through a chain of function calls and create a **backtrace**
without any overhead (compare e.g. logging of debug messages). This avoids much of the
need for any debugging and provides a way of ensuring that a library does not crash
ungracefully.