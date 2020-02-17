#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>


TEST_CASE("Checking that CHUNK_HEADER is defined correctly")
{
    static_assert(std::is_same<decltype(midi::CHUNK_HEADER::id), char[4]>::value, "CHUNK_HEADER is lacking member 'id' of the appropriate type");
    static_assert(std::is_same<decltype(midi::CHUNK_HEADER::size), uint32_t>::value, "CHUNK_HEADER is lacking member 'size' of appropriate type");
    check_size<midi::CHUNK_HEADER, sizeof(char[4]) + sizeof(uint32_t)>();
    static_assert(std::is_standard_layout<midi::CHUNK_HEADER>::value, "CHUNK_HEADER is not a standard layout type");

    static_assert(offsetof(midi::CHUNK_HEADER, id) == 0, "CHUNK_HEADER's id field does not have the correct offset");
    static_assert(offsetof(midi::CHUNK_HEADER, size) == 4, "CHUNK_HEADER's size field does not have the correct offset");
}

#endif
