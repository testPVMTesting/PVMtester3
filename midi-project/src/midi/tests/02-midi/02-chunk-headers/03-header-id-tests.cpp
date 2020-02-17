#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Reading id from header with id MThd")
{
    const midi::CHUNK_HEADER header{ { 'M', 'T', 'h', 'd'}, 0 };
    std::string id = header_id(header);

    CATCH_CHECK(id == "MThd");
}

TEST_CASE("Reading id from header with id MTrk")
{
    const midi::CHUNK_HEADER header{ { 'M', 'T', 'r', 'k' }, 111 };
    std::string id = header_id(header);

    CATCH_CHECK(id == "MTrk");
}

TEST_CASE("Reading id from header with id abcd")
{
    const midi::CHUNK_HEADER header{ { 'a', 'b', 'c', 'd' }, 78651 };
    std::string id = header_id(header);

    CATCH_CHECK(id == "abcd");
}

#endif
