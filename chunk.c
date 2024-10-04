#include "chunk.h"

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

void print_chunk_disas(Chunk *chunk) {
	printf("====== %s ======\n", chunk->name);
	size_t offset = 0;
     	while (offset < chunk->code.count) disas_instruction(chunk, offset);
}

size_t disas_simple(const char* opcode_name, Chunk* chunk, size_t offset) {
	printf("%-16s\n", opcode_name);
	return offset + 1;
}

size_t disas_constant(Chunk* chunk, size_t offset) {
	size_t const_idx = chunk->code.items[offset + 1];
	printf("%-16s %4zu ", opcode_names[OP_CONSTANT], const_idx);
	printValue(&chunk->values.items[const_idx]);
	printf("\n");
	return offset + 2;
}

size_t disas_constant_long(Chunk* chunk, size_t offset) {
	size_t const_idx = read32(&chunk->code.items[offset+1]);
	printf("%zu\n", const_idx);
	printf("%-16s %4zu", opcode_names[OP_CONSTANT_LONG], const_idx);
	printValue(&chunk->values.items[const_idx]);
	printf("\n");
	return offset + 5;
}

size_t disas_instruction(Chunk* chunk, size_t offset) {
	uint8_t opcode = chunk->code.items[offset];
	printf("%04zu: %04zu Op: ", offset, chunk->lines.items[offset]);
	switch (opcode) {
	case OP_RETURN:
	case OP_NEG:
	case OP_ADD:
	case OP_SUB:
	case OP_MUL:
	case OP_DIV:
		offset = disas_simple(opcode_names[opcode], chunk, offset);
		break;
	case OP_CONSTANT:
		offset = disas_constant(chunk, offset);
		break;
	case OP_CONSTANT_LONG:
		offset = disas_constant_long(chunk, offset);
		break;
	default:
		printf("Unknown instruction with opcode %u at offset %zu!", opcode, offset);
		offset += 1;
        }
	return offset;
}
