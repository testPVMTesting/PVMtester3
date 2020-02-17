# `read_to`

Write a template function `read_to<T>` that receives

* a input stream `in`, type `std::istream`;
* a pointer `buffer` of type `T*`;
* a value `n` of type `size_t` (which is basically an unsigned integer). Its default value is `1`.

`read_to<T>(in, buffer, n)` reads in `n` `T` values from the `in`.
It does this by reading raw bytes from the `istream` and
reinterpreting these bytes as a `n` consecutive `T`s.
For example,

```c++
std::ifstream in(filename);
int data;

read_to(in, &data);
```

reads 4 bytes (assuming `int` is 4 bytes long) and reinterprets these bytes as a single `int`.

`read_to` should check whether it succeeded. Rely on the `istream`'s `fail` method for this.
If the read operation failed, simply crash the application.
For this, you can make use of the `CHECK` macro:

```c++
#include "logging.h"

void foo()
{
    CHECK(condition) << error_message;
}
```

If `condition` evaluates to `false`, `error_message` is printed and the application
is aborted. Hardly elegant, but good enough for our purposes.