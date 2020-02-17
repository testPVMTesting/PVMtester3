#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with single zero-length meta event with dt=0")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        0x00, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(0), 0x01, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=1")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        0x01, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(1), 0x01, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=0b1000'0000")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0000, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(0b1000'0000), 0x01, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with single zero-length meta event with dt=0b1000'0110")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        char(0b1000'0001), 0b0000'0110, char(0xFF), 0x01, 0x00, // Some zero-length meta event
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(0b1000'0110), 0x01, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with single meta event with data = "x")")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 9, // Length
        0, char(0xFF), 0x05, 0x01, 'x',
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string metadata = "x";
    auto receiver = Builder()
        .meta(midi::Duration(0), 0x05, metadata)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with single meta event with data = "K5")")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xFF), 0x05, 0x02, 'K', '5',
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string metadata = "K5";
    auto receiver = Builder()
        .meta(midi::Duration(0), 0x05, metadata)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with two meta events")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 12, // Length
        1, char(0xFF), 0x01, 0x00, // Meta 1
        1, char(0xFF), 0x02, 0x00, // Meta 2
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(1), 0x01, "")
        .meta(midi::Duration(1), 0x02, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE("Reading MTrk with three meta events")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 12, // Length
        1, char(0xFF), 0x01, 0x00, // Meta 1
        1, char(0xFF), 0x02, 0x00, // Meta 2
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .meta(midi::Duration(1), 0x01, "")
        .meta(midi::Duration(1), 0x02, "")
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with single meta event with data length 128; checks that length is read as VLI)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, char(137), // Length
        0, char(0xFF), char(0x01), char(0b1000'0001), 0, '1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8','1', '2', '3', '4', '5', '6', '7', '8', '1', '2', '3', '4', '5', '6', '7', '8',
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string metadata = "12345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678123456781234567812345678";
    auto receiver = Builder()
        .meta(midi::Duration(0), 0x01, metadata)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

TEST_CASE(R"(Reading MTrk with single meta event, VLI length == 3)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 10, // Length
        0, char(0xFF), 0x05, char(0b1000'0000), char(0b1000'0000), char(0b0000'0000),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    std::string metadata = "";
    auto receiver = Builder()
        .meta(midi::Duration(0), 0x05, metadata)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
