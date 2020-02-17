#ifdef TEST_BUILD
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading empty MTrk")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 0x04, // Length
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder().meta(midi::Duration(0), 0x2F, "").build();
    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
