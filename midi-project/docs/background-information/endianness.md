# Endianness

The decimal notation of the number twelve is `12`:
the digits are ordered from most to least significant.
We could as well reverse the order and write `21`, only
that's not the choice that history made for us.

In CPU-speak, our writing style is called *big-endian*:
the digit with the biggest weight comes first.
The opposite order is called *little-endian*.

While big-endian notation has the advantage of
being more familiar to us humans, little-endian
has some performance advantages (well, on old computers anyway, not sure
if it makes much of a difference today.)
For this reason Intel chose little endianness for its CPUs.
However, MIDI files are big-endian, meaning
that every time we read a value from a MIDI file,
we need to reverse the digit order.

Since endianness involves reverses digits, this raises
the question "What constitutes a digit?"

Endianness works on the level of *bytes*, i.e., a digit
corresponds to a single byte. Consider the 16-bit number `0x1234`,
which in big-endian order would be stored as `0x12 0x34`.
To determine the little-endian order, we reverse
the *bytes*, resulting in `0x34 0x12`.
