#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "Catch.h"
#include <vector>


TEST_CASE("NoteCollector with single note (channel 0, number 5, from 0, duration 100)")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(5), 11);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(5), 0);

    CATCH_REQUIRE(notes.size() == 1);

    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 11, midi::Instrument(0)));
}

TEST_CASE("NoteCollector with single note (channel 2, number 5, from 0, duration 100)")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(0), midi::Channel(2), midi::NoteNumber(5), 32);
    collector.note_off(midi::Duration(100), midi::Channel(2), midi::NoteNumber(5), 0);

    CATCH_REQUIRE(notes.size() == 1);

    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 32, midi::Instrument(0)));
}

TEST_CASE("NoteCollector with two consecutive notes")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(5), 112);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(5), 0);
    collector.note_on(midi::Duration(100), midi::Channel(0), midi::NoteNumber(7), 113);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(7), 0);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 112, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(7), midi::Time(200), midi::Duration(100), 113, midi::Instrument(0)));
}

TEST_CASE("NoteCollector with two consecutive notes on different channels")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(5), 112);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(5), 0);
    collector.note_on(midi::Duration(100), midi::Channel(1), midi::NoteNumber(7), 113);
    collector.note_off(midi::Duration(100), midi::Channel(1), midi::NoteNumber(7), 0);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 112, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(7), midi::Time(200), midi::Duration(100), 113, midi::Instrument(0)));
}

TEST_CASE("NoteCollector interprets note on event with velocity 0 as a note off")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(100), midi::Channel(5), midi::NoteNumber(50), 64);
    collector.note_on(midi::Duration(200), midi::Channel(5), midi::NoteNumber(50), 0);

    CATCH_REQUIRE(notes.size() == 1);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(50), midi::Time(100), midi::Duration(200), 64, midi::Instrument(0)));
}

TEST_CASE("NoteCollector interprets note on event with velocity 0 as a note off (2 notes)")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(100), midi::Channel(3), midi::NoteNumber(50), 72);
    collector.note_on(midi::Duration(200), midi::Channel(3), midi::NoteNumber(40), 75);
    collector.note_off(midi::Duration(400), midi::Channel(3), midi::NoteNumber(40), 0);
    collector.note_off(midi::Duration(300), midi::Channel(3), midi::NoteNumber(50), 0);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(40), midi::Time(300), midi::Duration(400), 75, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(50), midi::Time(100), midi::Duration(900), 72, midi::Instrument(0)));
}

TEST_CASE("NoteCollector, 5 simultaneous notes")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.program_change(midi::Duration(0), midi::Channel(0), midi::Instrument(10));
    collector.program_change(midi::Duration(0), midi::Channel(1), midi::Instrument(20));
    collector.program_change(midi::Duration(0), midi::Channel(2), midi::Instrument(30));
    collector.program_change(midi::Duration(0), midi::Channel(3), midi::Instrument(40));
    collector.program_change(midi::Duration(0), midi::Channel(4), midi::Instrument(50));
    
    collector.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 1);
    collector.note_on(midi::Duration(0), midi::Channel(1), midi::NoteNumber(1), 2);
    collector.note_on(midi::Duration(0), midi::Channel(2), midi::NoteNumber(2), 3);
    collector.note_on(midi::Duration(0), midi::Channel(3), midi::NoteNumber(3), 4);
    collector.note_on(midi::Duration(0), midi::Channel(4), midi::NoteNumber(4), 5);

    collector.note_on(midi::Duration(1000), midi::Channel(0), midi::NoteNumber(0), 0);
    collector.note_on(midi::Duration(0), midi::Channel(1), midi::NoteNumber(1), 0);
    collector.note_on(midi::Duration(0), midi::Channel(2), midi::NoteNumber(2), 0);
    collector.note_on(midi::Duration(0), midi::Channel(3), midi::NoteNumber(3), 0);
    collector.note_on(midi::Duration(0), midi::Channel(4), midi::NoteNumber(4), 0);

    CATCH_REQUIRE(notes.size() == 5);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(0), midi::Time(0), midi::Duration(1000), 1, midi::Instrument(10)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(1), midi::Time(0), midi::Duration(1000), 2, midi::Instrument(20)));
    CATCH_CHECK(notes[2] == midi::NOTE(midi::NoteNumber(2), midi::Time(0), midi::Duration(1000), 3, midi::Instrument(30)));
    CATCH_CHECK(notes[3] == midi::NOTE(midi::NoteNumber(3), midi::Time(0), midi::Duration(1000), 4, midi::Instrument(40)));
    CATCH_CHECK(notes[4] == midi::NOTE(midi::NoteNumber(4), midi::Time(0), midi::Duration(1000), 5, midi::Instrument(50)));
}

TEST_CASE("NoteCollector, 2 notes encoded as on-on-off")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.note_on(midi::Duration(100), midi::Channel(0), midi::NoteNumber(50), 64);
    collector.note_on(midi::Duration(300), midi::Channel(0), midi::NoteNumber(50), 32);
    collector.note_on(midi::Duration(200), midi::Channel(0), midi::NoteNumber(50), 0);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(50), midi::Time(100), midi::Duration(300), 64, midi::Instrument(0)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(50), midi::Time(400), midi::Duration(200), 32, midi::Instrument(0)));
}

TEST_CASE("NoteCollector with two consecutive notes, instrument change on different channel")
{
    std::vector<midi::NOTE> notes;
    midi::NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });

    collector.program_change(midi::Duration(0), midi::Channel(0), midi::Instrument(41));
    collector.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(5), 112);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(5), 0);
    collector.program_change(midi::Duration(0), midi::Channel(1), midi::Instrument(45));
    collector.note_on(midi::Duration(100), midi::Channel(0), midi::NoteNumber(7), 113);
    collector.note_off(midi::Duration(100), midi::Channel(0), midi::NoteNumber(7), 0);

    CATCH_REQUIRE(notes.size() == 2);
    CATCH_CHECK(notes[0] == midi::NOTE(midi::NoteNumber(5), midi::Time(0), midi::Duration(100), 112, midi::Instrument(41)));
    CATCH_CHECK(notes[1] == midi::NOTE(midi::NoteNumber(7), midi::Time(200), midi::Duration(100), 113, midi::Instrument(41)));
}

#endif
