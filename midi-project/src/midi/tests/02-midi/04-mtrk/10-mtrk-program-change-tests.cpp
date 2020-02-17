#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with program change event (dt 127 channel 4 program 2)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 7, // Length
        127, PROGRAM_CHANGE(4, 2),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .program_change(midi::Duration(127), midi::Channel(4), midi::Instrument(2))
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
