#include "emulate.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "inhandler.h"
#include "utilities.h"

/* 
The size of memory in 32-bit format, and the number of registers.
*/

#define REG_SIZE 17
#define MEM_SIZE 16384
#define CPSR 16
#define PC 15

/*
Address is calculated, since it is byte-addressible, as multiples of 4. So
if we are trying to access data[i], the address is i*4. 
least significant byte (LSB) of data[i] : address  = i*4
most significant byte (MSB) of data[i]  : address  = i*4 + 3
It is in little endian format.
*/


int main(int argc, char *argv[]) {
  uint32_t data[MEM_SIZE] = {0};
  uint32_t registers[REG_SIZE] = {0};
  int instrCount = 0;
  char *file;
  FILE *fptr = NULL;
  if (argv[1] != NULL) {
    file = argv[1];
    fptr = fopen(file, "rb");
  }
  if (fptr != NULL) {
    binaryLoader(fptr, file, data, MEM_SIZE, &instrCount);

    // Used in order to convert only instructions to big endian for executing functions
    uint32_t bigData[instrCount + 1];
    for (int i = 0; i < instrCount; i++) {
      bigData[i] = littleEndToBigEnd(data[i]);
    }
    // The halt instruction
    bigData[instrCount] = 0;
    //fetch - decode - execute
    uint32_t pipeline[] = {-1, -1}; 
    int i = 0;
    while (true) { 
      bool changePC = true;
      if (pipeline[1] != -1) {
        uint32_t executeWord = pipeline[1];
        if (executeWord == 0) {
          break;
        }
        if (instrIsBranch(executeWord)) {
          if (executeBranchInstruction(executeWord, data, registers, BRANCH)) {
            clearPipeline(pipeline);
            changePC = false;
          }
        }
        else {
          executeInstruction(executeWord, data, registers);
        }
      }
      
      if (changePC) {
        pushPipeline(pipeline, bigData[registers[PC] / 4]);
        registers[PC] = registers[PC] + 4;
      }
      i++;
    }
    printEndState(data, registers);
    return EXIT_SUCCESS;
  }
  printf("No Arguments or file does not exist!\n");
  return 0;
}


/* 
Parses the multiply big-endian instruction and executes the instruction
If A flag set:
  Rdest = Rm * Rs + Rn
If not:
  Rdest = Rm * Rs
*/
void executeMultiplyInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type) {
  assert(instrIsMultiply(word));
  uint32_t regDest = getDestinationRegister(word, type);
  uint32_t regN    = isAFlagSet(word) ? registers[getFirstOperandRegister(word, type)] : 0;
  uint32_t regM    = registers[getSecondOperandRegister(word)];
  uint32_t regS    = registers[getRegisterS(word)];
  if (instrSatisfyCond(word, registers[CPSR])) {
    registers[regDest] = regM * regS + regN;
    if (isSFlagSet(word)) { 
      uint32_t flag = (((cpsr_N_flag(registers[regDest])) << 3)  |
      (registers[regDest] == 0)) << 2 | cpsr_C_flag(registers[CPSR]) << 1
      | cpsr_V_flag(registers[CPSR]);
      registers[CPSR] = flag << 28;
    }
  }
}

/* 
Parses the branch big-endian instruction and executes it
If cond == register[Rcpsr] then PC = offset
*/
bool executeBranchInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type) {
  if (instrSatisfyCond(word, registers[CPSR])) {
    int32_t offset = getOffset(word, type);
    int msb = getNBits(word, 1, 23);
    if (msb) {
      offset |= 4278190080;
    }
    offset = offset << 2;
    registers[PC] += offset;
    return true;
  }
  return false;
}

/*
Parses the Big-Endian transfer instruction and then executes it.
*/
void executeTransferInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type) {
  assert(instrIsSingleDataTrans(word));
  // Obtain registers and offset
  uint32_t regSrcDst = getDestinationRegister(word, type);
  uint32_t regBase   = getFirstOperandRegister(word, type);
  uint32_t offset;
  bool carry = false;
  /*
  Check I flag
  */
  if (isIFlagSet(word)) {
    offset = getShiftedRegister(word, registers, &carry);
  }
  else {
    offset = getOffset(word, type);
  }
  // Check if Cond satisfies CSPR
  if (instrSatisfyCond(word, registers[CPSR])) {
    bool pFlag = isPFlagSet(word);
    bool uFlag = isUFlagSet(word);
    bool lFlag = isLFlagSet(word);
    uint32_t flags = (pFlag << 2) | (uFlag << 1) | lFlag;
    // Format: 0/1 - PFlag, 0/1 - UFlag, 0/1 - LFlag
    switch(flags) {
      // 0 0 0
      case 0: loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] - offset;
      break;
      // 0 0 1
      case 1: loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] - offset;
      break;
      // 0 1 0
      case 2: loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] + offset;
      break;
      // 0 1 1
      case 3: loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] + offset;
      break;
      // 1 0 0
      case 4: registers[regBase] = registers[regBase] - offset;
              loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] + offset;
      break;
      // 1 0 1
      case 5: registers[regBase] = registers[regBase] - offset;
              loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] + offset;
      break;
      // 1 1 0
      case 6: registers[regBase] = registers[regBase] + offset;
              loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
              registers[regBase] = registers[regBase] - offset;
      break;
      // 1 1 1
      default: registers[regBase] = registers[regBase] + offset;
               loadStore(registers[regBase], regSrcDst, lFlag, data, registers);
               registers[regBase] = registers[regBase] - offset;
    }
  }
}

