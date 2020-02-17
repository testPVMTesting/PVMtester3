# `NOTE`

In order to visualize the music, we need to extract the notes.
Right now, the notes are encoded as a series of note-on note-off events.
The instrument is determined by the channel and the program-change event.
We now wish to bundle all note-related information into a single data structure.
It would contain:

* The note number
* The time at which the note starts playing
* The duration of the note
* The velocity (0-127)
* The instrument

Define a `NOTE` structure with the required fields. Make use of the correct
types for each. Since we will not be reading `NOTE` objects
directly from file, we don't need to worry about memory alignment details.
Define a constructor that can be used to initialize all fields.

Also define `==` and `!=` operators as well as a `<<` operator
which turns a note in the following string:

```text
Note(number=N,start=N,duration=N,instrument=N)
```
