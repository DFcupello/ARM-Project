#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "inassembler.h"
#include "inhandler.h"
#include "testsuite.h"


void testAssembleInstruction(void) {
    char name[] = "Assemble Instruction test";
    char branch1[] = "bne wait:";
    char transfer1[] = "ldr r2,=0x20200020";
    char transfer2[] = "str r1,[r2],r4";
    char transfer3[] = "str r1,[r0,#3]";
    char data1[] = "mov r1,#1";
    char data2[] = "add r2,r1,#2";
    char data3[] = "orr r2,r1,#0xAB";
    char multiply1[] = "mul r3,r1,r2";
    char multiply2[] = "mla r3,r1,r2,r4";
    uint32_t answers[9] = {0x0000001a, 0x08209fe5, 0x041082e6, 0x0110a0e3, 0x022081e2, 0xab2081e3, 0x910203e0, 0x914223e0, 0x031080e5};
    testInt32(littleEndToBigEnd(assembleInstruction(branch1)),answers[0], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer1)),answers[1], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer2)),answers[2], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data1)), answers[3], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data2)),answers[4], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data3)),answers[5], name);
    testInt32(littleEndToBigEnd(assembleInstruction(multiply1)),answers[6], name);
    testInt32(littleEndToBigEnd(assembleInstruction(multiply2)),answers[7], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer3)),answers[8], name);

    
}
int main(void) {
    char instruction[] = "str r1,[r0,#3]";
    uint32_t size = getTokenSize(instruction);  
    printf("%d\n", size);
    printf("result: 0x%08x\n", assembleInstruction(instruction));
    printf("expected: 0x%08x\n", bigEndToLittleEnd(0x031080e5));
    printf("wow: %ld\n", strtol("12]", NULL, 0));
    testAssembleInstruction();
    return 0;
}