/*
Loads or stores values.
*/
void loadStore(uint32_t regB, uint32_t regSrcDst, bool load, uint32_t data[], uint32_t registers[]) {
  uint32_t mod = regB % 4;
  uint32_t div = regB / 4;
  if (div >= 16384) {
    // Memory out of bounds.
    printf("Error: Out of bounds memory access at address 0x%08x\n", regB);
    return;
  }
  if (load) {
    registers[regSrcDst] = bigEndToLittleEnd((getNBits(data[div], 8 * (4 - mod), 0) << 8 * mod) |
    getNBits(data[div + 1], 8 * mod, 32 - 8 * mod));
  }
  else { 
    data[div] = (getNBits(data[div], 8 * mod, 32 - 8 * mod) << (8 * (4 - mod)) | 
    ((getNBits(bigEndToLittleEnd(registers[regSrcDst]), 8 * (4 - mod), 8 * mod))));
    data[div + 1] = (getNBits(bigEndToLittleEnd(registers[regSrcDst]), 8 * mod, 0) << (8 * (4 - mod)) |
    (getNBits(data[div + 1], 8 * (4 - mod), 0)));
  }
  
}
/*
Parses the Big-Endian Data instruction and then executes it.
*/
void executeDataInstruction(uint32_t word, uint32_t data[], uint32_t registers[], int type) {
  assert(instrIsDataProc(word));

  // Get offset and registers
  uint32_t regDest = getDestinationRegister(word, type);
  uint32_t regN = getFirstOperandRegister(word, type);
  uint32_t offset;
  uint32_t regTemp;
  uint32_t mask = 0x80000000;
  bool carry = false;

  // Check I flag
  if (isIFlagSet(word)) {
    offset = getOffset(word, type);
  }
  else {
    offset = getShiftedRegister(word, registers, &carry);
  }

  /*
  Check if Cond satisfies CSPR
  */
  if (instrSatisfyCond(word, registers[CPSR])) {
    uint32_t opCode = opcode(word);

    switch (opCode) {
      // 0 0 0 0 - and: regDest = regN AND offset
      case 0: registers[regDest] = registers[regN] & offset;
      break;
      // 0 0 0 1 - eor: regDest = regN EOR offset
      case 1: registers[regDest] = registers[regN] ^ offset;
      break;
      // 0 0 1 0 - sub: regDest = regN MINUS offset
      case 2: 
      carry = false;
      registers[regDest] = registers[regN] - offset;
      carry = ((registers[regDest] & mask) == 0x80000000) ? 0 : 1;
      break;
      // 0 0 1 1 - rsb: regDest = offset MINUS regN
      case 3: 
      carry = false;
      registers[regDest] = offset - registers[regN];
      carry = ((registers[regDest] & mask) == 0x80000000) ? 0 : 1;
      break;
      // 0 1 0 0 - add: regDest = regN PLUS offset
      case 4: 
      carry = false;
      registers[regDest] = registers[regN] + offset;
      carry = ((registers[regDest] & mask) == 0x80000000) ? 1 : 0;
      break;
      // 1 0 0 0 - tst: regN AND offset
      case 8: regTemp = registers[regN] & offset;
      break;
      // 1 0 0 1 - teq: regN EOR offset
      case 9: regTemp = registers[regN] ^ offset;
      break;
      // 1 0 1 0 - cmp: regN MINUS offset
      case 10:
      carry = false;
      regTemp = registers[regN] - offset;
      carry = ((regTemp & mask) == 0x80000000) ? 0 : 1;
      break;
      // 1 1 0 0 - oor: regN OR offset
      case 12: registers[regDest] = registers[regN] | offset;
      break;
      // 1 1 0 1 - mov: just operand2 (regN ignored)
      case 13: registers[regDest] = offset;
    }
    // Check S flag
    if (isSFlagSet(word)) {
      uint32_t cpsrFlags = condCode(registers[16]);
      uint32_t flagMask = 1;
      uint32_t flag;
      switch (opCode) {
        // results written in regTemp
        case 8:
        case 9:
        case 10:
        flag = (!((regTemp & mask) == 0) << 3) | ((regTemp == 0) << 2) |
        (carry << 1) | (flagMask & cpsrFlags);
        break;
        default:
        flag = (!((registers[regDest] & mask) == 0) << 3) | ((registers[regDest] == 0) << 2) |
        (carry << 1) | (flagMask & cpsrFlags);
      }
      registers[CPSR] = flag << 28;
    }
  }
}

// Checks the instruction type and executes the instruction accordingly
void executeInstruction(uint32_t word, uint32_t data[], uint32_t registers[]) {
  enum InstType type = getInstType(word);
  switch (type) {
    // Branch
    case BRANCH: executeBranchInstruction(word, data, registers, type);
    break;
    // Transfer
    case TRANSFER: executeTransferInstruction(word, data, registers, type);
    break;
    // Multiply
    case MUL: executeMultiplyInstruction(word, data, registers, type);
    break;
    // Data Processing
    case DATA: executeDataInstruction(word, data, registers, type);
    break;
    case HALT: return;
  }
}

// Prints the end state of the registers and non-zero memory.
void printEndState(uint32_t data[], uint32_t registers[]) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-2d : %10d (0x%08x)\n", i, registers[i], registers[i]);
  }
  printf("PC  : %10d (0x%08x)\n", registers[PC], registers[PC]);
  printf("CPSR: %10d (0x%08x)\n", registers[CPSR], registers[CPSR]);
  printf("Non-zero memory:\n");
  for (int i = 0; i < MEM_SIZE; i++) {
    if (data[i] != 0) {
      printf("0x%08x: 0x%08x\n", i * 4, data[i]);
    }
  }
}



