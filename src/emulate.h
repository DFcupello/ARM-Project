#ifndef EMULATE
#define EMULATE
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void executeMultiplyInstruction(uint32_t word);
bool executeBranchInstruction(uint32_t word);
void executeTranserInstruction(uint32_t word);
void loadStore(uint32_t regBase, uint32_t regSrcDst, bool load);
void executeDataInstruction(uint32_t word);
void executeInstruction(uint32_t word);
void printEndState(void);
#endif