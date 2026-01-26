#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]){
    initVM();

    Chunk chunk;
    initChunk(&chunk); 
    for (int i = 0; i < 25; i++) {
	int constant = addConstant(&chunk, 1.2);
	writeChunk(&chunk, OP_CONSTANT, 12);
	writeChunk(&chunk, constant, 12);
    }

    writeChunk(&chunk, OP_RETURN, 123);    
    
    interpret(&chunk);
    freeVM();
    freeChunk(&chunk);


    return 0;
    
}
