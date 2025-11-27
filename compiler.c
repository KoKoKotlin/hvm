#include "compiler.h"

void compile(const char* source) {
	init_scanner(source);

	int line = -1;
	for (;;) {
		Token token = scan_token();
		#ifdef DEBUG
		if (token.line != line) {
			printf("%4d ", token.line);
			line = token.line;
		} else {
			printf("    | ");
		}
		printf(TOKEN_FMT"\n", TOKEN_VAL(token));
		printf("%.*s\n", token.symbols.length, token.symbols.start);
                #endif
		if (token.type == TOKEN_EOF) break;
	}
}
