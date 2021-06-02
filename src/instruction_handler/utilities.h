#ifndef UTILITIES
#define UTILITIES

//Data processing
void binaryLoader(FILE *fptr, char *file, uint32_t *data, int size, int *instrCount);
uint32_t getNBits(uint32_t word, int amount, int pos);
uint32_t rotateRight(uint32_t offset, uint32_t rotateAmount, bool *carry);
uint32_t arithmeticShiftRight(uint32_t value, uint32_t shiftAmount, bool *carry);
uint32_t logicalShift(uint32_t value, uint32_t shiftAmount, bool right, bool *carry);

// Pipeline
void pushPipeline(uint32_t pipeline[], uint32_t fetchWord);
void clearPipeline(uint32_t pipeline[]);
#endif