#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 0)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, PITCH_WHEEL_CHANGE(15, 0),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(midi::Duration(129), midi::Channel(15), 0)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 5)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, PITCH_WHEEL_CHANGE(15, 5),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(midi::Duration(129), midi::Channel(15), 5)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with pitch wheel change event (dt 129 channel 15 value 0b11011011100111)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0001, PITCH_WHEEL_CHANGE(15, 0b11011011100111),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .pitch_wheel_change(midi::Duration(129), midi::Channel(15), 0b11011011100111)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
