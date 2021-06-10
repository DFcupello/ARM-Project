#ifndef EMULATE
#define EMULATE

#include <stdint.h>
#include <stdbool.h>
/* 
The size of memory in 32-bit format, and the number of registers.
*/

#define REG_SIZE 17
#define MEM_SIZE 16384
#define CPSR 16
#define PC 15

void printEndState(uint32_t data[], uint32_t registers[]);
bool emulateInstruction(uint32_t data[], uint32_t registers[], uint32_t bigData[], uint32_t pipeline[]);
void emulate(uint32_t data[], uint32_t registers[], int instrCount);

#endif