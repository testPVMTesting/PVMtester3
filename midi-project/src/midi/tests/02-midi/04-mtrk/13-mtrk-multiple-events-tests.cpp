#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk, multiple note on events without running status")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 12, // Length
        0, NOTE_ON(0, 0, 0),
        10, NOTE_ON(0, 5, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 0)
        .note_on(midi::Duration(10), midi::Channel(0), midi::NoteNumber(5), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk, multiple note on events with running status")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 11, // Length
        0, NOTE_ON(0, 0, 0),
        10, NOTE_ON_RS(5, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 0)
        .note_on(midi::Duration(10), midi::Channel(0), midi::NoteNumber(5), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk, multiple note off events with running status")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 14, // Length
        0, NOTE_OFF(1, 0, 0),
        10, NOTE_OFF_RS(5, 0),
        20, NOTE_OFF_RS(9, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0), midi::Channel(1), midi::NoteNumber(0), 0)
        .note_off(midi::Duration(10), midi::Channel(1), midi::NoteNumber(5), 0)
        .note_off(midi::Duration(20), midi::Channel(1), midi::NoteNumber(9), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk, multiple program change events with running status")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 11, // Length
        10, PROGRAM_CHANGE(2, 5),
        40, PROGRAM_CHANGE_RS(10),
        20, PROGRAM_CHANGE_RS(111),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .program_change(midi::Duration(10), midi::Channel(2), midi::Instrument(5))
        .program_change(midi::Duration(40), midi::Channel(2), midi::Instrument(10))
        .program_change(midi::Duration(20), midi::Channel(2), midi::Instrument(111))
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk, running status with different kinds of events")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 34, // Length
        10, NOTE_ON(1, 10, 55),
        0, NOTE_ON_RS(20, 66),
        0, NOTE_ON_RS(30, 77),
        50, NOTE_ON_RS(20, 0),
        10, CHANNEL_PRESSURE(5, 42),
        60, CHANNEL_PRESSURE_RS(100),
        40, NOTE_OFF(1, 30, 0),
        20, NOTE_OFF_RS(10, 0),
        40, PITCH_WHEEL_CHANGE(7, 10),
        25, PITCH_WHEEL_CHANGE_RS(20),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(midi::Duration(10), midi::Channel(1), midi::NoteNumber(10), 55)
        .note_on(midi::Duration(0), midi::Channel(1), midi::NoteNumber(20), 66)
        .note_on(midi::Duration(0), midi::Channel(1), midi::NoteNumber(30), 77)
        .note_on(midi::Duration(50), midi::Channel(1), midi::NoteNumber(20), 0)
        .channel_pressure(midi::Duration(10), midi::Channel(5), 42)
        .channel_pressure(midi::Duration(60), midi::Channel(5), 100)
        .note_off(midi::Duration(40), midi::Channel(1), midi::NoteNumber(30), 0)
        .note_off(midi::Duration(20), midi::Channel(1), midi::NoteNumber(10), 0)
        .pitch_wheel_change(midi::Duration(40), midi::Channel(7), 10)
        .pitch_wheel_change(midi::Duration(25), midi::Channel(7), 20)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk consisting of note on, note on, note off, note off")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 18, // Length
        0, NOTE_ON(0, 50, 127),
        0, NOTE_ON_RS(49, 127),
        10, NOTE_OFF(0, 50, 0),
        0, NOTE_OFF_RS(49, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(50), 127)
        .note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(49), 127)
        .note_off(midi::Duration(10), midi::Channel(0), midi::NoteNumber(50), 0)
        .note_off(midi::Duration(0), midi::Channel(0), midi::NoteNumber(49), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
