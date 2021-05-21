CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic
LIB     =   libinhandler.a
LIBOBJS =   inhandler.o
BUILD	=	intest $(LIB)

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core

$(LIB):          $(LIBOBJS) 
		ar rcs $(LIB) $(LIBOBJS)

intest:	intest.o inhandler.o
intest.o:	inhandler.h
inhandler.o:	inhandler.h

