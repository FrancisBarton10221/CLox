#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"
#include <stdint.h>

typedef enum {
    OP_CONSTANT,
    OP_CONSTANT_LONG,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

typedef struct {
    int count;
    int capacity;
    uint8_t* code;
    uint16_t currentLine;
    int lineArrSize;
    ValueArray constants;
    uint16_t* lines;
} Chunk;

void initChunk(Chunk* chunk);
void freeChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte, uint16_t line);
int addConstant(Chunk* chunk, Value value);

#endif
