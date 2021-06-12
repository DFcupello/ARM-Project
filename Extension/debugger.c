#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "emulate.h"
#include "utilities.h"
#include "assemble.h"
#include "inassembler.h"
#include "inhandler.h"
#include "priorityqueue.h"

#define MAX_COMMAND_SIZE 551

typedef enum Command {
    QUIT,
    RUN,
    HELP,
    NEXT,
    PRINT,
    BREAK,
    WATCH,
    TOKENERR
} command;
/*
Usage: ./debugger {Binary File} {Assembly File} 
List of Commands:
r - run
b {Line#} - Stops when it reaches this line
n - Step to next instruction
p R{Register#} - Prints the content of the register
p {Address} - Prints the content of memory
q - quit the debugger
h - print the list of commands

optional:
w R{Register#} - Watches a current register to see when its value changes
*/

/*
Parses the command, using the tokens obtained from the tokeniser.
Returns an int, representing the ENUM type Command
*/
int lexerCommand(char **tokens, int tokenSize) {
    assert(tokenSize >= 1 && tokenSize <= 2);
    if (tokenSize == 1) {
        if (strlen(tokens[0]) == 1) {
            switch (tokens[0][0]) {
                case 'q': return QUIT;
                break;
                case 'r': return RUN;
                break;
                case 'n': return NEXT;
                break;
                case 'h': return HELP;
                break;
                case 'p': return PRINT;
                break;
                default: return TOKENERR;
            }
        }
    }
    else if (tokenSize == 2) {
        if (strlen(tokens[0]) == 1) {
            switch(tokens[0][0]) {
                case 'b': return BREAK;
                break;
                case 'w': return WATCH;
                break;
                case 'p': return PRINT;
                break;
                default: return TOKENERR;
            }
        }
    }
    return TOKENERR;
}

char **commandTokenizer(char *string, int tokenSize) {
    char *saveptr1 = string;
    char **tokens = malloc(sizeof(char *) * tokenSize);
    for (int i = 0; i < tokenSize; i++) {
        char *temp =  strtok_r(saveptr1, " \n", &saveptr1);
        tokens[i] = strcpy(malloc(strlen(temp) + 1), temp);
    }
    
    return tokens;
}

bool strIsEmpty(char *str) {
    bool isEmpty = true;
    for (int i = 0; i < strlen(str); i++) {
        isEmpty = isEmpty && (str[i] == '\n' || str[i] == ' ');
    }
    return isEmpty;
}

/*
Prints the command symbol > and waits for user input. Tries to parse result, by tokenising.
*/
char **readDebuggerCommand(command *cmd, int *tokenSize) {
    printf("> ");
    char answer[MAX_COMMAND_SIZE];
    fgets(answer, MAX_COMMAND_SIZE, stdin);
    if (strIsEmpty(answer))
    {
        *cmd = TOKENERR;
        return NULL;
    }
    else {
        *tokenSize = getTokenSize(answer);
        char **tokens = commandTokenizer(answer, *tokenSize);
        *cmd = lexerCommand(tokens, *tokenSize);
        return tokens;
    } 
}

void printHelp(void) {
    printf("List Of Commands:\n");
    printf("q              - quit the program.\n");
    printf("n              - Execute current line and print next.\n");
    printf("r              - runs the program until it notices a breakpoint.\n");
    printf("w R{register#} - sets a watchpoint for the given register.\n");
    printf("b {Line#}      - sets a breakpoint for the given line.\n");
    printf("p {address}    - prints the content of the address.\n");
    printf("p R{register#} - prints the content of the register.\n");
}

