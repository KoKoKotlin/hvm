#include "hvm.h"

void reset_stack() {
	hvm.sp = hvm.stack;
}

void hvm_push(Value value) {
	assert(hvm.sp < hvm.stack + STACK_MAX && "Stack overflow");
	*hvm.sp = value;
	hvm.sp++;
}

Value hvm_pop() {
	assert(hvm.sp != hvm.stack && "Stack underflow!");
	return *(--hvm.sp);
}

void init_hvm() {
	reset_stack();
}

void free_hvm() {
	
}

HVMResult run_hvm() {
#define READ_BYTE() (*hvm.ip++)
#define READ_CONSTANT() (hvm.chunk->values.items[READ_BYTE()])
#define BINARY_OP(op) do {			\
		double left = hvm_pop();	\
		double right = hvm_pop();	\
		hvm_push(left op right);	\
	} while (0)

	for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
		printf("<<<<<< Stack trace >>>>>>\n");
		size_t i = 0;
		if (hvm.stack == hvm.sp) {
			printf("Stack emtpy.\n");
		} else {
			for (Value* slot = hvm.sp - 1; slot >= hvm.stack; slot--, i++) {
				printf("[ %zu: ", i);
				printValue(slot);
				printf(" ]\n");
			}
		}
		disas_instruction(hvm.chunk, (size_t)(hvm.ip - hvm.chunk->code.items));
#endif // DEBUG_TRACE_EXECUTION
		uint8_t instruction = READ_BYTE();
		switch (instruction) {
		case OP_RETURN:
			return INTERPRET_OK;
		case OP_CONSTANT: {
			Value constant = READ_CONSTANT();
			hvm_push(constant);
		} break;
		case OP_NEG:
			hvm_push(-hvm_pop());
			break;
		case OP_ADD:
			BINARY_OP(+);
			break;
		case OP_SUB:
			BINARY_OP(-);
			break;
		case OP_MUL:
			BINARY_OP(*);
			break;
		case OP_DIV:
			BINARY_OP(/);
			break;
		}
	}

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

HVMResult interpret(Chunk* chunk) {
	hvm.chunk = chunk;
	hvm.ip = chunk->code.items;
	return run_hvm();
}
