#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#define dl_create(cap, item_size, dl) do {				\
		(dl).count = 0;						\
		(dl).capacity = (cap);					\
		(dl).items = malloc((cap) * (item_size));		\
		memset((dl).items, 0, (dl).capacity * (item_size));	\
	} while(0) 

#define dl_check_cap(dl) do {						\
	if ((dl).count >= (dl).capacity) {				\
		(dl).capacity *= 2;					\
		(dl).items = realloc((dl).items, (dl).capacity * sizeof((dl).items[0])); \
	}								\
	} while(0)

#define dl_append(dl, item) do {			\
		dl_check_cap(dl);			\
		(dl).items[(dl).count] = (item);	\
		(dl).count += 1;			\
	} while(0)

#define dl_delete(dl) do {		\
		free((dl).items);	\
	} while(0)

typedef enum {
	OP_RETURN = 0,
	OP_ENUM_LEN,
} OpCode;

static char* opcode_text = {
	"Return",
};

typedef struct {
	int count;
	int capacity;
	uint8_t* items; // code fragments
} Chunk;

char* opcode_to_str(OpCode opcode) {
	assert(opcode < OP_ENUM_LEN && "OpCode not defined!");
	return opcode_text + opcode;
}

void print_inst_disas(Chunk *c, size_t offset) {
	uint8_t op_code = c->items[offset];
	assert(op_code < OP_ENUM_LEN && "Opcode is not defined!");
	printf("%zu:\tOpcode: %s", offset, opcode_to_str(op_code));
}

void print_chunk_disas(Chunk *c) {
	for (size_t i = 0; i < c->count; ++i) {
		print_inst_disas(c, i);
	}
}

int main() {
	Chunk c;
	dl_create(10, sizeof(uint8_t), c);
	dl_append(c, OP_RETURN);
	print_chunk_disas(&c);
	dl_delete(c);
	return 0;
}
