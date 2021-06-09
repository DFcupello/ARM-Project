CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework -Werror -Idata_structures
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm -Ldata_structures -ldatastructures
BUILD	=	libs emulate testutilities intest debugger inassemblertest assemble
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

assemble:          assemble.o inassembler.o
inassembler.o:     inassembler.h
testutilities:     testutilities.o 
testutilities.o:   testutilities.c  
intest:            intest.o
intest.o:          intest.c
inassemblertest:   inassembler.o inassemblertest.o
inassemblertest.o: inassemblertest.c 
debugger:	emulate.h emulate.c debugger.c
	$(CC) $(CFLAGS) -DMAIN_DEBUGGER -o debugger debugger.c emulate.c $(LDLIBS)

