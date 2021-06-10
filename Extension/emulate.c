#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "inhandler.h"
#include "utilities.h"
#include "execute.h"
#include "emulate.h"


/*
Address is calculated, since it is byte-addressible, as multiples of 4. So
if we are trying to access data[i], the address is i*4. 
least significant byte (LSB) of data[i] : address  = i*4
most significant byte (MSB) of data[i]  : address  = i*4 + 3
It is in little endian format.
*/

// Prints the end state of the registers and non-zero memory.
void printEndState(uint32_t data[], uint32_t registers[]) {
    printf("Registers:\n");
    for (int i = 0; i < 13; i++) {
        printf("$%-2d : %10d (0x%08x)\n", i, registers[i], registers[i]);
    }
    printf("PC  : %10d (0x%08x)\n", registers[PC], registers[PC]);
    printf("CPSR: %10d (0x%08x)\n", registers[CPSR], registers[CPSR]);
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEM_SIZE; i++) {
        if (data[i] != 0) {
            printf("0x%08x: 0x%08x\n", i * 4, data[i]);
        }
    }
}

bool emulateInstruction(uint32_t data[], uint32_t registers[], uint32_t bigData[], uint32_t pipeline[]) {
    bool changePC = true;
    if (pipeline[1] != -1) {
        uint32_t executeWord = pipeline[1];
        if (executeWord == 0) {
            return false;
        }
        if (instrIsBranch(executeWord)) {
            if (executeBranchInstruction(executeWord, data, registers, BRANCH)) {
                clearPipeline(pipeline);
                changePC = false;
            }
        }
        else {
            executeInstruction(executeWord, data, registers);
        }
    }

    if (changePC) {
        pushPipeline(pipeline, bigData[registers[PC] / 4]);
        registers[PC] = registers[PC] + 4;
    }
    return true;
}

void emulate(uint32_t data[], uint32_t registers[], int instrCount) {
    // Used in order to convert only instructions to big endian for executing functions
    uint32_t bigData[instrCount + 1];
    for (int i = 0; i < instrCount; i++) {
    bigData[i] = littleEndToBigEnd(data[i]);
    }
    // The halt instruction
    bigData[instrCount] = 0;
    //fetch - decode - execute
    uint32_t pipeline[] = {-1, -1};
    while (emulateInstruction(data, registers, bigData, pipeline));
}
#ifdef MAIN_EMULATE
int main(int argc, char *argv[]) {
    uint32_t data[MEM_SIZE] = {0};
    uint32_t registers[REG_SIZE] = {0};
    int instrCount = 0;
    char *file;
    FILE *fptr = NULL;
    if (argv[1] != NULL) {
        file = argv[1];
        fptr = fopen(file, "rb");
    }
    if (fptr != NULL) {
        binaryLoader(fptr, file, data, MEM_SIZE, &instrCount);
        emulate(data, registers, instrCount);
        printEndState(data, registers);
        return EXIT_SUCCESS;
    }
    printf("No Arguments or file does not exist!\n");
    return 0;
}
#endif


