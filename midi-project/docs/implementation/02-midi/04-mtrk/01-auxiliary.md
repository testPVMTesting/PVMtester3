# Auxiliary Functions

The goal of this series of tests is to implement `read_mtrk`,
a function that reads all events in a MTrk chunk. This is a
rather long function, so it might be best to implement it gradually.
We start off with a number of auxiliary functions.

## Event Type Identification

As described in the background information on MIDI,
there are different kinds of events:

* System exclusive events
* Meta events
* MIDI events

Each event starts with a variable length integer designated
the time elapsed since the previous event, followed
by a byte that identifies the event. In the case of
running status, the status byte can be missing. You need
to be able to recognize this situation.
Write the following functions:

* `bool is_sysex_event(uint8_t byte)`
* `bool is_meta_event(uint8_t byte)`
* `bool is_midi_event(uint8_t byte)`
* `bool is_running_status(uint8_t byte)`

## MIDI Events

A MIDI event's status byte can be split up in two nibbles, i.e., chunks of 4 bits.
The upper nibble identifies the type of MIDI event (note on, note off), the lower
nibble indicates on which channel the event takes place.

Write the functions

* `uint8_t extract_midi_event_type(uint8_t status)` returns the 4 upper bits of `status`.
* `Channel extract_midi_event_channel(uint8_t status)` return the 4 lower bits of `status`.

Next, write the following functions that, given the midi event type nibble,
check whether it identifies a MIDI event of a specific type.

* `bool is_note_off(uint8_t status)`
* `bool is_note_on(uint8_t status)`
* `bool is_polyphonic_key_pressure(uint8_t status)`
* `bool is_control_change(uint8_t status)`
* `bool is_program_change(uint8_t status)`
* `bool is_channel_pressure(uint8_t status)`
* `bool is_pitch_wheel_change(uint8_t status)`
