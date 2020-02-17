#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Checking that MThd is defined correctly")
{
    static_assert(std::is_same<decltype(midi::MTHD::header), midi::CHUNK_HEADER>::value, "MThd is lacking member 'header' of the appropriate type");
    static_assert(std::is_same<decltype(midi::MTHD::type), uint16_t>::value, "MThd is lacking member 'type' of the appropriate type");
    static_assert(std::is_same<decltype(midi::MTHD::ntracks), uint16_t>::value, "MThd is lacking member 'ntracks' of the appropriate type");
    static_assert(std::is_same<decltype(midi::MTHD::division), uint16_t>::value, "MThd is lacking member 'division' of the appropriate type");
    check_size<midi::MTHD, sizeof(midi::CHUNK_HEADER) + 3 * sizeof(uint16_t)>();

    static_assert(offsetof(midi::MTHD, header) == 0, "MThd's header field does not have the correct offset");
    static_assert(offsetof(midi::MTHD, type) == 8, "MThd's type field does not have the correct offset");
    static_assert(offsetof(midi::MTHD, ntracks) == 10, "MThd's ntracks field does not have the correct offset");
    static_assert(offsetof(midi::MTHD, division) == 12, "MThd's division field does not have the correct offset");
}

#endif