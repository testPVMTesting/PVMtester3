# Note On Events

Add support for note on events to `read_mtrk`.

As you may remember from the MIDI background information,
note on events with velocity set to 0 are equivalent
to note off events. `read_mtrk` should **not** implement
this rule: this eventuality is dealt with elsewhere.
Have `read_mtrk` simply call `note_on` whenever
a note on event is encountered, regardless of the velocity.