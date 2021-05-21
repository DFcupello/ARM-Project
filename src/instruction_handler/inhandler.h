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

// Getting the components of instructions
// TODO


#endif