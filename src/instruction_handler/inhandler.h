#ifndef INSTRUCTION_HANDLER
#define INSTRUCTION_HANDLER

// Endian convertion
uint32_t littleEndToBigEnd(uint32_t origInstr);
uint32_t bigEndToLittleEnd(uint32_t origInstr);

// Instruction type determination
bool instrIsDataProc(uint32_t instr);
bool instrIsMultiply(uint32_t instr);
bool instrIsSingleDataTrans(uint32_t instr);
bool instrIsBranch(uint32_t instr);

// Getting instruction flags and opcode;
bool isIFlagSet(uint32_t instr);
bool isSFlagSet(uint32_t instr);
bool isAFlagSet(uint32_t instr);
bool isPFlagSet(uint32_t instr);
bool isUFlagSet(uint32_t instr);
bool isLFlagSet(uint32_t instr);
uint32_t opcode(uint32_t instr);

// Condition code and CPSR register
uint32_t condCode(uint32_t instr);
bool cpsr_N_flag(uint32_t word);
bool cpsr_Z_flag(uint32_t word);
bool cpsr_C_flag(uint32_t word);
bool cpsr_V_flag(uint32_t word);
bool instrSatisfyCond(uint32_t instr, uint32_t cpsr);

#endif