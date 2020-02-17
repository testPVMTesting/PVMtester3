#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "tests/tests-util.h"
#include "Catch.h"
#include <vector>
#include <sstream>


std::stringstream build_midi_file(const std::vector<char>& mtrk_bytes)
{
    std::vector<char> buffer;

    {
        uint32_t mtrk_size = (uint32_t)(mtrk_bytes.size() + 4); // Add end-of-track (4 bytes)
        const char* mtrk_size_bytes = reinterpret_cast<const char*>(&mtrk_size);

        const char bytes[] = {
            MTHD,
            0x00, 0x00, 0x00, 0x06, // MThd size
            0x00, 0x01, // Type
            0x00, 0x01, // Number of tracks
            0x01, 0x00, // Division
            MTRK,
            mtrk_size_bytes[3], mtrk_size_bytes[2], mtrk_size_bytes[1], mtrk_size_bytes[0],
        };

        buffer.insert(buffer.end(), bytes, bytes + sizeof(bytes));
    }

    buffer.insert(buffer.end(), mtrk_bytes.begin(), mtrk_bytes.end());

    {
        const char bytes[] = {
            END_OF_TRACK
        };

        buffer.insert(buffer.end(), bytes, bytes + sizeof(bytes));
    }

    std::string data(&buffer[0], buffer.size());
    
    return std::stringstream(data);
}


TEST_CASE("read_notes, zero tracks")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x00, // Number of tracks
        0x01, 0x00, // Division
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 0);
}

TEST_CASE("read_notes, no notes in track")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 4, // MTrk size
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 0);
}

TEST_CASE("read_notes, single note")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 12, // MTrk size
        0, NOTE_ON(0, 5, 127),
        100, NOTE_OFF(0, 5, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);
    
    CATCH_REQUIRE(notes.size() == 1);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 127, midi::Instrument(0)));
}

TEST_CASE("read_notes, single note with instrument 5")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 12, // MTrk size
        0, PROGRAM_CHANGE(0, 5),
        0, NOTE_ON(0, 5, 127),
        100, NOTE_OFF(0, 5, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 1);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 127, midi::Instrument(5)));
}

TEST_CASE("read_notes, two notes on same track and channel")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 20, // MTrk size
        0, NOTE_ON(0, 5, 120),
        100, NOTE_OFF(0, 5, 0),
        100, NOTE_ON(0, 8, 110),
        100, NOTE_OFF(0, 8, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 120, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(8), midi::Time(200), midi::Duration(100), 110, midi::Instrument(0)));
}

TEST_CASE("read_notes, two notes on same track and channel, instruments 1 and 2")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 20, // MTrk size
        0, PROGRAM_CHANGE(0, 1),
        0, NOTE_ON(0, 5, 120),
        100, NOTE_OFF(0, 5, 0),
        0, PROGRAM_CHANGE(0, 2),
        100, NOTE_ON(0, 8, 100),
        100, NOTE_ON(0, 8, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 120, midi::Instrument(1)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(8), midi::Time(200), midi::Duration(100), 100, midi::Instrument(2)));
}

TEST_CASE("read_notes, two notes on same track different channels")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 20, // MTrk size
        0, NOTE_ON(0, 5, 45),
        100, NOTE_OFF(0, 5, 0),
        100, NOTE_ON(2, 8, 48),
        100, NOTE_OFF(2, 8, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 45, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(8), midi::Time(200), midi::Duration(100), 48, midi::Instrument(0)));
}

TEST_CASE("read_notes, two notes on different tracks")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x02, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 12, // MTrk size
        0, NOTE_ON(0, 5, 100),
        100, NOTE_OFF(0, 5, 0),
        END_OF_TRACK,
        MTRK,
        0x00, 0x00, 0x00, 12, // MTrk size
        0, NOTE_ON(0, 88, 90),
        100, NOTE_OFF(0, 88, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 100, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(88), midi::Time(0), midi::Duration(100), 90, midi::Instrument(0)));
}

TEST_CASE("read_notes, two notes using running status")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 18, // MTrk size
        0, NOTE_ON(0, 5, 77),
        0, NOTE_ON_RS(6, 66),
        100, NOTE_OFF(0, 5, 0),
        0, NOTE_OFF_RS(6, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 77, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(6), midi::Time(0), midi::Duration(100), 66, midi::Instrument(0)));
}

TEST_CASE("read_notes, two notes using note on with 0 velocity")
{
    char buffer[] = {
        MTHD,
        0x00, 0x00, 0x00, 0x06, // MThd size
        0x00, 0x01, // Type
        0x00, 0x01, // Number of tracks
        0x01, 0x00, // Division
        MTRK,
        0x00, 0x00, 0x00, 17, // MTrk size
        0, NOTE_ON(0, 5, 123),
        0, NOTE_ON_RS(6, 111),
        100, NOTE_ON_RS(5, 0), // Note "off"
        0, NOTE_ON_RS(6, 0), // Note "off"
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 123, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(6), midi::Time(0), midi::Duration(100), 111, midi::Instrument(0)));
}

TEST_CASE("read_notes, 1000 notes")
{
    const unsigned N_NOTES = 1000;
    std::vector<char> note_bytes;

    for (unsigned i = 0; i != N_NOTES; ++i)
    {
        const char buffer[] = {
            0, NOTE_ON(0, 69, 71),
            100, NOTE_OFF(0, 69, 71)
        };

        note_bytes.insert(note_bytes.end(), buffer, buffer + sizeof(buffer));
    }

    auto ss = build_midi_file(note_bytes);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == N_NOTES);

    for ( unsigned i = 0; i != N_NOTES; ++i )
    {
        const auto& note = notes[i];

        CATCH_CHECK(note.start == midi::Time(i * 100));
        CATCH_CHECK(note.duration == midi::Duration(100));
        CATCH_CHECK(note.note_number == midi::NoteNumber(69));
    }
}

TEST_CASE("read_notes, 100000 notes")
{
    const unsigned N_NOTES = 100000;
    std::vector<char> note_bytes;

    for (unsigned i = 0; i != N_NOTES; ++i)
    {
        const char buffer[] = {
            0, NOTE_ON(0, 69, 71),
            100, NOTE_OFF(0, 69, 71)
        };

        note_bytes.insert(note_bytes.end(), buffer, buffer + sizeof(buffer));
    }

    auto ss = build_midi_file(note_bytes);
    std::vector<midi::NOTE> notes = midi::read_notes(ss);

    CATCH_REQUIRE(notes.size() == N_NOTES);
}

#endif
