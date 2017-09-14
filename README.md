
# sgfy: Mix printf with ostream operators

A single header and one compilation unit (together around 200 lines of code) provide a convenient
way to mix old school, type-unsafe `std::printf` formatting with `std::ostream` operators. This is
achieved by variadic template arguments and one additional format specifier '%S', yielding a
convenient shortcut to turn primitive types and custom objects into a string. For the sake of
efficiency, argument pack expansion is favored over recursion. For true formating libraries or
type-safe printf implementations, see [here](https://github.com/r-lyeh/fmt11#alternatives) for a
collection of links.

Installation is as easy as adding `sgfy.h` and `sgfy.cpp` to your project (make sure the latter is
compiled and not forgotten during linking). Example usage:
```C++
#include "sgfy.h"

sgfy::str("%d custom objects %S, %S - and some double: %.2f", 2, myObject, myObject, -1.234);
```
where `myObject` must have a overloaded `std::ostream` << operator. Any classical format specifier
listed [here](http://en.cppreference.com/w/cpp/io/c/fprintf) will work as well as empty strings or
strings without format specifier. Further use cases are demonstrated in `tests.cpp`.

For compilation, C++11 must be supported. The header file has been written such that `<regex>` is
included only in the implementation file, which heavily reduces compilation time when multiple file
use `sgfy.h`. To compile tests, [CppUTest](https://github.com/cpputest/cpputest) is additionally
required.
