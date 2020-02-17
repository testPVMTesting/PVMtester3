# Event Receiver

Read up on Inversion of Control to understand the purpose of this step.

Define an interface-like class (i.e. only unimplemented virtual member functions) `EventReceiver`
that has one member function per type of event. Each member function must be called after the type of event
and be able to receive all extra data through its parameters. A few examples:

* `void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)`
* `void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)`
* `void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)`
* `void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)`

Note that these member functions should not `const` since we want our receiver objects to
change state internally upon reception of event data.
