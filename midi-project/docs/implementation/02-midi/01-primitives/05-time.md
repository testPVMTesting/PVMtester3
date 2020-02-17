# Time

In dealing with music, time is of the essence. Since
we'll have to perform certain time-related operations,
we will distinguish two separate time-related concepts:

* `Time`: represents an absolute moment in time.
* `Duration`: represents the difference between two `Time`s.

Making this distinction helps ensure we're performing meaningful
calculations. A `Time` is similar to a date when it comes to allowed operations.
Adding dates does not make sense: how much is 1/1/2000 + 1/1/2000?
Should it be 2/2/4000? Or perhaps 1/1/4000? Or 1/1/13800002000, if
we start counting from the birth of the universe?
What *does* make sense is to add a *duration* to a date:
1/1/2000 + 1 year equals 1/1/2001.

We'll allow the following operations:

<center>

|Left|Operator|Right| |Result|
|:-:|:-:|:-:|:-:|:-:|
|Duration|+|Duration|=|Duration
|Time|+|Duration|=|Time
|Duration|+|Time|=|Time
|Time|-|Time|=|Duration
|Duration|-|Duration|=|Duration

</center>

Added to this we also want

<center>

|Left|Operator|Right|
|:-:|:-:|:-:|
|Time|+=|Duration
|Duration|+=|Duration
|Duration|-=|Duration

</center>

You'll have to define each of these operators manually. There's so
predefined trait/mixin like `equality` or `ordered` available to you.

Apart from these `+` and `-` operators, we also want
`==`, `!=`, `<`, `>`, `<=`, `>=` and `<<`.