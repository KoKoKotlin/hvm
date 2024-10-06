#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

#include "dl.h"
#include "chunk.h"
#include "hvm.h"
#include "compiler.h"

HVMResult interpret_code(const char* source) {
	compile(source);
	return INTERPRET_OK;
}

static void repl() {
	char line[1024];

	for(;;) {
		printf("> ");

		if(!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		interpret_code(line);
	}
}

static char* read_file(const char* path) {
	FILE* file = fopen(path, "r");
	if (!file) goto FILE_READ_ERROR;

	if (!fseek(file, 0, SEEK_END)) goto FILE_READ_ERROR;
	long file_size = ftell(file);
	rewind(file);

	char* buf = malloc(file_size * sizeof(char));
	memset(buf, 0, file_size * sizeof(char));
	if (sizeof(char) * file_size != fread(buf, sizeof(char), file_size, file)) goto FILE_READ_ERROR;
	return buf;
	
 FILE_READ_ERROR:
	fprintf(stderr, "Couldn't read file %s! Reason: %s", path, strerror(errno));
	exit(errno);
}

static void run_file(const char* path) {
	char* source = read_file(path);
	HVMResult result = interpret_code(source);
	free(source);

	if (result == INTERPRET_COMPILE_ERROR) exit(65);
	if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

int main(int argc, char** argv) {
	init_hvm();

	if (argc == 1) {
		repl();
	} else if (argc == 2) {
		run_file(argv[1]);
	} else {
		fprintf(stderr, "Usage: hvm [path]\n");
		exit(-1);
	}
	
	free_hvm();	
	return 0;
}
