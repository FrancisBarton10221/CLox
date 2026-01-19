#include <stdlib.h>
#include <stdio.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk){
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->currentLine = 0;
    chunk->lineArrSize = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(uint8_t, chunk->lines, chunk->lineArrSize);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void writeChunk(Chunk* chunk, uint8_t byte, uint16_t line){
    
    if (chunk->currentLine != line) {
        chunk->currentLine = line;
        chunk->lines = GROW_ARRAY(uint16_t, chunk->lines, chunk->lineArrSize, (chunk->lineArrSize)+2);
        chunk->lineArrSize += 2;
        chunk->lines[chunk->lineArrSize-1] = chunk->currentLine;
        chunk->lines[chunk->lineArrSize-2] = 0;
    }
    if (chunk->capacity < chunk->count + 1 ){
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
            oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->lineArrSize-2] += 1;
    chunk->count++;
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count-1;
}