#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL

#include "midi/primitives.h"
#include "util/tagged.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <cstddef>
#include <type_traits>


static_assert(std::is_standard_layout<midi::Instrument>::value, "Instrument should have standard layout");

static_assert(std::is_convertible<midi::Instrument, equality<midi::Instrument>>::value, "Instrument should derive from equality");

static_assert(!std::is_convertible<midi::Instrument, ordered<midi::Instrument>>::value, "Instrument should not derive from ordered");

CATCH_TEST_CASE("sizeof(Instrument)")
{
    check_size<midi::Instrument, sizeof(uint8_t)>();
}

CATCH_TEST_CASE("ostream << Instrument")
{
    std::stringstream ss;
    midi::Instrument c(10);

    ss << c;

    CATCH_CHECK("10" == ss.str());
}

CATCH_TEST_CASE("Instrument == Instrument on equal channels")
{
    CATCH_CHECK(midi::Instrument(10) == midi::Instrument(10));
}

CATCH_TEST_CASE("Instrument == Instrument on unequal channels")
{
    CATCH_CHECK(!(midi::Instrument(10) == midi::Instrument(11)));
}

CATCH_TEST_CASE("Instrument != Instrument on equal channels")
{
    CATCH_CHECK(!(midi::Instrument(5) != midi::Instrument(5)));
}

CATCH_TEST_CASE("Instrument != Instrument on unequal channels")
{
    CATCH_CHECK(midi::Instrument(5) != midi::Instrument(6));
}

#endif
