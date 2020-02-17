# `read_array`

Just like `read` is a more user-friendly version of `read_to` for reading a single value,
we'd like an easy-to-use version of `read_to` for multiple values, returning
them as an array.

How do we best return this array of `T`s?
Returning a `T[]` will not compile: C++ forbids this. Also, if this were allowed,
it would mean returning the array by value, i.e., copying it. Since
we don't know how large this array will be, we'd rather look for
more efficient solutions.

Returning a `T*` is allowed by C++. This raises the question: where is the memory `T*`
points to allocated?

* **Static allocation**: simply not an option as it is way too restrictive.
* **Stack allocation**: bad idea for two reasons.
  * The stack frame where the array resides would be freed when `read_array` returns.
  * C++ does not allow stack allocating an array of a size unknown at compile time.
* **Heap allocation**: the only remaining option.

Returning a `T*` is risky: heap allocation means we also need to manually free it.
This is something we must endeavor to avoid. Instead, we should
use a 'self-cleaning' pointer: either `shared_ptr` or `unique_ptr`.

`shared_ptr` would make sense if the pointer is actually shared, i.e.,
that both `read_array` and the caller keep a pointer to the array.
However, `read_array` is not interested in storing this pointer;
instead, it happily gives it away.

This leaves us with `unique_ptr`. Remember to make use of `std::make_unique`
to heap-allocate your block memory and give ownership to the `unique_ptr`.

Write a function `read_array<T>(in, n)` that reads `n` values of type `T` from the input stream `in` and returns them as a `std::unique_ptr<T[]>`.
