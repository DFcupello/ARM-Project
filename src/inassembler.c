#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <ctype.h>
#include "inassembler.h"
#include "inhandler.h"

#define MAX_LINE_LENGTH 511

// int main(void) {
//     char instruction[] = "teqeq r0,r0";
//     uint32_t size = getTokenSize(instruction);  
//     printf("%d\n", size);
//     printf("result: 0x%08x\n", assembleInstruction(instruction));
//     printf("expected: 0x%08x\n", bigEndToLittleEnd(0x020011e1));
//     printf("wow: %ld\n", strtol("12]", NULL, 0));
//     return 0;
// }

/* 
    Takes instruction mnemonic (add, sub, beq, ...) with '\0' char at the end.
    Returns pointer to new created sub array last two chars followed by '\0' char.
    In the absence of suffix, returns pointer to "al", which is equivalent to no suffix.
    Require to free the pointer using freeSuffix() or just free().
*/
char *getSuffix(char *mnemonic) {

    char *suffix = malloc(sizeof(char) * 3);
    suffix[2] = '\0';
    int length = strlen(mnemonic);

    if (length == 5) {                              // non branch
        suffix[0] = mnemonic[3]; 
        suffix[1] = mnemonic[4];
    } else if (length == 3 && mnemonic[0] == 'b') { // branch
        suffix[0] = mnemonic[1];
        suffix[1] = mnemonic[2];
    } else {                                        // no suffix
        suffix[0] = 'a';
        suffix[1] = 'l';
    }
    return suffix;
}

/*
    Takes pointer to suffix
    Returns condition code corresponding to the suffix
    Assumes the suffix is supported by the implementation otehrwise returns 2^32 - 1.
*/
uint32_t getCondCodeFromSuffix(char *suffix) {

    char suffixAsStr[2] = {suffix[0], suffix[1]};

    char supportedSuffixes[7][2]   = {"al", "eq", "ne", "ge", "lt", "gt", "le"};
    uint32_t correspondingCodes[7] = {0x0e, 0x00, 0x01, 0x0a, 0x0b, 0x0c, 0x0d};
    int i = 0;
    while (strncmp(suffixAsStr, supportedSuffixes[i], 2) != 0) {
        i++;
        if (i >= 7) {
            return 0xffffffff; // case of unsupported suffix
        }
    }
    return correspondingCodes[i];
}

/*
	Takes the instruction as array of tokens.
	Retuns 32-bit value representing condcode in 4 MOST significant bits 
	Manages all suffix related work in one function, including shifting. 
*/
uint32_t getCondCodeFromTokens(char **tokens) {

	char *suffix = getSuffix(tokens[0]);
    uint32_t cond = getCondCodeFromSuffix(suffix) << 28;
    free(suffix);
    return cond;
}

/*
    Takes instruction mnemonics and address (&) of boolean flag variable.
    Returns the oppcode if the instruction is data processing, 2^32 - 1 otherwise.
    Includes "lsl", "tst", "ldr" as they can be treated as "mov" in some cases.
*/
uint32_t getOpcodeFromMnemonic(char *mnemonic) {

    char mnemonicAsStr[3] = {mnemonic[0], mnemonic[1], mnemonic[2]};

    char mnemonics[12][3] = {"and", "eor", "sub", "rsb", "add",
                              "orr", "mov", "lsl", "ldr", "tst", "teq", "cmp"};
    uint32_t opcodes[12] = {0, 1, 2, 3, 4, 0xc, 0xd, 0xd, 0xd, 8, 9, 0xa};

    int i = 0;
    while (strncmp(mnemonicAsStr, mnemonics[i], 3) != 0) {
        i++;
        if (i >= 12) {
            return 0xffffffff; // case of unsupported suffix
        }
    }
    return opcodes[i];
}

/*
  Take register token ("r0, r1, ..., r16")
  Returns corresponding register number
*/
uint32_t registerCode(char *regToken) { 
    char *numPtr = regToken + sizeof(char);
    uint32_t res;
    if (numPtr[1] == '\0') {
        res = numPtr[0] - '0';
    } else {
        res = 10 + (numPtr[1] - '0'); 
    }
    return res;
}

