#ifndef UTILITIES
#define UTILITIES
enum InstType {Data, Mul, Branch, Transfer, HALT};

// Getting type
enum InstType getInstType(int32_t word);

//Data processing
void binaryLoader(FILE *fptr, char *file, bool bigEndian, int32_t *data, int size);
int32_t getNBits(int32_t word, int amount, int pos);
int32_t appendBytes(int32_t word, int size, bool bigEndian);
int32_t rotateRight(int32_t offset, int rotateAmount, bool* carry);
int32_t arithmeticShiftRight(int32_t value, int shiftAmount, bool* carry);
int32_t logicalShift(int32_t value, int shiftAmount, bool right, bool* carry);

// Obtaining register and offset from word
int32_t getDestinationRegister(int32_t word);
int32_t getFirstOperandRegister(int32_t word);
int32_t getSecondOperandRegister(int32_t word);
int32_t getOffset(int32_t word);
int32_t getRegisterS(int32_t word);
int32_t getShiftedRegister(int32_t word, int32_t registers[], bool *carry);

#endif