CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework -Idata_structures
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm -Ldata_structures -ldatastructures
BUILD	=	libs emulate testutilities intest assemble inassemblertest
OBJS	=	emulate.o utilities.o symboltable.o inassembler.o inassemblertest.o assemble.o

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

assemble:       assemble.o symboltable.o inassembler.o
assemble.o:     symboltable.h
inassembler.o:  symboltable.o inassembler.c inassembler.h
symboltable.o:  symboltable.c symboltable.h
emulate:		emulate.o utilities.o
emulate.o:		emulate.h utilities.h
utilities.o:	utilities.c utilities.h
testutilities:  testutilities.o utilities.o
testutilities.o: testutilities.c 
inassemblertest: inassembler.o inassemblertest.o symboltable.o
inassemblertest.o: inassemblertest.c 
intest:         intest.o
intest.o:       intest.c

