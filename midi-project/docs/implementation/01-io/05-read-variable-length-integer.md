# `read_variable_length_integer`

Write a function `read_variable_length_integer(in)` that reads a variable length integer
from the input stream `in`. See the technical details documents to read about variable length integers.

Since you do not know beforehand how many bytes
the variable length integer will need, you will need to read one byte at a time.

You will need to find a way to determine whether the
most significant bit of a byte is set.

* You can either rely on bitwise operators. `&` is your friend.
* You can ask yourself the question what it means for a byte's value to have its most significant bit set.
