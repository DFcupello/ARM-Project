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
#include "symtable.h"
#include "fifos.h"

void testAssembleInstruction(void)
{
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
    symbolTable_t *symbolTable = allocateInitialSymbolTable();
    addNewEntryToSymbolTable(symbolTable, "wait", 20);
    ldrCollection_t *queue = allocateInitialLdrCollection();
    int numOfInstrs = 4;
    testInt32(littleEndToBigEnd(assembleInstruction(branch1, symbolTable, &numOfInstrs, 12, NULL)), answers[0], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer1, symbolTable, &numOfInstrs, 0, queue)), answers[1], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer2, symbolTable, &numOfInstrs, 0, queue)), answers[2], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data1, symbolTable, &numOfInstrs, 0, NULL)), answers[3], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data2, symbolTable, &numOfInstrs, 0, NULL)), answers[4], name);
    testInt32(littleEndToBigEnd(assembleInstruction(data3, symbolTable, &numOfInstrs, 0, NULL)), answers[5], name);
    testInt32(littleEndToBigEnd(assembleInstruction(multiply1, symbolTable, &numOfInstrs, 0, NULL)), answers[6], name);
    testInt32(littleEndToBigEnd(assembleInstruction(multiply2, symbolTable, &numOfInstrs, 0, NULL)), answers[7], name);
    testInt32(littleEndToBigEnd(assembleInstruction(transfer3, symbolTable, &numOfInstrs, 0, NULL)), answers[8], name);
    freeSymbolTable(symbolTable);
    freeldrCollection(queue);
}
// Testing instructions
int main(void)
{
    testAssembleInstruction();
    return 0;
}