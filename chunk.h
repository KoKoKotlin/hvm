#ifndef __CHUNK_H_
#define __CHUNK_H_

#include "dl.h"
#include "value.h"
#include "op.h"
#include "utils.h"
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

dl_define(ValueDl, Value);
dl_define(CodeDl, uint8_t);
dl_define(LineNumDl, size_t);

typedef struct {
	char* name;
	ValueDl values;
	CodeDl code;
	LineNumDl lines;
} Chunk;

size_t disas_simple(const char* opcode_name, Chunk* chunk, size_t offset);
size_t disas_constant(Chunk* chunk, size_t offset);
size_t disas_constant_long(Chunk* chunk, size_t offset);
size_t disas_instruction(Chunk* chunk, size_t offset);
Chunk create_chunk(char* name);
size_t add_constant(Chunk* c, Value v);
void write_chunk(Chunk* c, uint8_t byte, size_t line);
void write_chunk_32(Chunk* c, uint32_t v, size_t line);
void write_constant(Chunk* c, Value value, int line);
void free_chunk(Chunk* c);
void print_chunk_disas(Chunk *chunk);

#endif // __CHUNK_H_
