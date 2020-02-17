#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with control change event (dt 77 channel 7 controller 3 value 55)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        77, CONTROL_CHANGE(7, 3, 55),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .control_change(midi::Duration(77), midi::Channel(7), 3, 55)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
