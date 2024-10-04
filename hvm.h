#ifndef __HVM_H_
#define __HVM_H_

#include "chunk.h"
#include <stdlib.h>
#include <assert.h>

#define DEBUG_TRACE_EXECUTION
#define STACK_MAX 2056

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR,
} HVMResult;

typedef struct {
	Chunk* chunk;
	uint8_t* ip;
	Value stack[STACK_MAX];
	Value* sp;
} HVM;

// global vm object that keeps track of te state of execution of the bytecode provided
static HVM hvm;

void reset_stack();
void hvm_push(Value value);
Value hvm_pop();
void init_hvm();
void free_hvm();
HVMResult run_hvm();
HVMResult interpret(Chunk* chunk);

#endif // __HVM_H_"
