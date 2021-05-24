#ifndef EMULATE
#define EMULATE
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void executeMultiplyInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
bool executeBranchInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void executeTranserInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void loadStore(uint32_t regBase, uint32_t regSrcDst, bool load, uint32_t data[], uint32_t registers[]);
void executeDataInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void executeInstruction(uint32_t word, uint32_t data[], uint32_t registers[]);
void printEndState(uint32_t data[], uint32_t registers[]);

#endif