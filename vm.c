#include <stdio.h>

#include "common.h"
#include "memory.h"
#include "debug.h"
#include "vm.h"
#include "value.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    vm.stack = GROW_ARRAY(Value, vm.stack, 0, 5);
    vm.stackSize = 5;
    printf("Stack size: %d\n", vm.stackSize);
    printf("Stack lower: %d\n", vm.stack);
    printf("Stack upper: %d\n", vm.stack + vm.stackSize * sizeof(Value));
    resetStack();
    printf("stackTop: %d\n", vm.stackTop);
}

void freeVM() {
    FREE_ARRAY(Value, vm.stack, vm.stackSize); 
    resetStack();
}

void push(Value value) {
    if (vm.stackTop == NULL) {
	printf("stackTop is null");
    }
    if (vm.stackTop == vm.stackSize + vm.stack) {
	printf("Stack bounds reached!\n");
	int stackTopOffset = vm.stackTop - vm.stack;
	vm.stack = GROW_ARRAY(Value, vm.stack, vm.stackSize, vm.stackSize * 2);
	vm.stackSize *= 2;
	vm.stackTop = stackTopOffset + vm.stack;
	printf("New size: %d\n", vm.stackSize);
    }
    *vm.stackTop = value;
    vm.stackTop++;
    printf("stackTop: %d\n", vm.stackTop);
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
	double b = pop(); \
	double a = pop(); \
	push(a op b); \
    } while (false)

    int lineCount = 0;
    int lineNumP = 0;
    int i = 0;
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
	printf("         ");
	for (Value* slot = vm.stack; slot < vm.stackTop; slot++) {
	    printf("[ ");
	    printValue(*slot);
	    printf(" ]");
	}
	printf("\n");
	// Update implementation to support the run length encoding implentation of line number storage. 
	disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code), &lineCount, &lineNumP);
#else
#warning "DEBUG_TRACE_EXECUTION IS NOT DEFINED!"
#endif
	uint8_t instruction;
	switch (instruction = READ_BYTE()) {
	    case OP_CONSTANT: {
		Value constant = READ_CONSTANT();
		push(constant);
		printValue(constant);
		printf("\n");
		break;
	    }
	    case OP_ADD:	BINARY_OP(+); break;
	    case OP_SUBTRACT:	BINARY_OP(-); break;
	    case OP_MULTIPLY:	BINARY_OP(*); break;
	    case OP_DIVIDE: 	BINARY_OP(/); break;
	    case OP_NEGATE: push(-pop()); break;
	    case OP_RETURN: {
		printValue(pop());
		printf("\n");
		return INTERPRET_OK;
	    }
	}
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

