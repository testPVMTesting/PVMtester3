#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL

#include "midi/primitives.h"
#include "util/tagged.h"
#include "util/check-size.h"
#include "Catch.h"
#include <sstream>
#include <cstddef>
#include <type_traits>


static_assert(std::is_standard_layout<midi::Channel>::value, "Channel should have standard layout");

static_assert(std::is_convertible<midi::Channel, equality<midi::Channel>>::value, "Channel should derive from equality");

static_assert(!std::is_convertible<midi::Channel, ordered<midi::Channel>>::value, "Channel should not derive from ordered");

CATCH_TEST_CASE("sizeof(Channel)")
{
    check_size<midi::Channel, sizeof(uint8_t)>();
}

CATCH_TEST_CASE("Channel == Channel on equal channels")
{
    CATCH_CHECK(midi::Channel(10) == midi::Channel(10));
}

CATCH_TEST_CASE("Channel == Channel on unequal channels")
{
    CATCH_CHECK(!(midi::Channel(10) == midi::Channel(11)));
}

CATCH_TEST_CASE("Channel != Channel on equal channels")
{
    CATCH_CHECK(!(midi::Channel(5) != midi::Channel(5)));
}

CATCH_TEST_CASE("Channel != Channel on unequal channels")
{
    CATCH_CHECK(midi::Channel(5) != midi::Channel(6));
}

#endif
