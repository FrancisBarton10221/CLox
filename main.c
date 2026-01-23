#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]){
    initVM();

    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 163);
    writeChunk(&chunk, constant, 163);
    writeChunk(&chunk, OP_NEGATE, 163);
    
    writeChunk(&chunk, OP_RETURN, 163);
    
    
    interpret(&chunk);
    freeVM();
    freeChunk(&chunk);
    return 0;
    
}
