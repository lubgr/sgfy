
# sgfy: Mix printf with ostream operators

This single header file with less than 200 lines provides a convenient way to mix old school,
type-unsafe `std::printf` formatting with `std::ostream` operators. This is achieved by variadic
template arguments and one additional format specifier %S, yielding a convenient shortcut to turn
primitive types and custom objects into a string. For the sake of efficiency, argument pack
expansion is favored over recursion. For true formating libraries or type-safe printf
implementations, see [here](https://github.com/r-lyeh/fmt11#alternatives) for a collection of links.

Installation is as easy as storing `sgfy.h` somewhere such that the preprocessor finds it. Example
usage:
```C++
#include "sgfy.h"

sgfy::str("%d custom objects %S, %S - and some double: %.2f", 2, myObject, myObject, -1.234);
```
where `myObject` must have a overloaded `std::ostream` << operator. Empty strings or strings without
format specifier finally work as expected. Further use cases are demonstrated in `tests.cpp`.

To compile code using the header, C++11 must be supported. You might want to move the definition of
the first function `getFmtMatches` into a separate file, which heavily speeds up compilation when
multiple files are involved. To compile tests, [CppUTest](https://github.com/cpputest/cpputest) is
additionally required.