void freeTokens(char **tokens, int tokenSize) {
    for (int i = 0; i < tokenSize; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

void breakWatchFunc(priorityQueue *collection, char **tokens, int instrCount) {
    int lineNum = atoi(tokens[1]);
    if (lineNum >= 1 && lineNum <= instrCount)
    {
        addNode(collection, lineNum - 1);
        printPriorityQueue(collection);
    }
}

bool isNumber(char *str) {
    bool isNum = true;
    for (int i = 0; i < strlen(str); i++) {
        isNum = isNum && isdigit(str[i]);
    }
    return isNum;
}

bool isHexadecimal(char *str) {
    bool isHex = true;
    for (int i = 0; i < strlen(str); i++) {
        isHex = isHex && (isdigit(str[i]) || (str[i] >= 'a' && str[i] <= 'f'));
    }
    return isHex;
}

void debug(uint32_t data[], int instrCount, char assemblyInstrs[][MAX_COMMAND_SIZE]) {
    printf("WELCOME TO THE ARM DEBUGGER - press h to see list of commands.\n");
    uint32_t instructions[instrCount + 1];
    uint32_t registers[REG_SIZE] = {0};
    for (int i = 0; i < instrCount; i++) {
        instructions[i] = littleEndToBigEnd(data[i]);
    }
    // The halt instruction
    instructions[instrCount] = 0;
    //fetch - decode - execute
    uint32_t pipeline[] = {-1, -1};
    // Preperation for debugging
    priorityQueue *breakpoints = allocatePriorityQueue();
    priorityQueue *watchpoints = allocatePriorityQueue();
    command cmd;
    bool printLine = true;
    while (true) {
        int tokenSize;
        if (registers[PC] / 4 >= instrCount) {
            break;
        }
        if (printLine) {
            printf("%d %s\n", registers[PC] / 4 + 1, assemblyInstrs[registers[PC] / 4]);
        }
        char **tokens = readDebuggerCommand(&cmd, &tokenSize);
        if (tokens == NULL || tokens[0][0] == ' ') {
            printf("Put a command :)\n");
            continue;
        }
        if (cmd == QUIT) {
            freeTokens(tokens, tokenSize);
            break;
        }
        else if (cmd == TOKENERR) {
            printf("Invalid command :)\n");
            freeTokens(tokens, tokenSize);
            continue;
        }
        switch (cmd) {
        case RUN: 
            {
                bool notDone = true;
                while (notDone) {
                    notDone = emulateInstruction(data, registers, instructions, pipeline);
                    notDone = notDone && !contains(breakpoints, registers[PC] / 4);
                }
                printLine = true;
            }
        break;
        case NEXT:
            {
                emulateInstruction(data, registers, instructions, pipeline); //need to fix pipeline as registers take 2 instructions to update
                printLine = true;
            }
        break;
        case HELP: 
            {
                printHelp();
                printLine = false;
            }
        break;
        case BREAK:
            { 
                breakWatchFunc(breakpoints, tokens, instrCount);
                printLine = false;
            }
        break;
        case PRINT: 
            {
                if (tokenSize == 2 && strlen(tokens[1]) > 1 && tokens [1][0] == 'R' && isNumber(&(tokens[1][1])))
                {
                    int regNum = atoi(&(tokens[1][1]));
                    if (regNum >= 0 && regNum <= 16) {
                        printf("Register %d: %d\n", regNum, registers[regNum]);
                    }
                    
                }
                else if (tokenSize == 2 && strlen(tokens[1]) > 2 && tokens[1][0] == '0' && tokens[1][1] == 'x' && isHexadecimal(&(tokens[1][2])))
                {
                    int addNum = (int) strtol(&(tokens[1][2]), NULL, 16);
                    printf("Address 0x%08x: 0x%08x\n", addNum, data[addNum / 4]);
                }
                else if (tokenSize == 1)
                {
                    printEndState(data, registers);
                }

                printLine = false;
            }
        break;
        case WATCH:
            { 
                breakWatchFunc(watchpoints, tokens, instrCount);
                printLine = false;
            }
        break;
        default: 
            {
                printf("Invalid command :)\n");
                printLine = false;
            }
        }
        freeTokens(tokens, tokenSize);
    } 
    
    //while (emulateInstruction(data, registers, instructions, pipeline));
    //printEndState(data, registers);
}

#ifdef MAIN_DEBUGGER
int main(int argc, char *argv[]) {

    uint32_t data[MEM_SIZE] = {0};

    char *binaryFile = "/homes/dc1020/arm11_testsuite/test_cases/loop01";
    char *assemblyFile = "/homes/dc1020/arm11_testsuite/test_cases/loop01.s";
    FILE *fptrBinary = fopen(binaryFile, "rb"); 
    FILE *fptrAssembly = fopen(assemblyFile, "r");
    int instrCount = 0;
    // if (argc != 3) {
    //     printf("Usage: ./debugger {Binary File} {Assembly File}\n");
    //     return EXIT_FAILURE;
    // }
    // if (argv[1] != NULL && argv[2] != NULL) {
    //     assemblyFile = argv[1];
    //     binaryFile = argv[2];
    //     fptrAssembly = fopen(assemblyFile, "r");
    //     fptrBinary = fopen(binaryFile, "rb");
    // }
    if (fptrAssembly != NULL && fptrBinary != NULL) {
        // Preperation to emulate
        binaryLoader(fptrBinary, binaryFile, data, MEM_SIZE, &instrCount);
        char assemblyInstrs[instrCount][MAX_COMMAND_SIZE];
        for (int i = 0; i < instrCount; i++) {
            char line[MAX_COMMAND_SIZE];
            fgets(line, MAX_COMMAND_SIZE, fptrAssembly);
            bool isLabel = false;
            for (int j = 0; j < strlen(line); j++) {
                isLabel = isLabel | (line[j] == ':');
            }
            if (!isLabel) {
                strtok(line, "\n");
                strcpy(assemblyInstrs[i], line);                
            } else {
                i--;
            }
        }
        debug(data, instrCount, assemblyInstrs);
        fclose(fptrAssembly);
        return EXIT_SUCCESS;
    }
    printf("File does not exist!\n");
    return EXIT_FAILURE;
}
#endif