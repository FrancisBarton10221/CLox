#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

VM vm;

static void resetStack() {
    vm.stackTop = vm.stack;
}

void initVM() {
    resetStack();
}

void freeVM() {
    resetStack();
}

void push(Value value) {
    if (vm.stackTop == NULL) {
	printf("stackTop is null");
    }
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    
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
}

InterpretResult interpret(Chunk* chunk) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code;
    return run();
}

