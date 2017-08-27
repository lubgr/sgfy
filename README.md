
# sgfy: Mix printf with ostream operators

This single header file with hardly more than 100 lines provides a convenient way to mix old school,
type-unsafe `std::printf` formatting with `std::ostream` operators. This is achieved by variadic
template arguments and one additional format specifier %S, yielding a convenient shortcut to turn
primitive types and custom objects into a string. For true formating libraries or type-safe printf
implementations, see [here](https://github.com/r-lyeh/fmt11#alternatives) for a collection of links.

Installation is as easy as storing `sgfy.h` somewhere such that the preprocessor finds it. Example
usage:
```C++
#include "sgfy.h"

sgfy::str("%d custom objects %S, %S - and some double: %.2f", 2, myObject, myObject, -1.234);
```
where `myObject` must have a globally overloaded `std::ostream` << operator. Multiple format strings
are possible, too:
```C++
sgfy::str("%d ", 2, "custom objects %S,", myObject, " %S - and some double: %.2f", 2, myObject, -1.234);
```
Empty strings or strings without format specifier finally work as expected. Further use cases are
demonstrated in `tests.cpp` To compile the above examples, C++11 must be supported (`g++-5`,
`g++-4.9` and `clang 4.0.1` were tested). To compile tests,
[CppUTest](https://github.com/cpputest/cpputest) is additionally required.
