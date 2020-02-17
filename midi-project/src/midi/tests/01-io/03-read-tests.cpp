#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "io/read.h"
#include "Catch.h"
#include <sstream>


#define TEST(type, expected, ...)                                      \
        TEST_CASE("read<" #type "> from { " #__VA_ARGS__ " }")         \
        {                                                              \
            char buffer[] = { __VA_ARGS__ };                           \
            std::string data(buffer, sizeof(buffer));                  \
            std::stringstream ss(data);                                \
                                                                       \
            type result = io::read<type>(ss);                          \
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

#endif
