


// utility functions
char *getSuffix(char *mnemonic);
void freeSuffix(char *suffix);
uint32_t getCondCodeFromSuffix(char *suffix);
uint32_t getOpcodeFromMnemonic(char *mnemonic);
uint32_t getCondCodeFromTokens(char **tokens);
uint32_t registerCode(char *regToken);
uint32_t expressionInBinary(char *expression, uint32_t *rotateAmount);

// String tokenizer
void instructionTokenizer(char *instruction, uint32_t size, char **tokens);
uint32_t getTokenSize(char *instruction);

// main assembly functions
uint32_t assembleDataProcessing(char **tokens, uint32_t size);
uint32_t assembleMov(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleDataResults(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleDataNoResults(uint32_t opcode, char **tokens, uint32_t size);
uint32_t assembleMultiply(char **tokens, uint32_t size);
uint32_t assembleTransfer(char **tokens, uint32_t size);
uint32_t assembleBranch(char **tokens, uint32_t size);
uint32_t assembleInstruction(char *instruction);
