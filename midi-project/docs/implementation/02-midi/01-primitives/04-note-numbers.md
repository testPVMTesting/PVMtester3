# Note numbers

Next, we define a separate type for note numbers. In the MIDI world,
they can take on values ranging from `0` to `127`.

Contrary to channels and instruments, these values *can* be compared
using `<` and co: a lower number corresponds to a lower pitched note.

Introduce a new type `NoteNumber` whose values
can be compared using `==`, `!=`, `<`, `>`, `<=` and `>=`.
Also, don't forget about `<<` for printing.