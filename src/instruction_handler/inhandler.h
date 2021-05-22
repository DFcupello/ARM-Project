#ifndef INSTRUCTION_HANDLER
#define INSTRUCTION_HANDLER

// Endian convertion
int32_t littleEndToBigEnd(int32_t origInstr);
int32_t bigEndToLittleEnd(int32_t origInstr);

// Instruction type determination
bool instrIsDataProc(int32_t instr);
bool instrIsMultiply(int32_t instr);
bool instrIsSingleDataTrans(int32_t instr);
bool instrIsBranch(int32_t instr);

// Getting instruction flags and opcode;
bool isIFlagSet(int32_t instr);
bool isSFlagSet(int32_t instr);
bool isAFlagSet(int32_t instr);
bool isPFlagSet(int32_t instr);
bool isUFlagSet(int32_t instr);
bool isLFlagSet(int32_t instr);
int32_t opcode(int32_t instr);

// Condition code and CPSR register
int32_t condCode(int32_t instr);
bool cpsr_N_flag(int32_t word);
bool cpsr_Z_flag(int32_t word);
bool cpsr_C_flag(int32_t word);
bool cpsr_V_flag(int32_t word);
bool instrSatisfyCond(int32_t instr, int32_t cpsr);

#endif