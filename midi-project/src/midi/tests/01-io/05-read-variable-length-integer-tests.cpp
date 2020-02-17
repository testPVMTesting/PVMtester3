#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "io/vli.h"
#include "Catch.h"
#include <sstream>


TEST_CASE("Reading variable sized integer from { 0x00 }")
{
    char buffer[] = { 0x00 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0);
}

TEST_CASE("Reading variable sized integer from { 0x01 }")
{
    char buffer[] = { 0x01 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 1);
}

TEST_CASE("Reading variable sized integer from { 0x7F }")
{
    char buffer[] = { 0x7F };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0x7F);
}

TEST_CASE("Reading variable sized integer from { 0x81, 0x00 }")
{
    char buffer[] = { char(0x81), 0x00 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == (1 << 7));
}

TEST_CASE("Reading variable sized integer from { 0x81, 0x80, 0x00 }")
{
    char buffer[] = { char(0x81), char(0x80), 0x00 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == (1 << 14));
}

TEST_CASE("Reading variable sized integer from { 0x81, 0x80, 0x80, 0x00 }")
{
    char buffer[] = { char(0x81), char(0x80), char(0x80), 0x00 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == (1 << 21));
}

TEST_CASE("Reading variable sized integer from { 0b10000001, 0b10000001, 0b10000001, 0b00000001 }")
{
    char buffer[] = { char(0b10000001), char(0b10000001), char(0b10000001), char(0b00000001) };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b1000000100000010000001);
}

TEST_CASE("Reading variable sized integer from { 0b10000111, 0b10010001, 0b11010101, 0b00000000 }")
{
    char buffer[] = { char(0b10000111), char(0b10010001), char(0b11010101), char(0b00000000) };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b0000111001000110101010000000);
}

TEST_CASE("Reading variable sized integer from { 0b10000111, 0b10010001, 0b11010101, 0b00000000, 0 }")
{
    char buffer[] = { char(0b10000111), char(0b10010001), char(0b11010101), char(0b00000000), 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b0000111001000110101010000000);
}

TEST_CASE("Reading variable sized integer from { 0b00000111, 0b10010001, 0b11010101, 0b00000000, 0 }")
{
    char buffer[] = { char(0b00000111), char(0b10010001), char(0b11010101), char(0b00000000), 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b0000111);
}

TEST_CASE("Reading variable sized integer from { 0b10000111, 0b10010001, 0b11010101, 0b01111111, 0 }")
{
    char buffer[] = { char(0b1000'0111), char(0b100'10001), char(0b1101'0101), char(0b0111'1111), 0 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b0000111'0010001'1010101'1111111);
}

TEST_CASE("Reading variable sized integer from { 0b11111111, 0b10000000, 0b10000000, 0b10001100, 0b00000010 }")
{
    char buffer[] = { char(0b11111111), char(0b10000000), char(0b10000000), char(0b10001100), char(0b00000010) };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    auto actual = io::read_variable_length_integer(ss);

    CATCH_CHECK(actual == 0b1111111'0000000'0000000'0001100'0000010);
}

#endif
