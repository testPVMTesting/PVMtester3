#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with sysex event with no data")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 7, // Length
        0, char(0xF0), 0x00, // Sysex
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .sysex(midi::Duration(0), "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with sysex event with data = "abc")")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xF0), 0x03, 'a', 'b', 'c', // Sysex
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string sysex_data = "abc";
    auto receiver = Builder()
        .sysex(midi::Duration(0), sysex_data)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with sysex event at dt = 0b1111'1111")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        char(0b1000'0001), 0b0111'1111, char(0xF0), 0, // Sysex
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .sysex(midi::Duration(0b1111'1111), "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with sysex event with VLI length == 2)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        0, char(0xF0), char(0b1000'0000), 0, // Sysex
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string sysex_data = "";
    auto receiver = Builder()
        .sysex(midi::Duration(0), sysex_data)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with sysex event with VLI length == 3)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xF0), char(0b1000'0000), char(0b1000'0000), 1, 'q', // Sysex
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string sysex_data = "q";
    auto receiver = Builder()
        .sysex(midi::Duration(0), sysex_data)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
