# Memory Alignment

Reading and writing memory does not happen on a byte-per-byte basis. Instead, they are read or written in small
chunks called *memory words*. These words are generally 4 bytes long on 32 bit systems
and 8 bytes long on 64 bit systems. This texts assumes a 4 byte word size.

<center>

```text
+---------------+---------------+---------------+---------------+
|     WORD 0    |     WORD 1    |     WORD 2    |     WORD 3    |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```

</center>

Say you need to read in a 32-bit integer `x` from RAM.
Assume its address `&x` is 8.

<center>

```text
+---------------+---------------+---------------+---------------+
|     WORD 0    |     WORD 1    |     WORD 2    |     WORD 3    |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|                               |       x       |               |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```

</center>

Reading this `x` is straightforward: word 2 is read in one go. Consider,
however, the case where `&x` is 6.

<center>

```text
+---------------+---------------+---------------+---------------+
|     WORD 0    |     WORD 1    |     WORD 2    |     WORD 3    |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | A | B | C | D | E | F |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
|                       |       x       |                       |
+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
```

</center>

In order to read `x`, two memory accesses are necessary:

* First word 1 is read.
* Next, word 2 is read.
* The redundant bits are cut off.
* The remaining bits are shifted around so as to form `x`.

In this case, reading `x` is twice as slow, if not more.
To avoid this unnecessary speed penalty, 4 byte integers
should have an address that is divisible by 4. This way,
we are certain they fit inside a single word and
a single RAM access will suffice.

These rule is applied by optimizing compilers in a more
generalized form:

* 2 byte values are stored on addresses divisible by 2.
* 4 byte values are stored on addresses divisible by 4.
* 8 byte values are stored on addresses divisible by 8.

When defining a struct, the compiler respects the order in which
you declared them, i.e., it will not move them around.
However, the compiler will by default inject
gaps between them so that each field has an address
divisible by its own size.

For example,

```cpp
struct Foo
{
    uint8_t  a;
    uint16_t b;
    uint8_t  c;
    uint32_t d;
};
```

Without padding, i.e., not trying to memory align the fields,
`sizeof(Foo)` would be 1+2+1+4 = 8.

<center>

```text
01234567
abbcdddd
```

</center>

With padding, however, the memory layout becomes

<center>

```text
0123456789AB
a.bbc...dddd
```

</center>

where `.` designate padding, unused bytes basically.
`sizeof(Foo)` is now 12. Giving up these 4 bytes should
make working with `Foo` objects more CPU-efficient.

## Arrays

Consider

```cpp
struct Bar
{
    uint32_t a;
    uint8_t  b;
};
```

You might think that, even with padding, `sizeof(Bar)` equals 5: each
field is perfectly memory-aligned. However, you'll find that
if you write code that outputs `sizeof(Bar)`, you'll get 8.

This is due to the fact that the compiler also takes into account
the possibility of having multiple `Bar`s in a row (i.e., an array.)
Take for example `Bar* p = new Bar[2]`:

<center>

```text
0123456789
aaaabaaaab
```

</center>

Now `&p[1].a` has address 5 while it has size 4.

To remedy this, the compiler adds further padding at the
end of the struct so as to make its size a multiple
of the size of the largest member. In `Bar`'s case,
`a` is largest with 4 bytes. Without padding, `sizeof(Bar) == 5`,
so an extra 3 bytes are required to make `sizeof(Bar)` a multiple of 4.
`Bar`'s memory layout thus becomes

<center>

```text
01234567
aaaab...
```

</center>

Every field is perfectly aligned even in the case of arrays:

<center>

```text
0123456789ABCDEF
aaaab...aaaab...
```

</center>

## Forcing Memory Alignment

Sometimes we need to prevent the compiler from padding structures.
This is done using the `#pragma` directive:

```cpp
#pragma pack(push, 1)

struct Foo
{
    uint8_t  a;
    uint16_t b;
    uint8_t  c;
    uint32_t d;
};

#pragma pack(pop)
```

The `1` indicates that we wish to align fields on multiples of 1,
meaning there should be no padding at all.