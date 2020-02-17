# Variable Length Integers

What if you need to be able to encode large integers but at the same
time have to frugal as possible when it comes to storage needs.

* If you use single byte numbers, you cannot represent numbers larger than 255.
* If you use four byte numbers for each number, you can be pretty sure you'll be able to represent all values you need in a MIDI file, but it is rather wasteful.

To satisfy both range and storage demands, MIDI relies on variable length integers (VLIs):
an arbitrary number of bytes can be used to encode an integer value. When reading
in a variable length integer, you somehow need to detect how many bytes it will use.
This is achieved using the high bit of each byte: a `1` bit indicates
that the next byte is to be considered part of the VLI, whereas a `0`
indicates the end of the number. This means that each byte contains
7 bits of 'number data' and 1 bit of 'continue-or-stop' data.

For example, consider the value `18` or `10010‬` in binary.
This comfortably fits into a single byte. Encoded as a VLI, this would become

```text
    00010010
    ^
```

Let's consider a larger number: `1234` which in binary form
equals `‭10011010010‬`, i.e., we need 11 bits and have to spread it out over two bytes:

```text
     |-----|  |------|
    ‭10001001  01010010‬
    ^          ^
```

The actual 'value' bits are indicated by `|-----|`.
The high bit of the first byte (leftmost `^`)
is `1`, which means that the next byte is also to be considered
part of the VLI. The second byte's high bit is `0`,
indicating that this is the last byte.

As a final example, let's take `100100100100100`. This value requires 15 bit.
While this fits in two bytes, keep in mind that each VLI byte can only
contain 7 bits of actual data. In other words, we actually need three bytes to encode
this value. We chop it up in 7-bit pieces, starting from the right:

```text
    1 0010010 0100100
```

The leftmost 7-bit piece could use some more `0`'s:

```text
    0000001 0010010 0100100
```

Next, we add the VLI bits:

```text
    10000001 10010010 00100100
    ^        ^        ^
```

Notice how the high bits of the first two bytes are `1`, while the final byte's high bit is `0`.
