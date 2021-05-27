CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework -pg
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm -pg
BUILD	=	libs emulate testutilities intest assemble inassembler
OBJS	=	emulate.o utilities.o

all:	$(BUILD) 

clean:
		/bin/rm -f $(BUILD) *.o core 
		cd instruction_handler; make clean
		cd test_framework; make clean

libs:
		cd instruction_handler; make
		cd test_framework; make

assemble:       assemble.o symboltable.o
assemble.o:     symboltable.h
inassembler:    inassembler.o
inassembler.o:  inassembler.h
symboltable.o:  symboltable.c symboltable.h
emulate:		emulate.o utilities.o
emulate.o:		emulate.h utilities.h
utilities.o:	utilities.c utilities.h
testutilities:  testutilities.o utilities.o
testutilities.o: testutilities.c 
intest:         intest.o
intest.o:       intest.c

