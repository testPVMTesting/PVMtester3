# MIDI

Like PNG or JPEG files contain image data, the MIDI format is used to encode music.
Other audio formats are WAV and MP3, but MIDI differs from these in a fundamental way.

WAVs and MP3s encode digital approximations of actual sound waves. They can encode
all forms of sound arbitrarily accurately: music of any instruments, voices, etc.
A song typically requires a few megabytes of storage room.

While a couple of megabytes is no big deal today, it certainly was in the past.
One 3 1/2" floppy disk could contain 1.44MB of data, so a single piece
of music would already require multiple disks.

MIDI files have a different approach to storing audio: instead of encoding
the actual sound wave, a more abstract representation is used: musical notes.
In essence, a MIDI file contains such information as
"a piano plays [E Eb E Eb B D C A](https://www.youtube.com/watch?v=pUhBiT4Uiwc)".
This description can clearly be encoded using very few bytes (easily less than 100)
while it would take around 24,000 bytes in MP3 (assuming a bitrate of 192kbps and
a one second duration).

However, MIDI files have several disadvantages:

* There are many ways a musician can play a note. MIDI can not encode these nuances which gives it a [very mechanical feel](https://www.youtube.com/watch?v=WvWuco54FHg).
* A lyrics singing voice cannot be represented as notes. Maybe you can fix this using Stephen Hawking's voice with some autotune, but [somehow I doubt the results will be convincing](https://youtu.be/DvLPmmrofEg).

## MIDI File Format Overview

MIDI files are binary, unlike JSON or HTML, which are text formats. Windows makes a
distinction between the two, so it will be important that you remember to specify
to open the file in binary mode.

A MIDI file is divided into "sections" called *chunks*.
The first chunk is the *MThd chunk* which contains a description of the further contents of the MIDI file.
Next comes a series of *MTrk chunks* which represents tracks. The division of the music
into tracks will not matter to us: we will interpret them simply as "lists of notes"
which we will merge together into one single large list of notes. Note that tracks do **not** correspond to channels
or instruments.

<center>

| File Layout|
|:-------:|
| MThd |
| MTrk |
| MTrk |
| MTrk |
| MTrk |

</center>

## Chunks

Each chunk starts with a *chunk header*, which tells you which kind of chunk it is (`MThd` or `MTrk`) and how
large it is. As far as we know, the type of the chunk is redundant: the MIDI file should always contain a single `MThd` chunk followed by one or more `MTrk`s. Maybe the designers added the type to the header in anticipation
of other kinds of chunks. We will be using this chunk type information solely as a sanity check.

The chunk header has the following structure:

<center>

| Field name | Size | Description
|:-:|-|-|
| id | 4 bytes | Type of the chunk. Either `MThd` or `MTrk` |
| size | 4 bytes | Length |

</center>

The length does not include the header itself! Following this 8-byte header are `size` bytes that
comprise the chunk data.

## MThd

As mentioned earlier, the `MThd` chunk always comes first in a MIDI file.
It has the following structure:

<center>

| Field name | Size | Description
|:-:|-|-|
| header | 8 bytes | Chunk header (described above) |
| type | 2 bytes | Type of MIDI file |
| ntracks | 2 bytes | Number of tracks |
| division | 2 bytes | Number of units per beat |

</center>

The `MThd` chunk always has the same size: the header counts 8 bytes (all chunks have this in common)
plus 6 bytes of extra data. The `header.size` will therefore always contain the value 6 (remember that the header
is not included in this count.)

The `type` field is either `0`, `1` or `2`.

* `0` means the file contains a single track. The `ntracks` field should agree with this and contain `1`.
* `1` means the file contains multiple tracks which together form a single piece of music. The actual number of tracks is encoded in `ntracks`.
* `2` means the file contains multiple tracks, each of which contains a separate piece of music. We will not support this type of file.

In practice, we can simply ignore `type` as `ntracks` contains all necessary information anyway.

The `division` field determines how to interpret time values. To deal with this correctly is a bit fastidious,
so we'll simply ignore it and leave it to next year's students.

## MTrk

The `MTrk` chunks are more relevant to the task at hand: here we will find the actual music data.
A `MTrk` chunk starts with a chunk header (with `id` equal to `MTrk`) followed
by a list of *events*.

There are categories types of events: meta events, system exclusive events and MIDI events.
Within each category there are different subtypes of events:

<center>

| Category | Subtype |
|----------|------|
| Meta Event | Sequence Number |
| | Text Event |
| | Copyright Notice |
| | Lyric Text |
| | Marker Text |
| | End of Track |
| | ... |
| System Exclusive Event | *No subdivision* |
| MIDI Event | Note Off |
| | Note On |
| | Polyphonic Key Pressure |
| | Control Change |
| | Program Change |
| | Channel Pressure |
| | Pitch Wheel Change |

</center>

Only a couple of these events are relevant to the task at hand.
However, we need to be able to process all these events since we might
encounter them in MIDI files. We are interested in the following events:

<center>

| Event | Description |
|-------|-------------|
| Note On | Start playing a note |
| Note Off | Stop playing a note |
| Program Change | Initialize or change instrument |
| End of Track | End of Track (duh)

</center>

Each event occurs at a certain point in time and appear in chronological order.
For example, Für Elise would look as follows:

<center>

| Time | Event |
|:----:|:-----|
| 0 | Program Change to piano |
| 0   | Note On E |
| 100 | Note Off E |
| 100 | Note On Eb |
| 200 | Note Off Eb |
| 200 | Note On E |
| 300 | Note Of E |
| 300 | Note On Eb |
| 400 | Note Off Eb |
| 400 | Note On E |
| 500 | Note Off E |
| 500 | Note On B |
| 600 | Note Off B |
| 600 | Note On D |
| 700 | Note Off D |
| 700 | Note On C |
| 800 | Note Off C |
| 800 | Note On A |
| 1000 | Note Off A |

</center>

MIDI does not give events absolute timestamps, however. Instead,
it only provides *delta times*. These represent the amount of time
since the last event.

<center>

| Time | Delta Time | Event |
|:----:|:-----:|:-----|
| 0 | 0 | program change to piano |
| 0 | 0 | note on E |
| 100 | 100 | note off E |
| 100 | 0 | note on Eb |
| 200 | 100 | note off Eb |
| 200 | 0 | note on E |
| 300 | 100 | note off E |
| 300 | 0 | note on Eb |
| 400 | 100 | note off Eb |
| 400 | 0 | note on E |
| 500 | 100 | note off E |
| 500 | 0 | note on B |
| 600 | 100 | note off B |
| 600 | 0 | note on D |
| 700 | 100 | note off D |
| 700 | 0 | note on C |
| 800 | 100 | note off C |
| 800 | 0 | note on A |
| 1000 | 200 | note off A |

</center>

So, a `0` delta time means that events occur simultaneously. This occurs
quite frequently, as when one note starts, the previous one stops.
Note that all delta times are encoded as variable length integers.

Events all exhibit the same general structure:

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `identifier` | 1 | Byte identifying type of event |
| extra | | Extra data that depends on the type of event |

</center>

Every event has a unique identifier which determines want
extra data you can expect. In case of a note on event,
the extra data consists of the note and the velocity (explained later.)

In practice, to read the event, you will first read the delta time, then
the identifier byte. Based on this byte you will know what kind of
data will follow.

We now discuss each type of event in detail.

### Meta Events

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `identifier` | 1 | `0xFF` |
| `type` | 1 | Identifies the type of meta event |
| `length` | Variable length integer | Number of data bytes |
| `data` | `length` bytes | Data |

</center>

Meta events can be used to add, well, metadata. Examples are a copyright notice,
lyrics, etc. Each is identified by a unique number (e.g., `0x02` for copyright,
`0x05` for lyrics) which is stored in the `type` field.

We care only about one specific type of meta event: the end of track event.
You can recognize it using its type number: `0x2F`. It normally has no data associated with it.
Concretely, the end of track event is encoded by the following four bytes:

<center>

| Byte | Meaning |
|------|---------|
| `0x00` | Delta time |
| `0xFF` | Signals meta event |
| `0x2F` | Signals end of track |
| `0x00` | Signals zero bytes of data |

</center>

### System Exclusive Event

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `identifier` | 1 | `0xF0` or `0xF7` |
| `length` | Variable length integer | Number of data bytes |
| `data` | `length` bytes | Data |

</center>

### MIDI Events

As mentioned above, there are seven types of MIDI events:

<center>

| Type |
|:---------:|
| Note Off |
| Note On |
| Polyphonic Key Pressure |
| Control Change |
| Program Change |
| Channel Pressure |
| Pitch Wheel Change |

</center>

MIDI events always take place on a specific channel, of which there are 16.
MIDI events on different channels are independent: for example,
a program change (= change of musical instrument) on channel 5
only affects the notes played on channel 5.

The type and channel of a MIDI event are combined into a single byte and form
the MIDI event's identifier byte. This same byte is also called the *status byte*.
The upper 4 bits denote the type of MIDI event, the lower 4 bits denote the channel on which it takes place.
The general structure of a MIDI event can then be written

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xkn` where `k` denotes the type and `n` denotes channel (0-15) |
| type-specific | | Type specific extra data

</center>

We now discuss each MIDI event type.

#### Note Off

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0x8n` where `n` denotes channel (0-15) |
| `note` | 1 | Note that stops playing (0-127) |
| `velocity` | 1 | After touch (0-127) |

</center>

Notes are index from `0` to `127`. `0` corresponds to a very low A (8.18Hz) which
is so low we probably can't even hear it (human hearing starts at around 20Hz.) `127`
is a G#/Ab at 13,289Hz (that we can hear, but it may not be particularly pleasant.)
Musicians might want to know that A (440Hz) has index 69, but for our purposes
what matters is that a higher index corresponds to a higher note.

The velocity doesn't seem to have much effect according to my experiments.
According to the documentation, it is used for after touch, but I don't hear any difference.

#### Note On

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0x9n` where `n` denotes channel (0-15) |
| `note` | 1 | Note that starts playing (0-127) |
| `velocity` | 1 | Loudness (0-127) |

</center>

#### Polyphonic Key Pressure

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xAn` where `n` denotes channel (0-15) |
| `note` | 1 | Note that starts playing (0-127) |
| `pressure` | 1 | Pressure value (0-127) |

</center>

#### Control Change

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xBn` where `n` denotes channel (0-15) |
| `controller` | 1 | Controller number (0-127) |
| `value` | 1 | New value (0-127) |

</center>

#### Program Change

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xCn` where `n` denotes channel (0-15) |
| `program` | 1 | Program number (0-127) |

</center>

This corresponds to a change in musical instrument on the current channel.

#### Channel Pressure

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xDn` where `n` denotes channel (0-15) |
| `pressure` | 1 | Pressure value (0-127) |

</center>

#### Pitch Wheel Change

<center>

| Field name | Size | Description
|:-|-|-|
| `delta_time` | Variable length integer | Time since last event |
| `status` | 1 | `0xEn` where `n` denotes channel (0-15) |
| `lower_bits` | 1 | Lower 7 bits |
| `upper_bits` | 1 | Upper 7 bits |

</center>

`lower_bits` and `upper_bits` together form a 14 bit value denoteing
the pitch wheel's position.

## Example

Let's see what Für Elise would look like. The notes are

<center>

| Note | Note Number | Duration |
|:----:|:-----------:|:--------:|
| E | 76 | 100 |
| Eb | 75 | 100 |
| E | 76 | 100 |
| Eb | 75 | 100 |
| E | 76 | 100 |
| B | 71 | 100 |
| D | 74 | 100 |
| C | 72 | 100 |
| A | 69 | 200 |

</center>

In MIDI events form:

<center>

| Delta Time | Event Type | Note Number | Velocity | Bytes |
|:----:|:-----------:|:--------:|:---:|---|
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note Off | 76 | 0 | `0x64 0x80 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x90 0x4B 0x1E` |
| 100 | Note Off | 75 | 0 | `0x64 0x80 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note Off | 76 | 0 | `0x64 0x80 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x90 0x4B 0x1E` |
| 100 | Note Off | 75 | 0 | `0x64 0x80 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note Off | 76 | 0 | `0x64 0x80 0x4C 0x00` |
| 0 | Note On | 71 | 30 | `0x00 0x90 0x47 0x1E` |
| 100 | Note Off | 71 | 0 | `0x64 0x80 0x47 0x00` |
| 0 | Note On | 74 | 30 | `0x00 0x90 0x4A 0x1E` |
| 100 | Note Off | 74 | 0 | `0x64 0x80 0x4A 0x00` |
| 0 | Note On | 72 | 30 | `0x00 0x90 0x48 0x1E` |
| 100 | Note Off | 72 | 0 | `0x64 0x80 0x48 0x00` |
| 0 | Note On | 69 | 30 | `0x00 0x90 0x45 0x1E` |
| 200 | Note Off | 69 | 0 | `0x64 0x80 0x45 0x00` |

</center>

All events take place on channel `0`, which has been randomly chosen.

The value for note on velocity has been chosen rather arbitrarily: Für Elise is supposed
to be played very softly (pianissimo), so 30 is about one fourth of the maximum volume (127).
The note off velocity equals 0 because, as mentioned earlier, it doesn't seem to have any effect.

## Note On 0

Often, note off events are replaced by note on events with velocity set to 0.
Together with running status (see later), this leads to more compact MIDI files (up to 25% less space
requirements.)

Using this trick, we get

<center>

| Delta Time | Event Type | Note Number | Velocity | Bytes |
|:----:|:-----------:|:--------:|:---:|---|
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x90 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x90 0x4B 0x1E` |
| 100 | Note On | 75 | 0 | `0x64 0x90 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x90 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x90 0x4B 0x1E` |
| 100 | Note On | 75 | 0 | `0x64 0x90 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x90 0x4C 0x00` |
| 0 | Note On | 71 | 30 | `0x00 0x90 0x47 0x1E` |
| 100 | Note On | 71 | 0 | `0x64 0x90 0x47 0x00` |
| 0 | Note On | 74 | 30 | `0x00 0x90 0x4A 0x1E` |
| 100 | Note On | 74 | 0 | `0x64 0x90 0x4A 0x00` |
| 0 | Note On | 72 | 30 | `0x00 0x90 0x48 0x1E` |
| 100 | Note On | 72 | 0 | `0x64 0x90 0x48 0x00` |
| 0 | Note On | 69 | 30 | `0x00 0x90 0x45 0x1E` |
| 200 | Note On | 69 | 0 | `0x64 0x90 0x45 0x00` |

</center>

## Running Status

If a MIDI event has the same status byte as the previous event,
its status byte can be omitted.
This does not cause problems reading in the events:
all status bytes have their most significant
bit set to `1` while all non-status-bytes have
`0` as most significant bit.

Für Elise can therefore be encoded as follows:

<center>

| Delta Time | Event Type | Note Number | Velocity | Bytes |
|:----:|:-----------:|:--------:|:---:|---|
| 0 | Note On | 76 | 30 | `0x00 0x90 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x4B 0x1E` |
| 100 | Note On | 75 | 0 | `0x64 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x4C 0x00` |
| 0 | Note On | 75 | 30 | `0x00 0x4B 0x1E` |
| 100 | Note On | 75 | 0 | `0x64 0x4B 0x00` |
| 0 | Note On | 76 | 30 | `0x00 0x4C 0x1E` |
| 100 | Note On | 76 | 0 | `0x64 0x4C 0x00` |
| 0 | Note On | 71 | 30 | `0x00 0x47 0x1E` |
| 100 | Note On | 71 | 0 | `0x64 0x47 0x00` |
| 0 | Note On | 74 | 30 | `0x00 0x4A 0x1E` |
| 100 | Note On | 74 | 0 | `0x64 0x4A 0x00` |
| 0 | Note On | 72 | 30 | `0x00 0x48 0x1E` |
| 100 | Note On | 72 | 0 | `0x64 0x48 0x00` |
| 0 | Note On | 69 | 30 | `0x00 0x45 0x1E` |
| 200 | Note On | 69 | 0 | `0x64 0x45 0x00` |

</center>

Thanks to running status, we only need 55 bytes instead of 72.
[What a savings!](https://www.youtube.com/watch?v=kgv7U3GYlDY)

## Für Elise

Für Elise in its entirety:

<center>

| Bytes | Description |
|:----|:-----------|
| `0x4D 0x54 0x68 0x64` | `MThd` indicating start of MThd chunk |
| `0x00 0x00 0x00 0x06` | Chunk header size field: rest of chunk counts 6 bytes |
| `0x00 0x01` | MIDI type 1 |
| `0x00 0x01` | Number of tracks |
| `0x02 0x00` | Division 512 (picked randomly) |
| `0x4D 0x54 0x72 0x6B` | `MTrk` indicating start of MTrk chunk |
| `0x00 0x00 0x00 0x3D` | Chunk header size field: rest of chunk counts 61 bytes |
| `0x00 0xC0 0x00` | Program change to program #0 (= piano) |
| `0x00 0x90 0x4C 0x1E` | Notes |
| `0x64 0x4C 0x00` |
| `0x00 0x4B 0x1E` |
| `0x64 0x4B 0x00` |
| `0x00 0x4C 0x1E` |
| `0x64 0x4C 0x00` |
| `0x00 0x4B 0x1E` |
| `0x64 0x4B 0x00` |
| `0x00 0x4C 0x1E` |
| `0x64 0x4C 0x00` |
| `0x00 0x47 0x1E` |
| `0x64 0x47 0x00` |
| `0x00 0x4A 0x1E` |
| `0x64 0x4A 0x00` |
| `0x00 0x48 0x1E` |
| `0x64 0x48 0x00` |
| `0x00 0x45 0x1E` |
| `0x64 0x45 0x00` |
| `0xFF 0x2F 0x00` | Meta event: end of track |
</center>

## Further Information

* http://valentin.dasdeck.com/midi/midifile.htm
* http://www.music.mcgill.ca/~ich/classes/mumt306/StandardMIDIfileformat.html
