#ifdef TEST_BUILD
#define CATCH_CONFIG_PREFIX_ALL
#define TEST_CASE CATCH_TEST_CASE

#include "io/read.h"
#include "Catch.h"
#include <sstream>

#define EXPECTED(...)                  __VA_ARGS__
#define BYTES(...)                     __VA_ARGS__

#define TEST(type, expected, bytes)                                                \
        TEST_CASE("read_array<" #type ">(n, {" #bytes "})")                        \
        {                                                                          \
            std::vector<type> exp = { expected };                                  \
            char buffer[] = { bytes };                                             \
            std::string data(buffer, sizeof(buffer));                              \
            std::stringstream ss(data);                                            \
                                                                                   \
            std::unique_ptr<type[]> result = io::read_array<type>(ss, exp.size()); \
                                                                                   \
            for ( unsigned i = 0; i != exp.size(); ++i )                           \
                CATCH_CHECK(result[i] == exp[i]);                                  \
        }


TEST(uint8_t, EXPECTED(0), BYTES(0))
TEST(uint8_t, EXPECTED(1), BYTES(1))
TEST(uint8_t, EXPECTED(5), BYTES(5))
TEST(uint8_t, EXPECTED(5, 4, 3, 2, 1), BYTES(5, 4, 3, 2, 1))
TEST(uint16_t, EXPECTED(0, 0), BYTES(0, 0, 0, 0))
TEST(uint16_t, EXPECTED(1, 2), BYTES(1, 0, 2, 0))
TEST(uint16_t, EXPECTED(256, 512), BYTES(0, 1, 0, 2))
TEST(uint32_t, EXPECTED(0), BYTES(0, 0, 0, 0))
TEST(uint32_t, EXPECTED(1), BYTES(1, 0, 0, 0))
TEST(uint32_t, EXPECTED(256), BYTES(0, 1, 0, 0))
TEST(uint32_t, EXPECTED(256 * 256), BYTES(0, 0, 1, 0))
TEST(uint32_t, EXPECTED(256 * 256 * 256), BYTES(0, 0, 0, 1))
TEST(uint32_t, EXPECTED(1, 256, 256*256, 256*256*256), BYTES(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1))

#endif
