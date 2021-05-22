#ifndef EMULATE
#define EMULATE
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

void executeMultiplyInstruction(int32_t word);
void executeBranchInstruction(int32_t word);
void executeTranserInstruction(int32_t word);
void executeDataInstruction(int32_t word);
void printEndState(void);
#endif