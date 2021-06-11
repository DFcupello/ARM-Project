#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "emulate.h"
#include "utilities.h"
#include "assemble.h"
#include "inassembler.h"
#include "inhandler.h"

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
    if (tokenSize == 1 || tokenSize == 2) {
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

/*
Prints the command symbol > and waits for user input. Tries to parse result, by tokenising.
*/
int readDebuggerCommand(void) {
    printf("> ");
    char answer[MAX_COMMAND_SIZE];
    scanf("%s", answer);
    int tokenSize = getTokenSize(answer);
    char *tokens[tokenSize];
    tokenizer(answer, tokenSize, tokens);
    return lexerCommand(tokens, tokenSize);
}

void printHelp(void) {
    printf("List Of Commands:\n");
    printf("q - quit the program.\n");
    printf("n - Execute current line and print next.\n");
    printf("r - runs the program until it notices a breakpoint.\n");
    printf("w R{register#} - sets a watchpoint for the given register.\n");
    printf("b {Line#} - sets a breakpoint for the given line.\n");
    printf("p {address} - prints the content of the address.\n");
    printf("p R{register#} - prints the content of the register.\n");
}


void debug(uint32_t data[], int instrCount, FILE *fptrAssembly) {
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
    int breakpoints[instrCount];
    int watchpoints[instrCount];
    command cmd;
    while (true) {
        cmd = readDebuggerCommand();
        printf("%d\n", cmd);
        if (cmd == QUIT) {
            break;
        }
        else if (cmd == TOKENERR) {
            printf("Invalid command :)\n");
            continue;
        }
        switch (cmd) {
        case RUN:
        break;
        case NEXT:
            emulateInstruction(data, registers, instructions, pipeline);
        break;
        case HELP: printHelp();
        break;
        case BREAK:
        break;
        case PRINT: 
        break;
        case WATCH:
        break;
        default: printf("Invalid command :)\n");
        }
    } 
    
    //while (emulateInstruction(data, registers, instructions, pipeline));
    //printEndState(data, registers);
}

#ifdef MAIN_DEBUGGER
int main(int argc, char *argv[]) {

    uint32_t data[MEM_SIZE] = {0};
    char *binaryFile;
    char *assemblyFile;
    FILE *fptrBinary = NULL;
    FILE *fptrAssembly = NULL;
    int instrCount = 0;
    if (argc != 3) {
        printf("Usage: ./debugger {Binary File} {Assembly File}\n");
        return EXIT_FAILURE;
    }
    if (argv[1] != NULL && argv[2] != NULL) {
        assemblyFile = argv[1];
        binaryFile = argv[2];
        fptrAssembly = fopen(assemblyFile, "r");
        fptrBinary = fopen(binaryFile, "rb");
    }
    if (fptrAssembly != NULL && fptrBinary != NULL) {
        // Preperation to emulate
        binaryLoader(fptrBinary, binaryFile, data, MEM_SIZE, &instrCount);
        debug(data, instrCount);
        fclose(fptrAssembly);
        return EXIT_SUCCESS;
    }
    printf("File does not exist!\n");
    return EXIT_FAILURE;
}
#endif