/*
Tries to generate an 8bit immediate value by rotating left in intervals of 2
returns an error if it cannot be represented.
PRE: expression is NOT NULL
*/
uint32_t expressionInBinary(char *expression, uint32_t *rotateAmount) {
    uint32_t temp = strtol(expression + 1, NULL, 0);
    uint32_t rotateMask = 1 << 31;
    uint32_t mask = 0xFFFFFF00;
    uint32_t apply = 1;
    int i = 0;
    while (i < 30) {
        if (i % 2 == 0 && ((temp & mask) == 0)) {
            break;
        }
        temp = (temp << 1) | ((temp & rotateMask) == 1 ? apply : 0);
        i++;
    }
    if (i == 30) {
        printf("ERROR!");
        return 0;
    }
    *rotateAmount = i / 2;
    return temp;
}


/*
Loops through the string instruction and checks how big the token array would be.
Assumes assembly is syntactically correct
*/

uint32_t getTokenSize(char *instruction) {
    uint32_t size = 1;
    int i = 0;
    while (i < strlen(instruction) - 1) {
        if (instruction[i] == ',' || instruction[i] == ' ') {
            while (instruction[i] == ',' || instruction[i] == ' ') {
                i++;
            }
            size++;
        }
        i++;
    }
    return size;
}
/*
Transforms the input paramater tokens into the tokens of instruction.
*/
void instructionTokenizer(char *instruction, uint32_t size, char **tokens) {
    char *saveptr1 = instruction;
    for (int i = 0; i < size; i++) {
        tokens[i] = strtok_r(saveptr1, " ,\n", &saveptr1);
    }
}

/*
Assembles data instructions in big-endian
*/
uint32_t assembleDataProcessing(char **tokens, uint32_t size) {
    uint32_t opcode = getOpcodeFromMnemonic(tokens[0]);
    switch (opcode) { 
        case 13: return assembleMov(opcode, tokens, size);
        break;
        case 8:
        case 9:
        case 10: return assembleDataNoResults(opcode, tokens, size);
        break;
        default: return assembleDataResults(opcode, tokens, size);
    }
    
}

/*
Assembles the first main type: single operand assignment in big-endian
Parses: mov Rd, <expression> (token size is 3) or mov Rd, Rm <shift> (token size is not 3)
Assumes instruction is syntactically correct
*/
uint32_t assembleMov(uint32_t opcode, char **tokens, uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    // uint32_t cond = 14 << 28; // 1110
    uint32_t regDest = registerCode(tokens[1]) << 12;
    uint32_t wordOpcode = opcode << 21;
    if (strncmp(tokens[0], "lsl", 3) == 0) { // special case: lsl  
        uint32_t regM =  registerCode(tokens[1]);
        uint32_t shiftAmount = strtol(tokens[2] + 1, NULL, 0) << 7;
        return cond | wordOpcode | regDest | regM | shiftAmount;
    }
    else if (size == 3) { // expression
        uint32_t flagI = 1 << 25;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[2], &rotateAmount);
        return cond | flagI | wordOpcode | regDest | rotateAmount << 8 | expression;
    }

    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles the second main type: data instructions that compute results in big-endian
*/
uint32_t assembleDataResults(uint32_t opcode, char ** tokens, uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    uint32_t regDest = registerCode(tokens[1]) << 12;
    uint32_t regN    = registerCode(tokens[2]) << 16;
    uint32_t wordOpcode = opcode << 21;
    if (size == 4) { // expression
        uint32_t flagI = (strncmp(tokens[3], "#", 1) == 0) ? 1 << 25 : 0;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[3], &rotateAmount);
        return cond | flagI | wordOpcode | regN | regDest | rotateAmount << 8 | expression;
    }
    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles the second main type: data instructions that don't compute results in big-endian
*/
uint32_t assembleDataNoResults(uint32_t opcode, char **tokens , uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    uint32_t regN = registerCode(tokens[1]) << 16;
    uint32_t wordOpcode = opcode << 21;
    uint32_t flagS = 1 << 20;
    if (size == 3) { // expression
        uint32_t flagI = (tokens[2][0] == '#') ? 1 << 25 : 0;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[2], &rotateAmount);
        return cond | flagI | wordOpcode | flagS | regN | rotateAmount << 8 | expression;
    }
    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles multiply instructions in big endian
