#include "endianness.h"

namespace io {

	void switch_endianness(uint16_t* n) {
		*n = (*n << 8) | (*n >> 8);
	}
	void switch_endianness(uint32_t* n) {

		uint32_t byte4 = (*n & 0xFF000000) >> 24;
		uint32_t byte3 = (*n & 0x00FF0000) >> 8;
		uint32_t byte2 = (*n & 0x0000FF00) << 8;
		uint32_t byte1 = (*n & 0x0000000FF) << 24;

		*n = byte1 | byte2 | byte3 | byte4;
	}
	void switch_endianness(uint64_t* n) {
		uint64_t byte8 = (*n & 0xFF00000000000000) >> 56;
		uint64_t byte7 = (*n & 0x00FF000000000000) >> 40;
		uint64_t byte6 = (*n & 0x0000FF0000000000) >> 24;
		uint64_t byte5 = (*n & 0x000000FF00000000) >> 8;
		uint64_t byte4 = (*n & 0x00000000FF000000) << 8;
		uint64_t byte3 = (*n & 0x0000000000FF0000) << 24;
		uint64_t byte2 = (*n & 0x000000000000FF00) << 40;
		uint64_t byte1 = (*n & 0x00000000000000FF) << 56;

		*n = byte1 | byte2 | byte3 | byte4 | byte5 | byte6 | byte7 | byte8;
	}
}