# Channels

Make sure to read the texts on [strong typedefs](../../../background-information/strong-typedefs.md)
and [midi](../../../background-information/midi.md) before starting this series of tasks.

`tagged`, `equality` and `ordered` have already been defined for you in `util/tagged.h`.

We start of this series by defining a new type for channels. MIDI supports
up to 16 different channels, so an `uint8_t` will suffice.
Each channel has a unique number `0`-`15` associated with it,
but beyond giving us the ability to differentiate between
channels (i.e., channel #4 is a different channel than #5),
this number has no meaning: channel #0 is not less or more
important than channel #15. We can swap these channels
without it having any effect on the music encoded in them.
In other words, we want `==` and `!=` to
be defined on channels, but `<`, `>`, `<=` and `>=` are of no use.

Define a new type `Channel` that only supports `==` and `!=`.
Make sure to rely on the types already defined in `util/tagged.h`.