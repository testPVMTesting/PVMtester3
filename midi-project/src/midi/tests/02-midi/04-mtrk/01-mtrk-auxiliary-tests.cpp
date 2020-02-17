#ifdef TEST_BUILD

#include "midi/midi.h"
#include "Catch.h"

TEST_CASE("is_sysex_event")
{
    CHECK(!midi::is_sysex_event(0x00));
    CHECK(!midi::is_sysex_event(0x70));
    CHECK(midi::is_sysex_event(0xF0));
    CHECK(midi::is_sysex_event(0xF7));
}

TEST_CASE("is_meta_event")
{
    CHECK(!midi::is_meta_event(0));
    CHECK(!midi::is_meta_event(0x82));
    CHECK(midi::is_meta_event(0xFF));
}

TEST_CASE("is_midi_event")
{
    CHECK(!midi::is_midi_event(0x70));
    CHECK(midi::is_midi_event(0x80));
    CHECK(midi::is_midi_event(0x81));
    CHECK(midi::is_midi_event(0x8F));
    CHECK(midi::is_midi_event(0x95));
    CHECK(midi::is_midi_event(0xA3));
    CHECK(midi::is_midi_event(0xB8));
    CHECK(midi::is_midi_event(0xC3));
    CHECK(midi::is_midi_event(0xD1));
    CHECK(midi::is_midi_event(0xE7));
    CHECK(!midi::is_midi_event(0xF0));
    CHECK(!midi::is_midi_event(0xF1));
    CHECK(!midi::is_midi_event(0xFF));
}

TEST_CASE("extract_midi_event_type")
{
    CHECK(midi::extract_midi_event_type(0x80) == 0x08);
    CHECK(midi::extract_midi_event_type(0x85) == 0x08);
    CHECK(midi::extract_midi_event_type(0x92) == 0x09);
    CHECK(midi::extract_midi_event_type(0xB0) == 0x0B);
    CHECK(midi::extract_midi_event_type(0xC1) == 0x0C);
    CHECK(midi::extract_midi_event_type(0xD7) == 0x0D);
    CHECK(midi::extract_midi_event_type(0xE4) == 0x0E);
}

TEST_CASE("extract_midi_event_channel")
{
    CHECK(midi::extract_midi_event_channel(0x80) == midi::Channel(0x00));
    CHECK(midi::extract_midi_event_channel(0x85) == midi::Channel(0x05));
    CHECK(midi::extract_midi_event_channel(0x92) == midi::Channel(0x02));
    CHECK(midi::extract_midi_event_channel(0xB0) == midi::Channel(0x00));
    CHECK(midi::extract_midi_event_channel(0xC1) == midi::Channel(0x01));
    CHECK(midi::extract_midi_event_channel(0xD7) == midi::Channel(0x07));
    CHECK(midi::extract_midi_event_channel(0xE4) == midi::Channel(0x04));
}

TEST_CASE("is_running_status")
{
    CHECK( midi::is_running_status(0x00));
    CHECK( midi::is_running_status(0x01));
    CHECK( midi::is_running_status(0x57));
    CHECK( midi::is_running_status(0x7F));
    CHECK(!midi::is_running_status(0x80));
    CHECK(!midi::is_running_status(0x80));
    CHECK(!midi::is_running_status(0x81));
    CHECK(!midi::is_running_status(0xFF));
}

TEST_CASE("is_note_off")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_note_off(x) == (x == 0x08));
    }
}

TEST_CASE("is_note_on")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_note_on(x) == (x == 0x09));
    }
}

TEST_CASE("is_polyphonic_key_pressure")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_polyphonic_key_pressure(x) == (x == 0x0A));
    }
}

TEST_CASE("is_control_change")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_control_change(x) == (x == 0x0B));
    }
}

TEST_CASE("is_program_change")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_program_change(x) == (x == 0x0C));
    }
}

TEST_CASE("is_channel_pressure")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_channel_pressure(x) == (x == 0x0D));
    }
}

TEST_CASE("is_pitch_wheel_change")
{
    for (unsigned i = 0; i != 0x100; ++i)
    {
        auto x = uint8_t(i);
        CHECK(midi::is_pitch_wheel_change(x) == (x == 0x0E));
    }
}

#endif
