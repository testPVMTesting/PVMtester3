#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Reading MThd from {'M', 'T', 'h', 'd', 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01}")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0x00, 0x00, 0x00, 0x06, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01 };
    static_assert(sizeof(buffer) == sizeof(midi::MTHD), "Bug in tests");
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::MTHD mthd;

    read_mthd(ss, &mthd);
    CATCH_CHECK(header_id(mthd.header) == "MThd");
    CATCH_CHECK(mthd.header.size == 6);
    CATCH_CHECK(mthd.type == 1);
    CATCH_CHECK(mthd.ntracks == 1);
    CATCH_CHECK(mthd.division == 1);
}

TEST_CASE("Reading MThd from {'M', 'T', 'h', 'd', 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05, 0x02, 0x01}")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x05, 0x02, 0x01 };
    static_assert(sizeof(buffer) == sizeof(midi::MTHD), "Bug in tests");
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::MTHD mthd;

    read_mthd(ss, &mthd);
    CATCH_CHECK(header_id(mthd.header) == "MThd");
    CATCH_CHECK(mthd.header.size == 6);
    CATCH_CHECK(mthd.type == 0);
    CATCH_CHECK(mthd.ntracks == 5);
    CATCH_CHECK(mthd.division == 0x0201);
}

#endif
