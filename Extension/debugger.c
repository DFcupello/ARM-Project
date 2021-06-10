#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "emulate.h"
#include "utilities.h"
#include "assemble.h"
#include "inassembler.h"
#include "inhandler.h"

#define MAX_COMMAND_SIZE 551

enum Command {
    QUIT,
    RUN,
    HELP,
    NEXT,
    PRINT,
    BREAK,
    WATCH,
    TOKENERR
};
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
int parseCommand(char **tokens, int tokenSize) {
    assert(tokenSize >= 2 && tokenSize <= 3);
    if (tokenSize == 2 || tokenSize == 3) {
        if (strlen(tokens[1]) == 1) {
            switch (tokens[1][0]) {
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
                default: return TOKENERR;
            }
        }
    }
    return TOKENERR;
}

/*

*/
void readDebuggerCommand() {
    printf("> ");
    char answer[MAX_COMMAND_SIZE];
    scanf("%s", answer);
    int tokenSize = getTokenSize(answer);
    char *tokens[tokenSize];
    tokenizer(answer, tokenSize, tokens);

}

#ifdef MAIN_DEBUGGER
int main(int argc, char *argv[]) {
    uint32_t registers[REG_SIZE] = {0};
    uint32_t data[MEM_SIZE] = {0};
    char *binaryFile = "temp.bin";
    char *assemblyFile;
    FILE *fptrBinary = NULL;
    FILE *fptrAssembly = NULL;
    int instrCount = 0;
    if (argc != 2) {
        printf("Usage: ./debugger {Assembly File}\n");
        return EXIT_FAILURE;
    }
    if (argv[1] != NULL) {
        assemblyFile = argv[1];
        fptrAssembly = fopen(assemblyFile, "r");
        fptrBinary = fopen(binaryFile, "wb");
    }
    if (fptrAssembly != NULL) {
        // Assembling
        assemble(fptrAssembly, fptrBinary);
        fptrBinary = fopen(binaryFile, "rb");
        fptrAssembly = fopen(assemblyFile, "r");
        // Preperation to emulate
        binaryLoader(fptrBinary, binaryFile, data, MEM_SIZE, &instrCount);
        uint32_t bigData[instrCount + 1];
        for (int i = 0; i < instrCount; i++) {
            bigData[i] = littleEndToBigEnd(data[i]);
        }
        // The halt instruction
        bigData[instrCount] = 0;
        //fetch - decode - execute
        uint32_t pipeline[] = {-1, -1};

        // Preperation for debugging
        int breakpoints[instrCount];
        int watchpoints[instrCount];

        
        while (emulateInstruction(data, registers, bigData, pipeline));
        printEndState(data, registers);
        fclose(fptrBinary);
        fclose(fptrAssembly);
        remove("temp.bin");
        return EXIT_SUCCESS;
    }
    printf("File does not exist!\n");
    remove("temp.bin");
    return EXIT_FAILURE;
}
#endif