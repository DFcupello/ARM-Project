CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic -Werror
LIB     =   libinhandler.a
LIBOBJS =   inhandler.o execute.o utilities.o
BUILD	=   $(LIB)

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core

$(LIB):          $(LIBOBJS) 
		ar rcs $(LIB) $(LIBOBJS)

inhandler.o:	inhandler.h utilities.h
utilities.o:    utilities.h
execute.o:      execute.h

