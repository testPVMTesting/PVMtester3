#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "io/read.h"
#include "Catch.h"
#include <sstream>


#define TEST(type, expected, ...)                                      \
        TEST_CASE("read_to<" #type "> from { " #__VA_ARGS__ " }")      \
        {                                                              \
            char buffer[] = { __VA_ARGS__ };                           \
            std::string data(buffer, sizeof(buffer));                  \
            std::stringstream ss(data);                                \
            type result;                                               \
                                                                       \
            io::read_to(ss, &result);                                  \
            CATCH_CHECK(result == expected);                           \
        }


TEST(uint8_t, 0, 0)
TEST(uint8_t, 1, 1)
TEST(uint8_t, 5, 5)
TEST(uint8_t, 77, 77)

TEST(uint16_t, 0, 0, 0)
TEST(uint16_t, 1, 1, 0)
TEST(uint16_t, 5, 5, 0)
TEST(uint16_t, 256, 0, 1)

TEST(uint32_t, 0, 0, 0, 0, 0)
TEST(uint32_t, 1, 1, 0, 0, 0)
TEST(uint32_t, 5, 5, 0, 0, 0)
TEST(uint32_t, 0x100, 0, 1, 0, 0)
TEST(uint32_t, 0x10000, 0, 0, 1, 0)
TEST(uint32_t, 0x01000000, 0, 0, 0, 1)
TEST(uint32_t, 0x12345678, 0x78, 0x56, 0x34, 0x12)

TEST_CASE("read_to<Foo> from { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 }")
{
#   pragma pack(push, 1)
    struct Foo
    {
        uint8_t a;
        uint16_t b;
        uint32_t c;
    };
#   pragma pack(pop)

    char buffer[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);
    Foo result;

    io::read_to(ss, &result);
    CATCH_CHECK(result.a == 0x01);
    CATCH_CHECK(result.b == 0x0302);
    CATCH_CHECK(result.c == 0x07060504);
}

TEST_CASE("read_to<uint_8> from { 0x01, 0x02, 0x03, 0x04 } with n == 4")
{
    uint8_t result[4];
    char buffer[] = { 0x01, 0x02, 0x03, 0x04 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    io::read_to(ss, result, 4);
    CATCH_CHECK(result[0] == 0x01);
    CATCH_CHECK(result[1] == 0x02);
    CATCH_CHECK(result[2] == 0x03);
    CATCH_CHECK(result[3] == 0x04);
}

TEST_CASE("read_to<uint_16> from { 0x11, 0x22, 0x33, 0x44 } with n == 2")
{
    uint16_t result[2];
    char buffer[] = { 0x11, 0x22, 0x33, 0x44 };
    std::string data(buffer, sizeof(buffer));
    std::stringstream ss(data);

    io::read_to(ss, result, 2);
    CATCH_CHECK(result[0] == 0x2211);
    CATCH_CHECK(result[1] == 0x4433);
}

#endif
