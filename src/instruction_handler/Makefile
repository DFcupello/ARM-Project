CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic
LIB     =   libinhandler.a
LIBOBJS =   inhandler.o
BUILD	=   $(LIB)

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core

$(LIB):          $(LIBOBJS) 
		ar rcs $(LIB) $(LIBOBJS)

inhandler.o:	inhandler.h

