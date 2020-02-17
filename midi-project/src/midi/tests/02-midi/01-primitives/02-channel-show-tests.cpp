#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL

#include "midi/primitives.h"
#include "util/tagged.h"
#include "Catch.h"
#include <sstream>
#include <cstddef>
#include <type_traits>


CATCH_TEST_CASE("ostream << Channel(0)")
{
    std::stringstream ss;
    midi::Channel c(0);

    ss << c;

    CATCH_CHECK("0" == ss.str());
}

CATCH_TEST_CASE("ostream << Channel(5)")
{
    std::stringstream ss;
    midi::Channel c(5);

    ss << c;

    CATCH_CHECK("5" == ss.str());
}

CATCH_TEST_CASE("ostream << Channel(10)")
{
    std::stringstream ss;
    midi::Channel c(10);

    ss << c;

    CATCH_CHECK("10" == ss.str());
}

CATCH_TEST_CASE("ostream << Channel(15)")
{
    std::stringstream ss;
    midi::Channel c(15);

    ss << c;

    CATCH_CHECK("15" == ss.str());
}

#endif
