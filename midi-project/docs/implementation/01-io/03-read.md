# `read`

`read_to<T>` is a fine function, but it can be cumbersome to use.
If we want to read a simple `int`, we first need to introduce
a variable of this type and then pass a pointer to it to `read_to`:

```cpp
int value;
read_to(in, &value);
```

It would be much more pleasant to write

```cpp
int value = read<int>(in);
```

However, we only want to allow this for small `T`s, more specifically,
only the fundamental types (`bool`, `int`, `unsigned`, ...)
In essence, we want something like

```cpp
template<typename T where T is primitive>
T read(std::istream& in)
{
    // ...
}
```

C++ allows us to express such constraints, albeit using
a rather awkward syntax:

```cpp
template<typename T, typename std::enable_if<std::is_fundamental<T>::value, T>::type* = nullptr>
T read(std::istream& in)
{
    // ...
}
```

Complete the body of this function so that `read<T>(in)` reads a single `T` from `in` and
returns it.