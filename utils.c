#include "utils.h"

uint32_t read32(uint8_t* bytes) {
	uint32_t result = 0;
	for (size_t i = 0; i < 4; i++)
		result |= bytes[i] << i*8;
	return result;
}
