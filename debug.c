#include <stdio.h>
#include <stdlib.h>
#include "debug.h"
#include "value.h"

void disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    int lineCount = 0;
    int lineNumP = 0;
    for (int offset = 0; offset < chunk->count;) {
        offset = disassembleInstruction(chunk, offset, &lineCount, &lineNumP);
    }
}

static int getLine(Chunk* chunk, uint16_t offset) {
    uint16_t count = 0;
    int lineArrOffset = 0;
    while (1)
    {

        count += chunk->lines[lineArrOffset];
        lineArrOffset+=2;
        if (count >= offset) {
            return chunk->lines[lineArrOffset+1];
        }
    }
}

static int simpleInstruction(const char* name, int offset, int* lineCount) {
    printf("%s\n", name);
    *lineCount+=1;
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset, int* lineCount){
    uint8_t constant = chunk->code[offset+1];
    printf("%-16s %4d '", name, constant);
    printValue(chunk->constants.values[constant]);
    printf("'\n");
    *lineCount += 2;
    return offset + 2;
}

int disassembleInstruction(Chunk* chunk, int offset, int* lineCount, int* lineNumP) {
    printf("%04d ", offset);

    if (*lineCount == 0) {
        printf("%4u ", chunk->lines[*lineNumP+1]);
    }
    else if (chunk->lines[*lineNumP] > *lineCount) {
        printf("   | ");
    }
    else {
        *lineNumP += 2;
        *lineCount = 0;
        printf("%4u ", chunk->lines[*lineNumP+1]);
    }

    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", chunk, offset, lineCount);
        case OP_RETURN:
            return simpleInstruction("OP_RETURN", offset, lineCount);
        default:
            printf("Unkown opcode %d \n", instruction);
            printf("%04d", getLine(chunk, offset));
            exit(EXIT_FAILURE);
    }
}

