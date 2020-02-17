#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with channel pressure event (dt 128 channel 3 pressure 99)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        char(0b1000'0001), 0b0000'0000, CHANNEL_PRESSURE(3, 99),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .channel_pressure(midi::Duration(128), midi::Channel(3), 99)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
