#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->linesCapacity = 0;
    chunk->linesCount = 0;
    chunk->lines = NULL;
    chunk->linesSpan = NULL;
    initValueArray(&chunk->constants);
}

void freeChunk(Chunk* chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->linesCapacity);
    FREE_ARRAY(int, chunk->linesSpan, chunk->linesCapacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

void addLine(Chunk* chunk, int line) {

    if (chunk->linesCount == 0 || line != chunk->lines[chunk->linesCount-1]) {
        // add a new line
        if (chunk->linesCapacity < chunk->linesCount + 1) {
            int oldCapacity = chunk->linesCapacity;
            chunk->linesCapacity = GROW_CAPACITY(oldCapacity);
            chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->linesCapacity);
            chunk->linesSpan = GROW_ARRAY(int, chunk->linesSpan, oldCapacity, chunk->linesCapacity);
        }

        chunk->lines[chunk->linesCount] = line;
        chunk->linesSpan[chunk->linesCount] = 1;
        chunk->linesCount++;

    } else {
        // just bump the span by one
        chunk->linesSpan[chunk->linesCount-1]++;
    }
}

void writeChunk(Chunk* chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    }
    
    chunk->code[chunk->count] = byte;
    chunk->count++;

    addLine(chunk, line);
}

int addConstant(Chunk* chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}

// Get the line of the instruction at index i
int getLine(Chunk* chunk, int index) {

    for (int i = 0; i < chunk->linesCount; i++) {
        if (chunk->linesSpan[i] > index) {
            return chunk->lines[i];
        } else {
            index -= chunk->linesSpan[i];
        }
    }

    exit(1);
}