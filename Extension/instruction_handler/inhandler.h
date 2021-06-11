#ifndef INSTRUCTION_HANDLER
#define INSTRUCTION_HANDLER

enum InstType
{
    BRANCH,
    TRANSFER,
    MUL,
    DATA,
    HALT,
    BLOCK_DATA_TRANSFER
};

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

// Block Data Transfer Instruction Flags
bool isBDT_P_flagSet(uint32_t instr);
bool isBDT_U_flagSet(uint32_t instr);
bool isBDT_S_flagSet(uint32_t instr);
bool isBDT_W_flagSet(uint32_t instr);
bool isBDT_L_flagSet(uint32_t instr);

// Condition code and CPSR register
uint32_t condCode(uint32_t instr);
bool cpsr_N_flag(uint32_t word);
bool cpsr_Z_flag(uint32_t word);
bool cpsr_C_flag(uint32_t word);
bool cpsr_V_flag(uint32_t word);
bool instrSatisfyCond(uint32_t instr, uint32_t cpsr);

// Get Registers and Offset
uint32_t getDestinationRegister(uint32_t word, int type);
uint32_t getFirstOperandRegister(uint32_t word, int type);
uint32_t getSecondOperandRegister(uint32_t word);
uint32_t getOffset(uint32_t word, int type);
uint32_t getRegisterS(uint32_t word);
uint32_t getShiftedRegister(uint32_t word, uint32_t registers[], bool *carry);
//Block Data Transfer registers
bool getBaseRegisterForBDT(uint32_t instr);
int *getRegisterList(uint32_t instr);

// Getting type
enum InstType getInstType(uint32_t word);

#endif