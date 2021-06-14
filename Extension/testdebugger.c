#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "testsuite.h"
#include "emulate.h"
#include "utilities.h"
#include "debugger.h"

void testDebugger(int testNum, char *fileName, char *testName) {
    uint32_t data[MEM_SIZE] = {0};
    char base[] = "test_cases/";
    char binaryFile[strlen(fileName) + strlen(base) + 1];
    strcpy(binaryFile, base);
    strcat(binaryFile, fileName);
    char assemblyFile[strlen(binaryFile) + 3];
    strcpy(assemblyFile, binaryFile);
    strcat(assemblyFile, ".s");
    FILE *fptrBinary = fopen(binaryFile, "rb"); 
    FILE *fptrAssembly = fopen(assemblyFile, "r");
    int instrCount = 0;
    binaryLoader(fptrBinary, binaryFile, data, MEM_SIZE, &instrCount);
    char assemblyInstrs[instrCount][MAX_COMMAND_SIZE];
    getAssemblyInstrs(instrCount, fptrAssembly, assemblyInstrs);

    fclose(fptrAssembly);
    char inputName[] = "test_files/Input1.txt";
    inputName[16] = testNum + '0';
    char outputName[] = "test_files/Output1.txt";
    outputName[17] = testNum + '0';
    char resultName[] = "test_files/Result1.txt";
    resultName[17] = testNum + '0';
    FILE *fin = freopen(inputName, "r", stdin);
    FILE *fout = freopen(outputName, "w+", stdout);
    debug(data, instrCount, assemblyInstrs);
    FILE *fresult = fopen(resultName, "r");
    fclose(fin);
    fclose(fout);
    freopen("/dev/tty", "w", stdout);
    fout = fopen(outputName, "r");
    testTxtFile(fout, fresult, testName);
    fclose(fresult);
    fclose(fout);  
}

void testBreakFunc(void) {
    testDebugger(1, "bne01", "Breakpoints Test");
}

void testWatchFunc(void) {
    testDebugger(2, "add01", "Watchpoints Test");
}

void testQuitAndHelp(void) {
    testDebugger(3, "factorial", "Quit and Help Test");
}

void testPrint(void) {
    testDebugger(4, "factorial", "Print Test");
}

void testEverything(void) {
    testDebugger(5, "factorial", "Debugger Test");
}

#ifdef MAIN_TESTDEBUGGER
int main(void) {
    testBreakFunc();
    testWatchFunc();
    testQuitAndHelp();
    testPrint();
    testEverything();
    return EXIT_SUCCESS;
}
#endif