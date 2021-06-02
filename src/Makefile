CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework -Werror -Idata_structures
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm -Ldata_structures -ldatastructures
BUILD	=	libs emulate testutilities intest assemble inassemblertest
OBJS	=	emulate.o inassembler.o inassemblertest.o assemble.o intest.o testutilities.o

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

assemble:          assemble.o inassembler.o
emulate:		   emulate.o
inassembler.o:     inassembler.h
testutilities:     testutilities.o 
testutilities.o:   testutilities.c 
inassemblertest:   inassembler.o inassemblertest.o
inassemblertest.o: inassemblertest.c 
intest:            intest.o
intest.o:          intest.c

