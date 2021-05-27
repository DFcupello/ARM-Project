#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "inassembler.h"

int main(void) {
    char instruction[] = "mov r1,#1";
    uint32_t size = getTokenSize(instruction);  
    char *tokens[size];
    instructionTokenizer(instruction, tokens);
    for (int i = 0; i < size; i++) {
        printf("%s\n", tokens[i]);
    }
    return 0;
}

/* 
    Takes instruction mnemonic (add, sub, beq, ...) with '\0' char at the end.
    Returns pointer to new created sub array last two chars followed by '\0' char.
    In the absence of suffix, returns pointer to "al", which is equivalent to no suffix.
    Require to free the pointer using freeSuffix() or just free().
*/
char *getSuffix(char *mnemonic) {

    char * suffix = malloc(sizeof(char) * 3);
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
    Takes pointer created by getSuffix() function and frees it.
*/
void freeSuffix(char *suffix) {
    free(suffix);
}

/*
    Takes pointer to suffix
    Returns condition code corresponding to the suffix
    Assumes the suffix is supported by the implementation otehrwise returns 2^32 - 1.
*/
uint32_t getCondCodeFromSuffix(char *suffix) {

    char suffixAsStr[3] = {suffix[0], suffix[1], '\0'};

    char supportedSuffixes[7][3]   = {"al", "eq", "ne", "ge", "lt", "gt", "le"};
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
    Takes instruction mnemonics and address (&) of boolean flag variable.
    Returns the oppcode if the instruction is data processing, 2^32 - 1 otherwise.
    Modifies the value stored in the second argument address.
    Sets true if the mnemonic is in fact dataprocessing false otherwise.
*/
uint32_t getOpcodeFromMnemonic(char *mnemonic, bool *isItReallyDataProc) {

    char mneumonicAsStr[4] = {mnemonic[0], mnemonic[1], mnemonic[2], '\0'};

    char mneumonics[10][3] = {"and", "eor", "sub", "rsb", "add",
                              "orr", "mov", "tst", "teq", "cmp"};
    uint32_t opcodes[10] = {0, 1, 2, 3, 4, 0xc, 0xd, 8, 9, 0xa};

    int i = 0;
    while (strncmp(mneumonicAsStr, mneumonics[i], 3) != 0) {
        i++;
        if (i >= 10) {
            *isItReallyDataProc = false;
            return 0xffffffff; // case of unsupported suffix
        }
    }
    *isItReallyDataProc = true;
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
    *rotateAmount = i;
    return temp;
}


/*
Loops through the string instruction and checks how big the token array would be.
*/
uint32_t getTokenSize(char *instruction) {
    uint32_t size = 1;
    for (int i = 0; i < strlen(instruction); i++) {
        if (instruction[i] == ',' || instruction[i] == ' ') {
            size++;
        }
    }
    return size;
}
/*
Transforms the input paramater tokens into the tokens of instruction.
*/
void instructionTokenizer(char *instruction, char **tokens) {
    char *saveptr1 = instruction;
    char *token; 
    int i = 0;
    token = strtok_r(saveptr1, " ,", &saveptr1);
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok_r(saveptr1, " ,", &saveptr1);
    }
}

/*
Assembles data instructions in big-endian
*/
uint32_t assembleDataProcessing(char *instruction) {
    uint32_t size = getTokenSize(instruction);  
    char *tokens[size];
    instructionTokenizer(instruction, tokens);
    char *mnemonic = tokens[0];
    uint32_t (*func_ptr[3]) (uint32_t, char *, char **, uint32_t) = {assembleMov, 
                                                            assembleDataNoResults,
                                                            assembleDataResults};
    bool isData = false;
    uint32_t opcode = getOpcodeFromMnemonic(mnemonic, &isData);
    switch (opcode) { 
        case 13: return func_ptr[0](opcode, instruction, tokens, size);
        break;
        case 8:
        case 9:
        case 10: return func_ptr[1](opcode, instruction, tokens, size);
        break;
        default: return func_ptr[2](opcode, instruction, tokens, size);
    }
}

/*
Assembles the first main type: single operand assignment in big-endian
Parses: mov Rd, <expression> (token size is 3) or mov Rd, Rm <shift> (token size is not 3)
Assumes instruction is syntactically correct
*/
uint32_t assembleMov(uint32_t opcode, char *instruction, char ** tokens, uint32_t size) {
    uint32_t cond = 13 << 27;
    uint32_t regDest = registerCode(tokens[1]) << 11;
    uint32_t wordOpcode = opcode << 20;
    if (size == 3) { // expression
        uint32_t flagI = 1 << 24;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[2], &rotateAmount);
        return cond | flagI | wordOpcode | regDest | rotateAmount << 7 | expression;
    }
    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles the second main type: data instructions that compute results in big-endian
*/
uint32_t assembleDataResults(uint32_t opcode, char *instruction, char ** tokens, uint32_t size) {
    uint32_t cond = 13 << 27;
    uint32_t regDest = registerCode(tokens[1]) << 11;
    uint32_t regN    = registerCode(tokens[2]) << 15;
    uint32_t wordOpcode = opcode << 20;
    if (size == 4) { // expression
        uint32_t flagI = 1 << 24;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[2], &rotateAmount);
        return cond | flagI | wordOpcode | regN | regDest | rotateAmount << 7 | expression;
    }
    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles the second main type: data instructions that don't compute results in big-endian
*/
uint32_t assembleDataNoResults(uint32_t opcode, char *instruction, char ** tokens , uint32_t size) {
    uint32_t cond = 13 << 27;
    uint32_t regN = registerCode(tokens[1]) << 15;
    uint32_t wordOpcode = opcode << 20;
    uint32_t flagS = 1 << 19;
    if (size == 3) { // expression
        uint32_t flagI = 1 << 24;
        uint32_t rotateAmount = 0;
        uint32_t expression = expressionInBinary(tokens[2], &rotateAmount);
        return cond | flagI | wordOpcode | flagS | regN | rotateAmount << 7 | expression;
    }
    else { // optional: shifted register case
        return 0;
    }
}

/*
Assembles multiply instructions in big endian
Assumes instruction is syntactically correct
*/
uint32_t assembleMultiply(char *instruction) {
    uint32_t size = getTokenSize(instruction);  
    char *tokens[size];
    instructionTokenizer(instruction, tokens);
    uint32_t cond = 13 << 27;
    uint32_t regDest = registerCode(tokens[1]) << 15;
    uint32_t regM = registerCode(tokens[2]);
    uint32_t regS = registerCode(tokens[3]) << 7;
    uint32_t mulBits = 6 << 3;
    if (size == 4) { // mul
        return cond | regDest | regS | mulBits | regM;
    }
    uint32_t flagA = 1 << 20;
    uint32_t regN = registerCode(tokens[4]) << 11;
    return cond | flagA | regDest | regN | regS | mulBits | regM;
}

/*

*/
uint32_t assembleTransfer(char *instruction) {
    return 0;
}

/*

*/
uint32_t assembleBranch(char *instruction, uint32_t address) {
    return 0;
}

