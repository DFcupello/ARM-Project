#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

int main(void) {
    return 0;
}

/* 
  Takes instruction mnemonic (add, sub, beq, ...) with '\0' char at the end.
  Returns pointer to new created sub array last two chars followed by '\0' char.
  In the absence of suffix, returns pointer to "al", which is equivalent to no suffix.
	Reqire to free the pointer using freeSuffix() or just free().
*/
char * getSuffix(char *mnemonic) {

	char * suffix = malloc(sizeof(char) * 3);
	suffix[2] = '\0';
	int length = strlen(mnemonic);

	if (length == 5) {                              // non branch
		suffix[0] == mnemonic[3];
		suffix[1] == mnemonic[4];
	} else if (length == 3 && mnemonic[0] == 'b') { // branch
		suffix[0] == mnemonic[1];
		suffix[1] == mnemonic[2];
	} else {                                        // no suffix
		suffix[0] == 'a';
		suffix[1] == 'l';
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
	while (strcmp(suffixAsStr, supportedSuffixes[i]) == 0) {
		i++;
    if (i >= 7) {
      return 0xffffffff; // case of unsupported suffix
    }
	}
	return correspondingCodes[i];
}

/*
	Takes instruction mneumonics and address (&) of boolean flag variable.
	Returns the oppcode if the instruction is data processing, 2^32 - 1 otherwise.
	Modifies the value stored in the second argument address.
	Sets true if the mneumonic is in fact dataprocessing false otherwise.
*/
uint32_t getOpcodeFromMneumonic(char *mnemonic, bool *isItReallyDataProc) {

	char mneumonicAsStr[4] = {mnemonic[0], mnemonic[1], mnemonic[2], '\0'};

	char mneumonics[10][3] = {"and", "eor", "sub", "rsb", "add",
	                          "orr", "mov", "tst", "teq", "cmp"};
	uint32_t opcodes[10] = {0, 1, 2, 3, 4, 0xc, 0xd, 8, 9, 0xa};

  int i = 0;
  while (strcmp(mneumonicAsStr, mneumonics[i]) == 0) {
    i++;
    if (i >= 7) {
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

