#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "midi/midi.h"
#include "Catch.h"
#include <sstream>
#include <type_traits>


TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'h', 'd', 0, 0, 0, 0 }")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0, 0, 0, 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::CHUNK_HEADER header;
    midi::read_chunk_header(ss, &header);
    std::string type(header.id, sizeof(header.id));

    CATCH_CHECK(type == "MThd");
    CATCH_CHECK(header.size == 0);
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'h', 'd', 0x01, 0, 0, 0 }")
{
    char buffer[] = { 'M', 'T', 'h', 'd', 0x01, 0, 0, 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::CHUNK_HEADER header;
    midi::read_chunk_header(ss, &header);

    CATCH_SECTION("Checking id")
    {
        std::string type(header.id, sizeof(header.id));
        CATCH_CHECK(type == "MThd");
    }

    CATCH_SECTION("Checking size")
    {
        CATCH_INFO("Have you taken into account endianness?")
            CATCH_CHECK(header.size == 0x01000000);
    }
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'r', 'k', 0x01, 0x02, 0x03, 0x04 }")
{
    char buffer[] = { 'M', 'T', 'r', 'k', 0x01, 0x02, 0x03, 0x04 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::CHUNK_HEADER header;
    midi::read_chunk_header(ss, &header);

    CATCH_SECTION("Checking id")
    {
        std::string type(header.id, sizeof(header.id));
        CATCH_CHECK(type == "MTrk");
    }

    CATCH_SECTION("Checking size")
    {
        CATCH_INFO("Have you taken into account endianness?");
        CATCH_CHECK(header.size == 0x01020304);
    }
}

TEST_CASE("Reading CHUNK_HEADER { 'M', 'T', 'r', 'k', 0x45, 0x12, 0x75, 0x66 }")
{
    char buffer[] = { 'M', 'T', 'r', 'k', 0x45, 0x12, 0x75, 0x66 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    midi::CHUNK_HEADER header;
    midi::read_chunk_header(ss, &header);

    CATCH_SECTION("Checking id")
    {
        std::string type(header.id, sizeof(header.id));
        CATCH_CHECK(type == "MTrk");
    }

    CATCH_SECTION("Checking size")
    {
        CATCH_INFO("Have you taken into account endianness?");
        CATCH_CHECK(header.size == 0x45127566);
    }
}

#endif
