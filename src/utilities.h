#ifndef UTILITIES
#define UTILITIES

enum InstType {Data, Mul, Branch, Transfer, HALT};

// Getting type
enum InstType getInstType(int8_t bytes[]);

//Data processing
int8_t getNibble(int8_t byte, int n);
int8_t get2Bits(int8_t byte, int n);
int32_t appendBytes(int8_t bytes[], int size, bool bigEndian);
int32_t rotateRight(int32_t offset, int rotateAmount);
int32_t arithmeticShiftRight(int32_t value, int shiftAmount);

// Obtaining register and offset from bytes array
int8_t getDestinationRegister(int8_t bytes[]);
int8_t getFirstOperandRegister(int8_t bytes[]);
int8_t getSecondOperandRegister(int8_t bytes[]);
int32_t getOffset(int8_t bytes[]);
int8_t getRegisterS(int8_t bytes[]);



#endif