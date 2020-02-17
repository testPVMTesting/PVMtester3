#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with note off event (dt 0 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        0, NOTE_OFF(0, 0, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 1 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        1, NOTE_OFF(0, 0, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(1), midi::Channel(0), midi::NoteNumber(0), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 0 note 0 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(0, 0, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(0), midi::NoteNumber(0), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 1 note 0 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(1, 0, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(1), midi::NoteNumber(0), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 0 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(15, 0, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(15), midi::NoteNumber(0), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 1 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(15, 1, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(15), midi::NoteNumber(1), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(15, 32, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(15), midi::NoteNumber(32), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with note off event (dt 0b1'0000000'0000000 channel 15 note 32 velocity 100)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        char(0b1000'0001), char(0b1000'0000), 0b0000'0000, NOTE_OFF(15, 32, 100),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .note_off(midi::Duration(0b1'0000000'0000000), midi::Channel(15), midi::NoteNumber(32), 100)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
