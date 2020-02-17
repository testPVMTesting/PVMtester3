#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "tests/tests-util.h"
#include <sstream>

using namespace testutils;


TEST_CASE("Reading MTrk with polyphonic key pressure event (dt 5 channel 3 note 100 pressure 120)")
{
    char buffer[] = {
        MTRK,
        0x00, 0x00, 0x00, 8, // Length
        5, POLYPHONIC_KEY_PRESSURE(3, 100, 120),
        END_OF_TRACK
    };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    auto receiver = Builder()
        .polyphonic_key_pressure(midi::Duration(5), midi::Channel(3), midi::NoteNumber(100), 120)
        .meta(midi::Duration(0), 0x2F, "")
        .build();

    read_mtrk(ss, *receiver);
    receiver->check_finished();
}

#endif
