#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <assert.h>

#include "dl.h"
#include "chunk.h"
#include "hvm.h"

int main() {
	init_hvm();
	Chunk c = create_chunk("test chunk");
	write_constant(&c, 34.0, 1);
	write_constant(&c, 35.0, 2);
	write_chunk(&c, OP_ADD, 3);
	write_constant(&c, 20.0, 4);
	write_chunk(&c, OP_SUB, 5);
	write_constant(&c, 15.0, 6);
	write_chunk(&c, OP_DIV, 7);
	write_chunk(&c, OP_RETURN, 8);
	// print_chunk_disas(&c);
	interpret(&c);
	free_chunk(&c);
	free_hvm();	
	return 0;
}
