# Showing Channels

We'd also like to be able to print out `Channel` values:

```cpp
std::cout << Channel(10); // should print out 10
```

We could of course manually define an `operator <<` function,
but we'd like to go for a more reusable solution,
because we get the feeling we'll have to do this
repeatedly in the near future.

Therefore, just like `equality<T>` and `ordered<T>`
provide `==`, `!=`, `<`, etc. there is
also a `show_value<T>` type that automatically defines `<<`.

By default, `show_value<T>` defines `out << x` as simply `out << value(x)`, i.e.,
it prints the value of `x`.
However, since `Channel`s are just glorified `uint8_t`s (which are really `char`s)
outputting a `Channel` would not show its number, but its corresponding character
in the ASCII table. For example,
printing `Channel(10)` would not result in `10` but a line feed.

In order to force a `uint8_t` to be printed out as a decimal number,
we'll have to upgrade it to an `int` first: instead of
`out << value(x)`, we need `out << int(value(x))`.
For this reason, `show_value` has a second type parameter
which allows you to specify which type the value should be converted to
prior to printing. For example, `show_value<Channel, int>`
will convert `value(channel)` to an `int` before sending it off
to the input stream.

So, add an extra base class `show_value` to `Channel` so
that printing `Channel(4)` gives us `4`.

## You broke it!

If you're using MSVC++ (which you probably are),
adding this extra supertype to `Channel` will break a previously succeeding test,
more specifically, the `sizeof(Channel) == 1` check.

Fix this by updating the `Channel` declaration as follows:

```cpp
 struct __declspec(empty_bases) Channel : ...
```

### Explanation

This part is meant for those curious enough about how I spent
an entire afternoon finding the reason why a `Channel` suddenly
grew a byte when adding an extra empty base type. Others can go straight
for the next task.

To understand why adding zero fields to a class through inheritance
still increases the size, we must first answer the question
"How large is an empty class?"

```cpp
struct Empty { };

std::cout << sizeof(Empty) << std::endl;
```

While you might expect `0`, the correct answer is `1`.
Bjarne [knows why](http://www.stroustrup.com/bs_faq2.html#sizeof-empty):
to ensure different objects have different addresses.

So, say we have multiple such empty types (which we actually do:
`equality`, `ordered` and `show_value` have no fields and are therefore empty):

```cpp
struct Empty1 { };
struct Empty2 { };
struct Empty3 { };
struct Empty4 { };
```

What happens if we derive a class from all four?

```cpp
struct Sub : Empty1, Empty2, Empty3, Empty4 { };
```

MSVC++ performs a simple addition: the size of a class
is the sum of the sizes of its superclasses + its own fields.
This gives us `1 + 1 + 1 + 1 == 4`. So, there we go, we
have an empty type `Sub` which actually consumes 4 bytes.
I guess it's still better than Java, which consumes 16 bytes for an empty object.

Aware of this wastefulness, ~~people starting marching in the streets every Thursday~~
an optimization was developed, called [Empty Base Class Optimization](https://en.cppreference.com/w/cpp/language/ebo) (EBCO): inheriting from some empty type `T`
makes that `T` shrink back to 0. Using this optimization, `sizeof(Sub)` would
yield 1: 0 in total from its base classes, but in the end the rule
that every type must be at least 1 byte large still counts.

GCC and clang apply this optimization by default. MSVC++ does not,
presumably to make sure not to break any existing code. Fortunately,
you can give explicit permission to apply the EBCO using `__declspec(empty_bases)`.

That's C++ for you.