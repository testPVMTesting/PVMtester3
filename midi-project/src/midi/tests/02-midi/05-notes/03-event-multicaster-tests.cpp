#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <vector>
#include <functional>

using namespace testutils;


std::unique_ptr<uint8_t[]> copy_string_to_char_array(const std::string& string)
{
    auto result = std::make_unique<uint8_t[]>(string.size() + 1);

    for (size_t i = 0; i != string.size(); ++i)
    {
        result[i] = string[i];
    }
    
    result[string.size()] = 0;

    return std::move(result);
}

TEST_CASE("Multicaster test, one receiver, one event (note on)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().note_on(midi::Duration(1), midi::Channel(2), midi::NoteNumber(3), 4).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.note_on(midi::Duration(1), midi::Channel(2), midi::NoteNumber(3), 4);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (note off)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().note_off(midi::Duration(1), midi::Channel(2), midi::NoteNumber(3), 4).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.note_off(midi::Duration(1), midi::Channel(2), midi::NoteNumber(3), 4);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (polyphonic key pressure)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().polyphonic_key_pressure(midi::Duration(10), midi::Channel(5), midi::NoteNumber(20), 30).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.polyphonic_key_pressure(midi::Duration(10), midi::Channel(5), midi::NoteNumber(20), 30);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (control change)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().control_change(midi::Duration(5), midi::Channel(6), 7, 8).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.control_change(midi::Duration(5), midi::Channel(6), 7, 8);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (program change)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().program_change(midi::Duration(9), midi::Channel(6), midi::Instrument(3)).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.program_change(midi::Duration(9), midi::Channel(6), midi::Instrument(3));

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (channel pressure)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().channel_pressure(midi::Duration(8), midi::Channel(5), 2).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.channel_pressure(midi::Duration(8), midi::Channel(5), 2);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (pitch wheel change)")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().pitch_wheel_change(midi::Duration(1), midi::Channel(5), 9).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.pitch_wheel_change(midi::Duration(1), midi::Channel(5), 9);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, one event (meta)")
{
    std::string data = "fjlsaq";
    auto create_receiver = [&data]() {
        return std::shared_ptr<TestEventReceiver>(Builder().meta(midi::Duration(1), 9, data).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.meta(midi::Duration(1), 9, copy_string_to_char_array(data), data.size());

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, one receiver, two events")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(
            Builder()
            .note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 255)
            .note_off(midi::Duration(10), midi::Channel(0), midi::NoteNumber(0), 255)
            .build()
            .release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 255);
    multicaster.note_off(midi::Duration(10), midi::Channel(0), midi::NoteNumber(0), 255);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, two receivers, one event")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 255).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver(), create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.note_on(midi::Duration(0), midi::Channel(0), midi::NoteNumber(0), 255);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

TEST_CASE("Multicaster test, five receivers, one event")
{
    auto create_receiver = []() {
        return std::shared_ptr<TestEventReceiver>(Builder().note_on(midi::Duration(1000), midi::Channel(1), midi::NoteNumber(0), 255).build().release());
    };

    std::vector<std::shared_ptr<TestEventReceiver>> receivers{ create_receiver(), create_receiver(), create_receiver(), create_receiver(), create_receiver() };
    midi::EventMulticaster multicaster(std::vector<std::shared_ptr<midi::EventReceiver>>(receivers.begin(), receivers.end()));

    multicaster.note_on(midi::Duration(1000), midi::Channel(1), midi::NoteNumber(0), 255);

    for (auto receiver : receivers)
    {
        receiver->check_finished();
    }
}

#endif
