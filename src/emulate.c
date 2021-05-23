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

uint32_t data[MEM_SIZE];
uint32_t registers[REG_SIZE];

int main(int argc, char *argv[]) {
  char *file;
  FILE *fptr = NULL;
  if (argv[1] != NULL) {
    file = argv[1];
    fptr = fopen(file, "rb");
  }
  if (fptr != NULL) {
    binaryLoader(fptr, file, false, data, MEM_SIZE);
    // for (int i = 0; i < 10; i++) {
    //   printf("%d\n", data[i]);
    // }
    executeDataInstruction(data[0]);
    printEndState();
    
    /*
      start of loop
      data[0] is first instruction
      data[n] is last instruction (halt)
    */
    /*
    registers[15] = 0; //set the program counter to the first intruction
    while(true) {
      // 1. Fetch instruction
      int32_t currentInstruction = data[registers[15]];
      // 2 & 3. Decode and execute instruction
      //switch
      registers[15]++;
    } // Will finish when HALT instruction is executed
    */
    printf("Success!\n");
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
void executeMultiplyInstruction(uint32_t word) {
  assert(getInstType(word) == Mul);
  uint32_t regDest = getDestinationRegister(word);
  uint32_t regN    = isAFlagSet(word) ? registers[getFirstOperandRegister(word)] : 0;
  uint32_t regM    = registers[getSecondOperandRegister(word)];
  uint32_t regS    = registers[getRegisterS(word)];
  if (instrSatisfyCond(word, registers[CPSR])) {
    registers[regDest] = regM * regS + regN;
    if (isSFlagSet(word)) { 
      uint32_t flag = (((cpsr_N_flag(registers[regDest])) << 3)  |
      (registers[regDest] == 0)) << 2 | cpsr_C_flag(registers[CPSR]) << 1
      | cpsr_V_flag(registers[CPSR]) ;
      registers[CPSR] = flag << 28;
    }
  }
}

/* 
Parses the branch big-endian instruction and executes it
If cond == register[Rcpsr] then PC = offset
*/
void executeBranchInstruction(uint32_t word) {
  assert(getInstType(word) == Branch);
  uint32_t offset = (getOffset(word)) << 2;
  if (instrSatisfyCond(word, registers[CPSR])) {
    registers[PC] = offset;
  }
}

/*
Parses the Big-Endian transfer instruction and then executes it.
*/
void executeTransferInstruction(uint32_t word) {
  assert(getInstType(word) == Transfer);
  /*
  Obtain registers and offset
  */
  uint32_t regSrcDst = getDestinationRegister(word);
  uint32_t regBase   = getFirstOperandRegister(word);
  uint32_t offset;
  bool carry = false;
  /*
  Check I flag
  */
  if (isIFlagSet(word)) {
    offset = getShiftedRegister(word, registers, &carry);
  }
  else {
    offset = getOffset(word);
  }

  /*
  Check if Cond satisfies CSPR
  */
  if (instrSatisfyCond(word, registers[CPSR])) {
    bool pFlag = isPFlagSet(word);
    bool uFlag = isUFlagSet(word);
    bool lFlag = isLFlagSet(word);
    uint32_t flags = (pFlag << 2) | (uFlag << 1) | lFlag;
    /*
        Format: 0/1 - PFlag, 0/1 - uFlag, 0/1 - lFlag
    */
    switch(flags) {
      // 0 0 0
      case 0: data[registers[regBase]] = registers[regSrcDst];
              registers[regBase] = registers[regBase] - offset;
      break;
      // 0 0 1
      case 1: registers[regSrcDst] = data[registers[regBase]];
              registers[regBase] = registers[regBase] - offset;
      break;
      // 0 1 0
      case 2: data[registers[regBase]] = registers[regSrcDst];
              registers[regBase] = registers[regBase] + offset;
      break;
      // 0 1 1
      case 3: registers[regSrcDst] = data[registers[regBase]];
              registers[regBase] = registers[regBase] + offset;
      break;
      // 1 0 0
      case 4: registers[regBase] = registers[regBase] - offset;
              data[registers[regBase]] = registers[regSrcDst];
      break;
      // 1 0 1
      case 5: registers[regBase] = registers[regBase] - offset;
              registers[regSrcDst] = data[registers[regBase]];
      break;
      // 1 1 0
      case 6: registers[regBase] = registers[regBase] + offset;
              data[registers[regBase]] = registers[regSrcDst];
      break;
      // 1 1 1
      default: registers[regBase] = registers[regBase] + offset;
               registers[regSrcDst] = data[registers[regBase]];
    }
  }
}

/*
Parses the Big-Endian Data instruction and then executes it.
*/
void executeDataInstruction(uint32_t word) {
  assert(getInstType(word) == Data);
  /*
  Get offset and registers
  */
  uint32_t regDest = getDestinationRegister(word);
  uint32_t regN    = getFirstOperandRegister(word);
  uint32_t offset;
  uint32_t regTemp;
  uint32_t mask = 1 << 31;
  bool carry = false;
  /*
  Check I flag
  */
  if (isIFlagSet(word)) {
    offset = getOffset(word);
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
      carry = ((registers[regDest] & mask) == 1) ? 0 : 1;
      break;
      // 0 0 1 1 - rsb: regDest = offset MINUS regN
      case 3: 
      carry = false;
      registers[regDest] = offset - registers[regN];
      carry = ((registers[regDest] & mask) == 1) ? 0 : 1;
      break;
      // 0 1 0 0 - add: regDest = regN PLUS offset
      case 4: 
      carry = false;
      registers[regDest] = registers[regN] + offset;
      carry = ((registers[regDest] & mask) == 1) ? 1 : 0;
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
      carry = ((registers[regDest] & mask) == 1) ? 0 : 1;
      break;
      // 1 1 0 0 - oor: regN OR offset
      case 12: registers[regDest] = registers[regN] | offset;
      break;
      // 1 1 0 1 - mov: just operand2 (regN ignored)
      case 13: registers[regDest] = offset;
    }

    /* 
    Check S flag
    */
    if (isSFlagSet(word)) {
      uint32_t cpsrFlags = condCode(registers[16]);
      uint32_t flagMask = 1;
      uint32_t flag;
      switch (opCode) {
        /* 
        results written in regTemp
        */
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

// Prints the end state of the registers and non-zero memory.
void printEndState(void) {
  printf("Registers:\n");
    for (int i = 0; i < 13; i++) {
      printf("$%-2d : %10d (0x%08x)\n", i, registers[i], registers[i]);
    }
    printf("PC  : %10d (0x%08x)\n", registers[15], registers[15]);
    printf("CPSR: %10d (0x%08x)\n", registers[16], registers[16]);
    printf("Non-zero memory:\n");
    for (int i = 0; i < MEM_SIZE; i++) {
      if (data[i] != 0) {
        printf("0x%08x: 0x%08x\n", i * 4, data[i]);
      }
    }
}



