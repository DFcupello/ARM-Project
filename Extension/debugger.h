#ifndef DEBUGGER
#define DEBUGGER

#define MAX_COMMAND_SIZE 551

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

// Command Execution
void printHelp(void);
void breakWatchFunc(priorityQueue *collection, char **tokens, int instrCount, bool isWatch, bool remove);
void printFunc(uint32_t data[], uint32_t registers[], char **tokens);
void runFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[], 
uint32_t instructions[], priorityQueue *breakpoints, priorityQueue *watchpoints);
void nextFunc(uint32_t data[], uint32_t registers[], uint32_t pipeline[],
uint32_t instructions[], bool hasRun, bool *printLine, priorityQueue *watchpoints);
void quitFunc(bool *debugDone);

// Main debugger function
void debug(uint32_t data[], int instrCount, char assemblyInstrs[][MAX_COMMAND_SIZE]);

#endif