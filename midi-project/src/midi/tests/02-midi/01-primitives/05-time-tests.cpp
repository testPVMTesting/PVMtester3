#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL

#include "midi/primitives.h"
#include "util/check-size.h"
#include "util/tagged.h"
#include "Catch.h"
#include <sstream>
#include <cstddef>
#include <type_traits>


static_assert(std::is_standard_layout<midi::Time>::value, "Time should have standard layout");

static_assert(std::is_standard_layout<midi::Duration>::value, "Duration should have standard layout");

static_assert(std::is_convertible<midi::Time, ordered<midi::Time>>::value, "Time should derive from ordered");

static_assert(std::is_convertible<midi::Duration, ordered<midi::Duration>>::value, "Duration should derive from ordered");


CATCH_TEST_CASE("sizeof(Time)")
{
    check_size<midi::Time, sizeof(uint64_t)>();
}

CATCH_TEST_CASE("sizeof(Duration)")
{
    check_size<midi::Duration, sizeof(uint64_t)>();
}

CATCH_TEST_CASE("ostream << Time")
{
    std::stringstream ss;
    const midi::Time t(10);
    
    ss << t;

    CATCH_CHECK("10" == ss.str());
}

CATCH_TEST_CASE("ostream << Duration")
{
    std::stringstream ss;
    const midi::Duration dt(10);

    ss << dt;

    CATCH_CHECK("10" == ss.str());
}

CATCH_TEST_CASE("Time + Duration")
{
    const midi::Time t(10);
    const midi::Duration dt(2);
    midi::Time actual = t + dt;
    midi::Time expected(12);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Duration + Time")
{
    const midi::Time t(8);
    const midi::Duration dt(7);
    midi::Time actual = dt + t;
    midi::Time expected(15);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Time += Duration")
{
    midi::Time actual(15);
    const midi::Duration dt(5);
    actual += dt;
    midi::Time expected(20);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Checking return type of Time += Duration")
{
    midi::Time actual(15);
    const midi::Duration dt(5);
    (actual += dt) += dt;
    midi::Time expected(25);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Duration + Duration")
{
    const  midi::Duration dt1(17);
    const midi::Duration dt2(23);
    midi::Duration actual = dt1 + dt2;
    midi::Duration expected(17 + 23);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Duration += Duration")
{
    midi::Duration dt1(24);
    const midi::Duration dt2(77);
    (dt1 += dt2) += dt2;
    midi::Duration expected(24 + 77 + 77);

    CATCH_CHECK(expected == dt1);
}

CATCH_TEST_CASE("Duration -= Duration")
{
    midi::Duration dt1(2000);
    const midi::Duration dt2(100);
    (dt1 -= dt2) -= dt2;
    midi::Duration expected(2000 - 100 - 100);

    CATCH_CHECK(expected == dt1);
}

CATCH_TEST_CASE("Time - Time")
{
    const midi::Time t1(42);
    const midi::Time t2(11);
    midi::Duration actual = t1 - t2;
    midi::Duration expected(42 - 11);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Duration - Duration")
{
    const midi::Duration d1(500);
    const midi::Duration d2(150);
    midi::Duration actual = d1 - d2;
    midi::Duration expected(500 - 150);

    CATCH_CHECK(expected == actual);
}

CATCH_TEST_CASE("Time = Time")
{
    midi::Time t1(4);
    midi::Time t2(9);

    t1 = t2;

    CATCH_CHECK(t1 == t2);
}

CATCH_TEST_CASE("Duration = Duration")
{
    midi::Duration dt1(7);
    const midi::Duration dt2(5);

    dt1 = dt2;

    CATCH_CHECK(dt1 == dt2);
}

CATCH_TEST_CASE("Comparing Time")
{
    CATCH_CHECK(midi::Time(0) < midi::Time(1));
    CATCH_CHECK(!(midi::Time(5) < midi::Time(1)));
    CATCH_CHECK(!(midi::Time(3) < midi::Time(3)));

    CATCH_CHECK(midi::Time(4) > midi::Time(1));
    CATCH_CHECK(!(midi::Time(0) > midi::Time(1)));
    CATCH_CHECK(!(midi::Time(3) > midi::Time(3)));

    CATCH_CHECK(midi::Time(0) <= midi::Time(1));
    CATCH_CHECK(!(midi::Time(5) <= midi::Time(1)));
    CATCH_CHECK(midi::Time(3) <= midi::Time(3));

    CATCH_CHECK(midi::Time(4) >= midi::Time(1));
    CATCH_CHECK(!(midi::Time(0) >= midi::Time(1)));
    CATCH_CHECK(midi::Time(3) >= midi::Time(3));

    CATCH_CHECK(midi::Time(4) == midi::Time(4));
    CATCH_CHECK(!(midi::Time(4) == midi::Time(5)));

    CATCH_CHECK(midi::Time(6) != midi::Time(11));
    CATCH_CHECK(!(midi::Time(6) != midi::Time(6)));
}

CATCH_TEST_CASE("Comparing Duration")
{
    CATCH_CHECK(midi::Duration(0) < midi::Duration(1));
    CATCH_CHECK(!(midi::Duration(5) < midi::Duration(1)));
    CATCH_CHECK(!(midi::Duration(3) < midi::Duration(3)));

    CATCH_CHECK(midi::Duration(4) > midi::Duration(1));
    CATCH_CHECK(!(midi::Duration(0) > midi::Duration(1)));
    CATCH_CHECK(!(midi::Duration(3) > midi::Duration(3)));

    CATCH_CHECK(midi::Duration(0) <= midi::Duration(1));
    CATCH_CHECK(!(midi::Duration(5) <= midi::Duration(1)));
    CATCH_CHECK(midi::Duration(3) <= midi::Duration(3));

    CATCH_CHECK(midi::Duration(4) >= midi::Duration(1));
    CATCH_CHECK(!(midi::Duration(0) >= midi::Duration(1)));
    CATCH_CHECK(midi::Duration(3) >= midi::Duration(3));

    CATCH_CHECK(midi::Duration(4) == midi::Duration(4));
    CATCH_CHECK(!(midi::Duration(4) == midi::Duration(5)));

    CATCH_CHECK(midi::Duration(6) != midi::Duration(11));
    CATCH_CHECK(!(midi::Duration(6) != midi::Duration(6)));
}

#endif
