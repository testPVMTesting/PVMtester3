#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL

#include "midi/primitives.h"
#include "util/tagged.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <cstddef>
#include <type_traits>


static_assert(std::is_standard_layout<midi::NoteNumber>::value, "NoteNumber should have standard layout");

static_assert(std::is_convertible<midi::NoteNumber, ordered<midi::NoteNumber>>::value, "Instrument should derive from ordered");

CATCH_TEST_CASE("sizeof(NoteNumber)")
{
    check_size<midi::NoteNumber, sizeof(uint8_t)>();
}

CATCH_TEST_CASE("ostream << NoteNumber")
{
    std::stringstream ss;
    midi::NoteNumber n(10);

    ss << n;

    CATCH_CHECK("10" == ss.str());
}

CATCH_TEST_CASE("midi::NoteNumber == NoteNumber on equal values")
{
    CATCH_CHECK(midi::NoteNumber(10) == midi::NoteNumber(10));
}

CATCH_TEST_CASE("midi::NoteNumber == NoteNumber on unequal values")
{
    CATCH_CHECK(!(midi::NoteNumber(10) == midi::NoteNumber(11)));
}

CATCH_TEST_CASE("midi::NoteNumber != NoteNumber on equal values")
{
    CATCH_CHECK(!(midi::NoteNumber(5) != midi::NoteNumber(5)));
}

CATCH_TEST_CASE("midi::NoteNumber != NoteNumber on unequal values")
{
    CATCH_CHECK(midi::NoteNumber(5) != midi::NoteNumber(6));
}

CATCH_TEST_CASE("midi::NoteNumber < NoteNumber")
{
    CATCH_CHECK(midi::NoteNumber(5) < midi::NoteNumber(6));
    CATCH_CHECK(!(midi::NoteNumber(10) < midi::NoteNumber(9)));
    CATCH_CHECK(!(midi::NoteNumber(10) < midi::NoteNumber(10)));
}

CATCH_TEST_CASE("midi::NoteNumber > NoteNumber")
{
    CATCH_CHECK(!(midi::NoteNumber(5) > midi::NoteNumber(6)));
    CATCH_CHECK(midi::NoteNumber(10) > midi::NoteNumber(9));
    CATCH_CHECK(!(midi::NoteNumber(10) > midi::NoteNumber(10)));
}

CATCH_TEST_CASE("midi::NoteNumber <= NoteNumber")
{
    CATCH_CHECK(midi::NoteNumber(5) <= midi::NoteNumber(6));
    CATCH_CHECK(midi::NoteNumber(5) <= midi::NoteNumber(5));
    CATCH_CHECK(!(midi::NoteNumber(10) <= midi::NoteNumber(9)));
}

CATCH_TEST_CASE("midi::NoteNumber >= NoteNumber")
{
    CATCH_CHECK(midi::NoteNumber(5) >= midi::NoteNumber(2));
    CATCH_CHECK(midi::NoteNumber(5) >= midi::NoteNumber(5));
    CATCH_CHECK(!(midi::NoteNumber(4) >= midi::NoteNumber(5)));
}

#endif
