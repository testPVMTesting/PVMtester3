#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>
#include <cstddef>


TEST_CASE("Checking that NOTE is defined correctly")
{
    static_assert(std::is_same<decltype(midi::NOTE::note_number), midi::NoteNumber>::value, "NOTE is lacking member 'note_index' of the appropriate type");
    static_assert(std::is_same<decltype(midi::NOTE::start), midi::Time>::value, "NOTE is lacking member 'start' of the appropriate type");
    static_assert(std::is_same<decltype(midi::NOTE::duration), midi::Duration>::value, "NOTE is lacking member 'duration' of the appropriate type");
    static_assert(std::is_same<decltype(midi::NOTE::instrument), midi::Instrument>::value, "NOTE is lacking member 'instrument' of the appropriate type");
    static_assert(std::is_standard_layout<midi::NOTE>::value, "NOTE should have standard layout");
}

TEST_CASE("Comparing equal NOTE objects")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 42, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 42, midi::Instrument(0));

    CATCH_CHECK(a == b);
}

TEST_CASE("Comparing unequal NOTE objects (different instruments)")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 90, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 90, midi::Instrument(1));

    CATCH_CHECK(!(a == b));
}

TEST_CASE("Comparing unequal NOTE objects (different note indices)")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 74, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(2), midi::Time(4), midi::Duration(10), 74, midi::Instrument(0));

    CATCH_CHECK(!(a == b));
}

TEST_CASE("Comparing unequal NOTE objects (different starts)")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 15, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(1), midi::Time(3), midi::Duration(10), 15, midi::Instrument(0));

    CATCH_CHECK(!(a == b));
}

TEST_CASE("Comparing unequal NOTE objects (different durations)")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 5, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(1), midi::Time(4), midi::Duration(20), 5, midi::Instrument(0));

    CATCH_CHECK(!(a == b));
}

TEST_CASE("Comparing unequal NOTE objects (different velocities)")
{
    const midi::NOTE a(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 1, midi::Instrument(0));
    const midi::NOTE b(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 2, midi::Instrument(0));

    CATCH_CHECK(!(a == b));
}

TEST_CASE("ostream << NOTE")
{
    const midi::NOTE note(midi::NoteNumber(1), midi::Time(4), midi::Duration(10), 1, midi::Instrument(45));
    std::string expected = "Note(number=1,start=4,duration=10,instrument=45)";
    std::stringstream ss;

    ss << note;

    CATCH_CHECK(expected == ss.str());
}

#endif
