
# sgfy: Mix printf with std::ostream operators [![Build Status](https://travis-ci.org/lubgr/sgfy.svg?branch=master)](https://travis-ci.org/lubgr/sgfy)

These ~200 lines of code (one header, one compilation unit, C++11 required) allow you to mix old
school, type-unsafe `std::printf` formatting with `std::ostream` operators to assemble a
`std::string` object through one single function `sgfy::str`. There is an additional format
specifier `'%S'` that triggers `std::ostream& operator <<` for the corresponding argument. Note that
this is a small playground for variadic templates and the `<regex>` header and not a serious
formatting tool, see [here](https://github.com/r-lyeh/fmt11#alternatives) for a collection of
wholehearted approaches. Example usage:
```C++
#include "sgfy.h"

sgfy::str("%d custom objects %S, %S - and some double: %.2f", 2, myObject, myObject, -1.234);
```
where `myObject` must have a overloaded `std::ostream` << operator. Any [classical format
specifier](http://en.cppreference.com/w/cpp/io/c/fprintf) will work as well as empty strings or
strings without format specifier. Further use cases are demonstrated in `tests.cpp` (to compile
tests, [CppUTest](https://github.com/cpputest/cpputest) is required).
