#ifndef UTILITIES
#define UTILITIES

enum InstType {Data, Mul, Branch, Transfer, HALT};

// Getting type
enum InstType getInstType(uint32_t word);

//Data processing
uint8_t getByte(uint32_t word, int n);
uint8_t getNibble(uint32_t word, int n);
uint8_t get2Bits(uint32_t word, int n);

// Obtaining register and offset from word
uint8_t getDestinationRegister(uint32_t word);
uint8_t getFirstOperandRegister(uint32_t word);
uint8_t getSecondOperandRegister(uint32_t word);
uint32_t getOffset(uint32_t word);

#endif