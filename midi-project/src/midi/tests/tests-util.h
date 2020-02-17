#ifndef TESTS_UTIL_H
#define TESTS_UTIL_H
#define CATCH_CONFIG_PREFIX_ALL

#include "Catch.h"
#include "midi/midi.h"
#include <cstdint>
#include <memory>
#include <vector>
#include <list>

#define MTHD                                    'M', 'T', 'h', 'd'
#define MTRK                                    'M', 'T', 'r', 'k'

#define NOTE_OFF_RS(note_index, velocity)                      char(note_index), char(velocity)
#define NOTE_OFF(channel, note_index, velocity)                char(0x80 | (channel)), NOTE_OFF_RS(note_index, velocity)

#define NOTE_ON_RS(note_index, velocity)                       char(note_index), char(velocity)
#define NOTE_ON(channel, note_index, velocity)                 char(0x90 | (channel)), NOTE_ON_RS(note_index, velocity)

#define POLYPHONIC_KEY_PRESSURE_RS(note_index, pressure)       char(note_index), char(pressure)
#define POLYPHONIC_KEY_PRESSURE(channel, note_index, pressure) char(0xA0 | (channel)), POLYPHONIC_KEY_PRESSURE_RS(note_index, pressure)

#define CONTROL_CHANGE_RS(controller, value)                   char(controller), char(value)
#define CONTROL_CHANGE(channel, controller, value)             char(0xB0 | (channel)), CONTROL_CHANGE_RS(controller, value)

#define PROGRAM_CHANGE_RS(program)                             char(program)
#define PROGRAM_CHANGE(channel, program)                       char(0xC0 | (channel)), PROGRAM_CHANGE_RS(program)

#define CHANNEL_PRESSURE_RS(pressure)                          char(pressure)
#define CHANNEL_PRESSURE(channel, pressure)                    char(0xD0 | (channel)), CHANNEL_PRESSURE_RS(pressure)

#define PITCH_WHEEL_CHANGE_RS(value)                           char(value & 0x7F), char(value >> 7)
#define PITCH_WHEEL_CHANGE(channel, value)                     char(0xE0 | (channel)), PITCH_WHEEL_CHANGE_RS(value)

#define END_OF_TRACK                                           0, char(0xFF), 0x2F, 0x00

namespace testutils
{
    struct Event
    {
        midi::Duration dt;

        virtual ~Event() { }
    };

    struct MetaEvent : public Event
    {
        uint8_t type;
        std::string data;
    };

    struct SysexEvent : public Event
    {
        std::string data;
    };

    struct MidiEvent : public Event
    {
        midi::Channel channel;
    };

    struct NoteOnEvent : public MidiEvent
    {
        midi::NoteNumber note;
        uint8_t velocity;
    };

    struct NoteOffEvent : public MidiEvent
    {
        midi::NoteNumber note;
        uint8_t velocity;
    };

    struct PolyphonicEvent : public MidiEvent
    {
        midi::NoteNumber note;
        uint8_t pressure;
    };

    struct ControlChangeEvent : public MidiEvent
    {
        uint8_t controller;
        uint8_t value;
    };

    struct ProgramChangeEvent : public MidiEvent
    {
        midi::Instrument program;
    };

    struct ChannelPressureEvent : public MidiEvent
    {
        uint8_t pressure;
    };

    struct PitchWheelChangeEvent : public MidiEvent
    {
        uint16_t value;
    };

    class TestEventReceiver : public midi::EventReceiver
    {
    private:
        std::list<std::unique_ptr<Event>> expected_events;

    public:
        TestEventReceiver(std::list<std::unique_ptr<Event>> expected_events) : expected_events(std::move(expected_events)) { }