Assumes instruction is syntactically correct
*/
uint32_t assembleMultiply(char **tokens, uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    uint32_t regDest = registerCode(tokens[1]) << 16;
    uint32_t regM = registerCode(tokens[2]);
    uint32_t regS = registerCode(tokens[3]) << 8;
    uint32_t mulBits = 9 << 4;
    if (size == 4) { // mul
        return cond | regDest | regS | mulBits | regM;
    }
    uint32_t flagA = 1 << 21;
    uint32_t regN = registerCode(tokens[4]) << 12;
    return cond | flagA | regDest | regN | regS | mulBits | regM;
}

/*
Assembles transfer instructions in big endian.
Assumes instruction is syntactically correct
(Don't know if it fully works yet and it is missing a case.)
*/
uint32_t assembleTransfer(char **tokens, uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    const uint32_t regDest = registerCode(tokens[1]) << 12; 
    const uint32_t unnecessaryBits = 1 << 26;
    uint32_t regBase; 
    // Special case Where loading from an immediate value
    if (tokens[2][0] == '=') {
        assert(tokens[0][0] == 'l');
        if (strtol(tokens[2] + 1, NULL, 0) <= 0xFF) {
            return assembleDataProcessing(tokens, size);
        }
        else {
            const uint32_t currentAddress = 0; // needs address can get from 2nd pass loop
            const uint32_t newAddress = 0; // needs address (num of instruction count would be enough)
            const uint32_t offset = newAddress - currentAddress - 8;
            // Need to put newAddress: tokens[2] at the end of assembler file
            regBase = 15 << 16; 
            const uint32_t flags = 0x05900000;
            return cond | flags | regBase | regDest | offset;
        }
    }
    const uint32_t flagL = (tokens[0][0] == 'l') ? 1 << 20 : 0;
    uint32_t flagP;
    uint32_t offset; 
    uint32_t regM; 
    uint32_t flagU;
    uint32_t flagI;
    regBase = strtol(tokens[2] + 2, NULL, 0) << 16;
    if (size == 3) {
        flagP = 1 << 24;
        return cond | unnecessaryBits | flagP | flagL | regBase | regDest;
    }
    else if (size == 4) {
        flagP = (tokens[2][3] == ']') ? 0 : 1 << 24;
        if (tokens[3][0] == '#') {
            offset = strtol(tokens[3] + 1, NULL, 0);
            flagU = (tokens[3][1] == '-') ? 0 : 1 << 23;
            return cond | unnecessaryBits | flagP | flagU | flagL | regBase | regDest | offset;
        }
        flagI = 1 << 25;
        regM = strtol(tokens[3] + 1, NULL, 0);
        flagU = (tokens[3][0] == '-') ? 0 : 1 << 23;
        return cond | unnecessaryBits | flagI | flagP | flagU | flagL | regBase | regDest | regM;
    }
    else if (size == 5) { // optional case
        return 0;
    }
    return 0;
}

/*
Assembles branch instructions in big endian.
Assumes instruction is syntactically correct
Missing symboltable implementation
*/
uint32_t assembleBranch(char **tokens, uint32_t size) {
    uint32_t cond = getCondCodeFromTokens(tokens);
    uint32_t unnecessaryBits = 5 << 25;
    uint32_t address;
    if (isalpha(tokens[1][0])) {
        // Label case
        // use symboltable to get address
        address = 0;
    } 
    else {
        address = strtol(tokens[1], NULL, 0) >> 2;
    }
    return cond | unnecessaryBits | address;
}

uint32_t assembleInstruction(char *instruction) {
    uint32_t binary;
    uint32_t size = getTokenSize(instruction);
    char *tokens[size];
    instructionTokenizer(instruction, size, tokens);
    if (strncmp(tokens[0], "mul", 3) == 0 || strncmp(tokens[0], "mla", 3) == 0) {
        binary = assembleMultiply(tokens, size);
    }
    else if (strncmp(tokens[0], "str", 3) == 0  || strncmp(tokens[0], "ldr", 3) == 0 ) {
        binary = assembleTransfer(tokens, size);
    }
    else if (tokens[0][0] == 'b') {
        binary = assembleBranch(tokens, size);
    }
    else {
        binary = assembleDataProcessing(tokens, size);
    }
    return binary;
}
