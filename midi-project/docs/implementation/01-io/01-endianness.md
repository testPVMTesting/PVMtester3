# `switch_endianness`

Define the following three functions in the `io` namespace:

```c++
void switch_endianness(uint16_t* n);
void switch_endianness(uint32_t* n);
void switch_endianness(uint64_t* n);
```

These functions must reverse the order of the bytes in the given integer.

As always, put the declarations in a `.h` file and the corresponding
definitions in a `.cpp` file. Note that the tests expect
you to name these files a certain way, so go take a look at them
in order to determine what filenames you should use.

## Possible Approaches

There are multiple ways to implement this functionality.

### Bit Manipulation

One way to shift bytes around is to rely on bit manipulation operators:

* `n << k` shift the bits in `n` by `k` positions. For example,
`0b101 << 1` equals `0b1010`, i.e., a `0` has been added to the end.
Put another way, shifting `k` to the left also corresponds to multiplying by 2<sup>k</sup>.
* `n >> k` shift `n`'s bits `k` positions to the right. For example,
`0b101 >> 1` yields `0b10`, i.e., the last digit is dropped. `n >> k` can
also be seen as dividing by 2<sup>k</sup>.
* `a & b` performs a *bitwise* AND operation, which you can compare to taking the intersection of bits. For example, `0b0011 & 0b1010`
yields `0b0010`: the `k`-th bit of the result is `1` if the `k`-th bits of the operands
are both `1`. This operator is often used for extracting bits: say you are only interested in the lower 8 bits of a 32-bit integer `n`, you can cut off the 24 higher ones with
`n & 0b1111'1111` or, written in hexadecimal, `n & 0x00'00'00'FF`.
* `a | b` performs a *bitwise* OR operation, which is similar to taking a union.
For example, `0b0011 | 0b1010` produces `0b1011`. This is often used to merge bytes together:
say you have bytes `a` and `b` and wish to construct the value `ab`, you first shift `a` 8 positions to the left
after which you combine them using OR: `(a << 8) | b`. If `a == 0x12` and `b = 0x34`, `(a << 8)` equals `0x1200` and `(a << 8) | b` is `0x1234`.

To use these operators to perform the byte reversion, you will need to first extract all bytes using `&`,
then shift them around using `<<` and `>>`, to end by appending the moved bytes using `|`.

### Reinterpreting

A value can be viewed as an array of bytes using `reinterpret_cast<uint8_t*>`.
You then only need to write code that reverses the elements of an array.