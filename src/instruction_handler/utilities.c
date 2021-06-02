#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include "utilities.h"
#include "inhandler.h"

/* 
Takes a binary file pointer, file, and the bool endian, which if 1,
stores the binary file in arm memory in uint32_t format,
in big endian if 1, and little endian if 0.
*/
void binaryLoader(FILE *fptr, char *file, uint32_t *data, int size, int *instrCount) {
    unsigned char buffer[4];
    int i = 0;
    while (i < size) { // This is to ensure it terminates
        fread(buffer, sizeof(buffer), 1, fptr);
        if (feof(fptr)) {
            break;
        }
        uint32_t word = 0;
        for (int j = 0; j < 4; j++) {
            word |= buffer[j] << (8 * (3 - j));
        }
        data[i] = word;
        i++;
        *instrCount += 1;
    }
    fclose(fptr);
}

/*
  Gets n amount of bit in a certain position (including) (0th position is LSb)
  Example: input: (0101 0000 1010 1111 1110 0100 1101 1100, 4, 12) output: 1110
  Example: input: (0101 0000 1010 1111 1110 0100 1101 1100, 7, 0) output: 
  101 1100
*/
uint32_t getNBits(uint32_t word, int amount, int pos) {
    assert(amount >= 0 && pos >= 0 && amount + pos <= 32);
    uint32_t mask = pow(2, amount) - 1;
    uint32_t answer = 0;
    mask = mask << pos;
    answer = mask & word;
    answer = answer >> pos;
    return answer;
}


/* 
Rotates right a specified value by the rotateAmount:
input:               01110 
output(rotate by 4): 11100
precondition: rotateAmount must be even, and in range of 0-30
postcondition: r = value shifted
*/
uint32_t rotateRight(uint32_t value, uint32_t rotateAmount, bool *carry) {
    assert(rotateAmount % 2 == 0 && rotateAmount >= 0 && rotateAmount <= 30);
    uint32_t temp = value;
    uint32_t mask = 1;
    uint32_t apply = 1 << 31;
    for (int i = 0; i < rotateAmount; i++) {
        if ((temp & mask) == 1) {
            *carry = true;
        }
        temp = (temp >> 1) | ((temp & mask) == 1 ? apply : 0);
    }
    return temp;
}

/*
shifts right, perserving the sign bit for 2's complement 
input:               01110000
output(shift by 4):  00000111
precondition: true
postcondition: r = value shifted
*/
uint32_t arithmeticShiftRight(uint32_t value, uint32_t shiftAmount, bool *carry) {
    uint32_t temp = value;
    uint32_t mask = 1 << 31;
    uint32_t apply = temp & mask;
    uint32_t carryCheck = 1;
    for (int i = 0; i < shiftAmount; i++) {
        if ((carryCheck & temp) == 1) {
            *carry = true;
        }
        temp = (temp >> 1) | apply;
    }
    return temp;
}

/*
shifts right if (right = true), left otherwise
precondition: true
postcondition: r = value shifted
*/
uint32_t logicalShift(uint32_t value, uint32_t shiftAmount, bool right, bool *carry) {
    uint32_t temp = value;
    uint32_t carryCheck = right ? 1 : 1 << 31;
    for (int i = 0; i < shiftAmount; i++) {
        if ((carryCheck & temp) == 1) {
            *carry = true;
        }
        temp = right ? (temp >> 1) : (temp << 1);
    }
    return temp;
}



/* 
Pushes the pipeline and its instructions to the next stage
*/
void pushPipeline(uint32_t pipeline[], uint32_t fetchWord) {
    pipeline[1] = pipeline[0];
    pipeline[0] = fetchWord;
}

/*
Makes pipeline sentinal value -1
*/
void clearPipeline(uint32_t pipeline[]) {
    for (int i = 0; i < 2; i++)
    {
        pipeline[i] = -1;
    }
}
