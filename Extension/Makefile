CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework -Idata_structures
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm -Ldata_structures -ldatastructures
BUILD	=	libs emulate testutilities intest debugger inassemblertest assemble testdebugger
OBJS	=	emulate.o intest.o testutilities.o assemble.o inassemblertest.o inassembler.o
SRCS    =   intest.c testutilities.c emulate.c debugger.c

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD) 

clean:
		/bin/rm -f $(BUILD) *.o core 
		cd instruction_handler; make clean
		cd test_framework; make clean
		cd data_structures; make clean

libs:
		cd instruction_handler; make
		cd test_framework; make
		cd data_structures; make 

emulate:	emulate.c emulate.h
	$(CC) $(CFLAGS) -DMAIN_EMULATE -o emulate emulate.c $(LDLIBS)

assemble:	assemble.c assemble.h inassembler.c inassembler.h
	$(CC) $(CFLAGS) -DMAIN_ASSEMBLE -o assemble assemble.c inassembler.c $(LDLIBS)
debugger:	emulate.h emulate.c assemble.h assemble.c debugger.c debugger.h inassembler.c inassembler.h
	$(CC) $(CFLAGS) -DMAIN_DEBUGGER -o debugger debugger.c emulate.c assemble.c inassembler.c $(LDLIBS)
testdebugger: debugger.c debugger.h testdebugger.c emulate.c emulate.h inassembler.c inassembler.h
	$(CC) $(CFLAGS) -DMAIN_TESTDEBUGGER -o testdebugger testdebugger.c debugger.c emulate.c inassembler.c $(LDLIBS)

inassembler.o:     inassembler.h
testutilities:     testutilities.o 
testutilities.o:   testutilities.c  
intest:            intest.o
intest.o:          intest.c
inassemblertest:   inassembler.o inassemblertest.o
inassemblertest.o: inassemblertest.c 


