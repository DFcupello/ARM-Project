#ifndef EMULATE
#define EMULATE
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void executeMultiplyInstruction(uint32_t word);
void executeBranchInstruction(uint32_t word);
void executeTranserInstruction(uint32_t word);
void executeDataInstruction(uint32_t word);
void printEndState(void);
#endif