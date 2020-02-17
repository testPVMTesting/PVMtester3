
#include "io/vli.h"
#include "io/read.h"

namespace io
{
	bool leftmost_bit_set(uint8_t byte)
	{
		return (byte >> 7) == 1;
		// return byte >= 128;
	}

	uint8_t lowest_7_bits(uint8_t byte)
	{
		return byte & 0b0111'1111;
		// return uint8_t(byte < 1) > 1
	}
}

uint64_t io::read_variable_length_integer(std::istream& in) {
	uint8_t byte = read<uint8_t>(in);
	uint64_t acc = 0;

	while (leftmost_bit_set(byte))
	{
		acc = (acc << 7) | lowest_7_bits(byte);
		byte = read<uint8_t>(in);
	}

	acc = (acc << 7) | lowest_7_bits(byte);
	return acc;
}
