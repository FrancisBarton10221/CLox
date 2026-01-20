#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char* argv[]){
    Chunk chunk;
    initChunk(&chunk);

    int constant = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 163);
    writeChunk(&chunk, constant, 163);

    writeChunk(&chunk, OP_RETURN, 163);
    
    writeChunk(&chunk, 4, 300);
    writeChunk(&chunk, OP_RETURN, 300);
    
    disassembleChunk(&chunk, "test chunk");
    freeChunk(&chunk);
    return 0;
    
}