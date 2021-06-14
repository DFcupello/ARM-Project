#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "emulate.h"
#include "utilities.h"
#include "inassembler.h"
#include "inhandler.h"
#include "orderedset.h"
#include "debugger.h"

/*
Usage: ./debugger {Binary File} {Assembly File} 
List of Commands:
r - run/continue program
b {Line#} - Stops when it reaches this line
n - Step to next instruction
p R{Register#} - Prints the content of the register
p {Address} - Prints the content of memory
rb {Line#}  - removes a breakpoint for the given line.
rw R{reg#}  - removes a watchpoint for the given register.
q - quit the debugger
h - print the list of commands
w R{Register#} - Watches a current register to see when its value changes
*/

/*
Parses the command, using the tokens obtained from the tokeniser.
Returns an int, representing the ENUM type Command
*/
int lexerCommand(char **tokens, int tokenSize) {
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
        else if (strlen(tokens[0]) == 2) {
            if (strcmp(tokens[0], "rb") == 0) {
                return REMBREAK;
            }
            else if (strcmp(tokens[0], "rw") == 0) {
                return REMWATCH;
            }
        }
    }
    return TOKENERR;
}

/*
Prints the command symbol > and waits for user input. Tries to parse result, by tokenising.
*/
char **readDebuggerCommand(command *cmd, int *tokenSize) {
    printf("> ");
    char answer[MAX_COMMAND_SIZE];
    fgets(answer, MAX_COMMAND_SIZE, stdin);
    if (answer[0] == ' ' || strIsEmpty(answer))
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

/*
Prints the according helpful list of commands
*/
void printHelp(void) {
    printf("List Of Commands:\n");
    printf("q           - quit the program.\n");
    printf("n           - Execute current line and prints the next line.\n");
    printf("r           - runs the program until it notices a breakpoint.\n");
    printf("w R{reg#}   - sets a watchpoint for the given register.\n");
    printf("rw R{reg#}  - removes a watchpoint for the given register.\n");
    printf("b {Line#}   - sets a breakpoint for the given line.\n");
    printf("rb {Line#}  - removes a breakpoint for the given line.\n");
    printf("p {address} - prints the content of the address.\n");
    printf("p R{reg#}   - prints the content of the register.\n");
    printf("p all       - prints the state of all registers and memory.\n");
}

/*
Adds the breakpoint or watchpoint to the corresponding collection.
*/
void watchFunc(orderedSet *collection, char **tokens, bool remove) {
    if (strlen(tokens[1]) > 1 && tokens[1][0] == 'R' && isNumber(&(tokens[1][1]))) {
        int reg = registerCode(tokens[1]);
        if (reg >= 0 && reg < REG_SIZE) {
            if (remove) {
                if (!removeNode(collection, reg)) {
                    printf("Watchpoint not found!\n");
                    return;
                }
                printf("Removed a watchpoint on R%d.\n", reg);
            }
            else {
                if (!addNode(collection, reg)) {
                    printf("Duplicate watchpoint on R%d detected!\n", reg);
                    return;
                }
                printf("Placed a watchpoint on R%d.\n", reg);
            }
        }
    }
    else {
        printf("INVALID Watch Register.\n");
    }
    
}

void breakFunc(orderedSet *collection, char **tokens, int instrCount, bool remove) {
    int lineNum = atoi(tokens[1]);
    if (lineNum >= 1 && lineNum <= instrCount)
    {
        if (remove) {
            if (!removeNode(collection, lineNum - 1)) {
                printf("Breakpoint Not Found!\n");
                return;
            }
            printf("Removed a breakpoint on line number %d.\n", lineNum);
        }
        else {
            if (!addNode(collection, lineNum - 1)) {
                printf("Duplicate breakpoint on line number %d detected!\n", lineNum);
                return;
            }
            printf("Placed a breakpoint on line number %d.\n", lineNum);
        }
    }
    else {
        printf("INVALID Break line.\n");
    }
}
    /*
Either prints the picked register, or the address (must be in hexadecimal format).
p All prints the state of all registers and memory.
p R{#Register}
p #Address
p all
*/
void printFunc(uint32_t data[], uint32_t registers[], char **tokens) {
    if (strlen(tokens[1]) > 1 && tokens [1][0] == 'R' && isNumber(&(tokens[1][1]))) {
        int regNum = atoi(&(tokens[1][1]));
        if (regNum >= 0 && regNum <= 16) {
        printf("Register %d: %d\n", regNum, registers[regNum]);
        }
    }
    else if (strlen(tokens[1]) > 2 && tokens[1][0] == '0' && tokens[1][1] == 'x' && isHexadecimal(&(tokens[1][2]))) {
        int addNum = (int) strtol(&(tokens[1][2]), NULL, 16);
        if (addNum / 4 <= MEM_SIZE) {
            printf("Address 0x%08x: 0x%08x\n", addNum, data[addNum / 4]);
        }
        else {
            printf("Error: Out of bounds memory access at address 0x%08x\n", addNum);
        }
    }
    else if (strcmp(tokens[1], "all") == 0) {
        printEndState(data, registers);
    }
    else {
        printf("Invalid printing command.\n");
    }
}

void runFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[], 
uint32_t instructions[], orderedSet *breakpoints) {
    bool notDone = true;
    bool isPipelineFull = false;
    while (notDone) {
        do {
            notDone = emulateInstruction(data, registers, instructions, pipeline, &isPipelineFull);
        } while (!isPipelineFull);
        notDone = notDone && !contains(breakpoints, registers[PC] / 4 - 2);
    }
}

void nextFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[],
uint32_t instructions[], orderedSet *watchpoints) {
    bool isPipelineFull = false;
    bool watchIsEmpty = isEmpty(watchpoints);
    uint32_t copyReg[REG_SIZE];
    if (!watchIsEmpty) {
        for (int i = 0; i < REG_SIZE; i++) {
            copyReg[i] = registers[i];
        }
    }
    while (!isPipelineFull) {
        emulateInstruction(data, registers, instructions, pipeline, &isPipelineFull);
    }
    if (!watchIsEmpty) {
        for (int i = 0; i < REG_SIZE; i++) {
            if (contains(watchpoints, i) && registers[i] != copyReg[i]) {
                printf("R%d changed!\n", i);
                printf("Before: %d\n", copyReg[i]);
                printf("After: %d\n", registers[i]);
            }
        }
    }
}

void quitFunc(bool *debugDone) {
    bool hasAnswered = false;
    while (!hasAnswered) {
        printf("Are you sure you want to quit?[y/n]: ");
        char answer[MAX_COMMAND_SIZE];
        fgets(answer, MAX_COMMAND_SIZE, stdin);
        if (answer[0] == 'y' && strlen(answer) == 2) {  // Has newline
            hasAnswered = true;
            *debugDone = true;
        }
        else if (answer[0] == 'n' && strlen(answer) == 2) {
            hasAnswered = true;
        }
        else {
            printf("Please answer y or n.\n");
        }
    }
}

void debug(uint32_t data[], int instrCount, char assemblyInstrs[][MAX_COMMAND_SIZE]) {
    printf("WELCOME TO THE ARM11 DEBUGGER - press h to see list of commands.\n");
    uint32_t instructions[instrCount + 1];
    uint32_t registers[REG_SIZE] = {0};
    for (int i = 0; i < instrCount; i++) {
        instructions[i] = littleEndToBigEnd(data[i]);
    }
    // The halt instruction
    instructions[instrCount] = 0;

    //fetch - decode - execute pipeline
    uint32_t pipeline[] = {-1, -1};

    // Preperation for debugging
    orderedSet *breakpoints = allocateSet();
    orderedSet *watchpoints = allocateSet();
    command cmd;
    bool printLine = false;
    bool hasRun = false;
    bool debugDone = false;
    // Debugger Loop
    while (!debugDone) {
        int tokenSize;
        if (registers[PC] / 4 >= instrCount + 2) {
            printf("Program ended successfully.\n");
            printEndState(data, registers);
            break;
        }
        if (printLine) {
            printf("%d %s\n", registers[PC] / 4 - 1, assemblyInstrs[registers[PC] / 4 - 2]);
        }

        char **tokens = readDebuggerCommand(&cmd, &tokenSize);
        if (tokens == NULL || tokens[0][0] == ' ') {
            printf("Invalid command.\n");
            continue;
        }
        switch (cmd) {
        case RUN: 
            runFunc(data, registers, pipeline, instructions, breakpoints);
            printLine = true;
            hasRun = true;
            break;
        case NEXT:
            if (hasRun) {
                nextFunc(data, registers, pipeline, instructions, watchpoints);
                printLine = true;
            }
            else
            {
                printf("Program has not started yet.\n");
            }
            break;
        case HELP: 
            printHelp();
            printLine = false;
            break;
        case BREAK:
            breakFunc(breakpoints, tokens, instrCount, false);
            printLine = false;
            break;
        case PRINT: 
            printFunc(data, registers, tokens);
            printLine = false;
            break;
        case WATCH: 
            watchFunc(watchpoints, tokens, false);
            printLine = false;
            break;
        case QUIT:
            quitFunc(&debugDone);
            break;
        case REMBREAK:
            breakFunc(breakpoints, tokens, instrCount, true);
            printLine = false;
            break;
        case REMWATCH:
            watchFunc(watchpoints, tokens, true);
            printLine = false;
            break;
        default: 
            printf("Invalid command.\n");
            printLine = false;
        }
        freeCommandTokens(tokens, tokenSize);
    } 
    freeSet(breakpoints);
    freeSet(watchpoints);
}

void getAssemblyInstrs(int instrCount, FILE *fptrAssembly, char assemblyInstrs[][MAX_COMMAND_SIZE])  {
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
}

#ifdef MAIN_DEBUGGER
int main(int argc, char *argv[]) {

    uint32_t data[MEM_SIZE] = {0};
    char *binaryFile = "/homes/dc1020/arm11_testsuite/test_cases/factorial";
    char *assemblyFile = "/homes/dc1020/arm11_testsuite/test_cases/factorial.s";
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
        getAssemblyInstrs(instrCount, fptrAssembly, assemblyInstrs);
        debug(data, instrCount, assemblyInstrs);
        fclose(fptrAssembly);
        return EXIT_SUCCESS;
    }
    printf("File does not exist!\n");
    return EXIT_FAILURE;
}
#endif