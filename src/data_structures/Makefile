CC      = 	gcc
CFLAGS  = 	-Wall -g -D_POSIX_SOURCE -std=c99 -pedantic
LIB     =   libdatastructures.a
LIBOBJS =   fifoimpl.o symtableimpl.o
BUILD	=   $(LIB)

.SUFFIXES: .c .o

.PHONY: all clean

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core

$(LIB):          $(LIBOBJS) 
		ar rcs $(LIB) $(LIBOBJS)

fifoimpl.o:	fifos.h fifoimpl.c
symtableimpl.o: symtable.h symtableimpl.c
