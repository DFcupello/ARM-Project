#ifndef UTILITIES
#define UTILITIES
enum InstType {Data, Mul, Branch, Transfer, HALT};

// Getting type
enum InstType getInstType(uint32_t word);

//Data processing
void binaryLoader(FILE *fptr, char *file, uint32_t *data, int size);
uint32_t getNBits(uint32_t word, int amount, int pos);
uint32_t rotateRight(uint32_t offset, uint32_t rotateAmount, bool* carry);
uint32_t arithmeticShiftRight(uint32_t value, uint32_t shiftAmount, bool* carry);
uint32_t logicalShift(uint32_t value, uint32_t shiftAmount, bool right, bool* carry);

// Obtaining register and offset from word
uint32_t getDestinationRegister(uint32_t word);
uint32_t getFirstOperandRegister(uint32_t word);
uint32_t getSecondOperandRegister(uint32_t word);
uint32_t getOffset(uint32_t word);
uint32_t getRegisterS(uint32_t word);
uint32_t getShiftedRegister(uint32_t word, uint32_t registers[], bool *carry);

// Pipeline 
void pushPipeline(uint32_t pipeline[], uint32_t fetchWord);
bool isPipelineFull(uint32_t pipeline[]);
void clearPipeline(uint32_t pipeline[]);
#endif