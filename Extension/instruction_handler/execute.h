#ifndef EXECUTE
#define EXECUTE


void executeMultiplyInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
bool executeBranchInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void executeTransferInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void loadStore(uint32_t regB, uint32_t regSrcDst, bool load, uint32_t data[], uint32_t registers[]);
void executeDataInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type);
void executeBlockDataTransferInstruction(uint32_t instr, uint32_t data[], uint32_t registers[]);
void executeInstruction(uint32_t word, uint32_t data[], uint32_t registers[]);

#endif
