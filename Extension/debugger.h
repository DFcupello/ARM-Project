#ifndef DEBUGGER
#define DEBUGGER

#define MAX_COMMAND_SIZE 511

#include "orderedset.h"

typedef enum Command {
    QUIT,
    RUN,
    HELP,
    NEXT,
    PRINT,
    BREAK,
    REMBREAK,
    WATCH,
    REMWATCH,
    TOKENERR
} command;

// Command Proccesing
int lexerCommand(char **tokens, int tokenSize);
char **readDebuggerCommand(command *cmd, int *tokenSize);
void getAssemblyInstrs(int instrCount, FILE *fptrAssembly, char assemblyInstrs[][MAX_COMMAND_SIZE]);

// Command Execution
void printHelp(void);
void breakFunc(orderedSet *collection, char **tokens, int instrCount, bool remove);
void watchFunc(orderedSet *collection, char **tokens, bool remove);
void printFunc(uint32_t data[], uint32_t registers[], char **tokens);
void runFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[], 
uint32_t instructions[], orderedSet *breakpoints);
void nextFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[],
uint32_t instructions[], orderedSet *watchpoints, bool hasRun);
void quitFunc(bool *debugDone);

// Main debugger function
void debug(uint32_t data[], int instrCount, char assemblyInstrs[][MAX_COMMAND_SIZE]);

#endif