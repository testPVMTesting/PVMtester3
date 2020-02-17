# `read_notes`

This step is the culmination of all previous ones:
you'll read an entire MIDI file and return a list
of all notes contained within in.

Write the function `read_notes` that given
an `istream` containing an entire MIDI file,
parses the data and returns an `std::vector<NOTE>`.
It starts by reading an MThd chunk, from
which it learns how many MTrks there are.
Next, it proceeds to read all these MTrks chunks
and collects all notes in a vector. This
vector is ultimately returned.
