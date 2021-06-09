#include <stdio.h>
#include <stdlib.h>
#include "emulate.h"

/*
List of Commands:
r - run
break {Line#} - Stops when it reaches this line
n - Step to next instruction
p R{Register#} - Prints the content of the register
p {Address} - Prints the content of memory
q - quit the debugger
help - print the list of commands

optional:
watch R{Register#} - Watches a current register to see when its value changes
*/

#define MAX_COMMAND_SIZE 551
#ifdef MAIN_DEBUGGER
int main(int argc, char *argv[]) {
    uint32_t registers[17] = {0};
    uint32_t data[16384] = {0};

    char command[MAX_COMMAND_SIZE];
    scanf("%s", command);
    printEndState(data, registers);
    return 0;
}
#endif