        void note_on(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t velocity) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<NoteOnEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->note == note);
            CATCH_CHECK(actual->velocity == velocity);
        }

        void note_off(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t velocity) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<NoteOffEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->note == note);
            CATCH_CHECK(actual->velocity == velocity);
        }

        void polyphonic_key_pressure(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t pressure) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<PolyphonicEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->note == note);
            CATCH_CHECK(actual->pressure == pressure);
        }

        void control_change(midi::Duration dt, midi::Channel channel, uint8_t controller, uint8_t value) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ControlChangeEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->controller == controller);
            CATCH_CHECK(actual->value == value);
        }

        void program_change(midi::Duration dt, midi::Channel channel, midi::Instrument program) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ProgramChangeEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->program == program);
        }

        void channel_pressure(midi::Duration dt, midi::Channel channel, uint8_t pressure) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<ChannelPressureEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->channel == channel);
            CATCH_CHECK(actual->pressure == pressure);
        }

        void pitch_wheel_change(midi::Duration dt, midi::Channel channel, uint16_t value) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<PitchWheelChangeEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->value == value);
        }

        void meta(midi::Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override
        {
            {
                CATCH_INFO("This failure means that your function finds nonexistent events");

                CATCH_REQUIRE(expected_events.size() != 0);
            }

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<MetaEvent*>(expected.get());
            
            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_CHECK(actual->type == type);
            CATCH_REQUIRE(actual->data.size() == data_size);

            for (int i = 0; i != data_size; ++i)
            {
                CATCH_CHECK(actual->data[i] == data[i]);
            }
        }

        void sysex(midi::Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override
        {
            CATCH_REQUIRE(expected_events.size() != 0);

            std::unique_ptr<Event> expected = std::move(expected_events.front());
            expected_events.pop_front();
            auto actual = dynamic_cast<SysexEvent*>(expected.get());

            CATCH_REQUIRE(actual != nullptr);
            CATCH_CHECK(actual->dt == dt);
            CATCH_REQUIRE(actual->data.size() == data_size);

            for (int i = 0; i != data_size; ++i)
            {
                CATCH_CHECK(actual->data[i] == data[i]);
            }
        }

        void check_finished()
        {
            CATCH_SECTION("Checking that all events have been handled")
            {
                CATCH_INFO("This failure means that your code did not find all events in the track");

                CATCH_CHECK(expected_events.size() == 0);
            }
        }
    };

    class Builder
    {
    private:
        std::list<std::unique_ptr<Event>> expected_events;

    public:
        Builder& note_on(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t velocity)
        {
            auto event = std::make_unique<NoteOnEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->velocity = velocity;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& note_off(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t velocity)
        {
            auto event = std::make_unique<NoteOffEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->velocity = velocity;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& polyphonic_key_pressure(midi::Duration dt, midi::Channel channel, midi::NoteNumber note, uint8_t pressure)
        {
            auto event = std::make_unique<PolyphonicEvent>();
            event->dt = dt;
            event->channel = channel;
            event->note = note;
            event->pressure = pressure;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& control_change(midi::Duration dt, midi::Channel channel, uint8_t controller, uint8_t value)
        {
            auto event = std::make_unique<ControlChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->controller = controller;
            event->value = value;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& program_change(midi::Duration dt, midi::Channel channel, midi::Instrument program)
        {
            auto event = std::make_unique<ProgramChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->program = program;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& channel_pressure(midi::Duration dt, midi::Channel channel, uint8_t pressure)
        {
            auto event = std::make_unique<ChannelPressureEvent>();
            event->dt = dt;
            event->channel = channel;
            event->pressure = pressure;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& pitch_wheel_change(midi::Duration dt, midi::Channel channel, uint16_t value)
        {
            auto event = std::make_unique<PitchWheelChangeEvent>();
            event->dt = dt;
            event->channel = channel;
            event->value = value;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& meta(midi::Duration dt, uint8_t type, const std::string& data)
        {
            auto event = std::make_unique<MetaEvent>();
            event->dt = dt;
            event->type = type;
            event->data = data;

            expected_events.push_back(std::move(event));

            return *this;
        }

        Builder& sysex(midi::Duration dt, const std::string& data)
        {
            auto event = std::make_unique<SysexEvent>();
            event->dt = dt;
            event->data = data;

            expected_events.push_back(std::move(event));

            return *this;
        }

        std::unique_ptr<TestEventReceiver> build()
        {
            return std::make_unique<TestEventReceiver>(std::move(expected_events));
        }
    };
}

#endif