CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Iinstruction_handler -Itest_framework
LDLIBS  = 	-Linstruction_handler -linhandler -Ltest_framework -ltestsuite -lm
BUILD	=	libs emulate testutilities
OBJS	=	emulate.o utilities.o 

all:	$(BUILD) 

clean:
		/bin/rm -f $(BUILD) *.o core 
		cd instruction_handler; make clean
		cd test_framework; make clean


libs:
		cd instruction_handler; make
		cd test_framework; make

emulate:		emulate.o utilities.o
emulate.o:		emulate.c utilities.h
utilities.o:	utilities.c utilities.h
testutilities:  testutilities.o utilities.o
testutilities.o: testutilities.c 
