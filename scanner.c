#include "scanner.h"

void init_scanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
	scanner.line = 1;
}
