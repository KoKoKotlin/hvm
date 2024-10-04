#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#define dl_define(name, type) typedef struct {	\
		size_t capacity;		\
		size_t count;			\
		type* items;			\
	} name

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
	OP_CONSTANT,
	OP_CONSTANT_LONG,
	OP_ENUM_LEN,
} OpCode;

static char** opcode_names = (char*[]){
	"Return",
	"Constant",
	"ConstantLong",
};

typedef double Value; 

dl_define(ValueDl, Value);
dl_define(CodeDl, uint8_t);
dl_define(LineNumDl, size_t);

typedef struct {
	char* name;
	ValueDl values;
	CodeDl code;
	LineNumDl lines;
} Chunk;

uint32_t read32(uint8_t* bytes) {
	uint32_t result = 0;
	for (size_t i = 0; i < 4; i++)
		result |= bytes[i] << i*8;
	return result;
}

Chunk create_chunk(char* name) {
	Chunk c;
	ValueDl vdl;
	CodeDl code;
	LineNumDl lines;
	dl_create(32, sizeof(Value), vdl);
	dl_create(32, sizeof(uint8_t), code);
	dl_create(32, sizeof(size_t), lines);
	c.name = name;
	c.values = vdl;
	c.code = code;
	c.lines = lines;
       	return c;
}

size_t add_constant(Chunk* c, Value v) {
	dl_append(c->values, v);
	return c->values.count - 1;
}

void write_chunk(Chunk* c, uint8_t byte, size_t line) {
	dl_append(c->code, byte);
	dl_append(c->lines, line);
}

void write_chunk_32(Chunk* c, uint32_t v, size_t line) {
	for (size_t i = 0; i < 4; i++)
		write_chunk(c, (v >> i*8) & 0xF, line);	
}

void write_constant(Chunk* c, Value value, int line) {
	size_t const_idx = add_constant(c, value);
	if (const_idx < 256) {
		write_chunk(c, OP_CONSTANT, line);
		write_chunk(c, const_idx, line);
	} else {
		write_chunk(c, OP_CONSTANT_LONG, line);
		write_chunk_32(c, (uint32_t)const_idx, line);
	}
}

void free_chunk(Chunk* c) {
	dl_delete(c->values);
	dl_delete(c->code);
	dl_delete(c->lines);
}

size_t print_simple_inst_disas(const char* opcode_name, Chunk* chunk, size_t offset) {
	printf("%-16s\n", opcode_name);
	return offset + 1;
}

size_t print_constant_disas(Chunk* chunk, size_t offset) {
	size_t const_idx = chunk->code.items[offset + 1];
	printf("%-16s %4zu %lf\n", opcode_names[OP_CONSTANT], const_idx, chunk->values.items[const_idx]);
	return offset + 2;
}

size_t print_constant_long_disas(Chunk* chunk, size_t offset) {
	size_t const_idx = read32(&chunk->code.items[offset+1]);
	printf("%-16s %4zu %lf\n", opcode_names[OP_CONSTANT_LONG], const_idx, chunk->values.items[const_idx]);
	return offset + 5;
}

void print_chunk_disas(Chunk *c) {
	printf("====== %s ======\n", c->name);
	size_t offset = 0;
	CodeDl* code = &c->code;
     	while (offset < code->count) {
		uint8_t opcode = code->items[offset];
		printf("%04zu: %04zu Op: ", offset, c->lines.items[offset]);
		switch (opcode) {
		case OP_RETURN:
			offset = print_simple_inst_disas(opcode_names[OP_RETURN], c, offset);
			break;
		case OP_CONSTANT:
			offset = print_constant_disas(c, offset);
			break;
		case OP_CONSTANT_LONG:
			offset = print_constant_long_disas(c, offset);
			break;
		default:
			printf("Unknown instruction with opcode %u at offset %zu!", opcode, offset);
			offset += 1;
		}
	}
}

int main() {
	Chunk c = create_chunk("test chunk");
	write_chunk(&c, OP_RETURN, 1);
	for (size_t i = 0; i < 1000; i++) {
		write_constant(&c, i, i+1);
	}
	print_chunk_disas(&c);
	free_chunk(&c);
	return 0;
}
