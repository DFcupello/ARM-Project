
#ifndef INASSEMBLER
#define INASSEMBLER

#include "symtable.h"
#include "fifos.h"
// utility functions
bool mnemonicIsBlockDataTrans(char *mnemonic);
char *getBlockDataTransSuffix(char *mnemonic);
char *getCondSuffix(char *mnemonic);
uint32_t getCondCodeFromSuffix(char *suffix);
uint32_t getOpcodeFromMnemonic(char *mnemonic);
uint32_t getCondCodeFromTokens(char **tokens);
uint32_t registerCode(char *regToken);
uint32_t expressionInBinary(char *expression, uint32_t *rotateAmount);
uint32_t getShiftValue(char *shift);
bool containsChar(char *token, char searchedChar);

// String tokenizer
void tokenizer(char *instruction, uint32_t size, char **tokens);
uint32_t getTokenSize(char *instruction);

// main assembly functions
uint32_t assembleBlockDataTransfer(char **tokens, uint32_t size);
uint32_t assembleDataProcessing(char **tokens, uint32_t size);
uint32_t assembleMov(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleDataResults(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleDataNoResults(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleMultiply(char **tokens, uint32_t size);
uint32_t assembleTransfer(char **tokens, uint32_t size, int *numOfLines, uint32_t currAddress, ldrCollection_t *queue);
uint32_t assembleBranch(char **tokens, uint32_t size, symbolTable_t *symbolTable, uint32_t currAddress);
uint32_t assembleInstruction(char *instruction, symbolTable_t *symbolTable, int *numOfLines, uint32_t currAddress, ldrCollection_t *queue);

#endif