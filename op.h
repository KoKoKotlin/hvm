#ifndef __OP_H_
#define __OP_H

typedef enum {
	OP_RETURN = 0,
	OP_CONSTANT,
	OP_CONSTANT_LONG,
	OP_NEG,
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
	OP_ENUM_LEN,
} OpCode;

static char** opcode_names = (char*[]){
	"Return",
	"Constant",
	"ConstantLong",
	"Negate",
	"Add",
	"Subtract",
	"Multiply",
	"Divide",
};

#endif // __OP_H_
