LD = gcc
CFLAGS=-g3
CPPFLAGS = -I. -I$(HOME)/include/ -I$(HOME)/include/libxml2/ -I../Src -I/sc -I/sc/include -I/usr/include/libxml2/
SOURCES =  CUMain.c addTest.c 
LIBS = -L. -ltestfunctions -L../lib -lTIXI -L$(HOME)/lib -lcunit -lxml2

.SUFFIXES: .c .o
OBJECTS = $(SOURCES:.c=.o)

.c.$(O):
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $*.$(O) $<

all:
	cd ../Src; make -f Makefile.linux all
	make $(OBJECTS)
	cd TestFunctions; $(MAKE) all
	$(LD) -o runtests $(OBJECTS) $(LIBS) -L/sistec/lib 
clean:
	cd TestFunctions; $(MAKE) clean
	$(RM) *.o *.a
# DO NOT DELETE

addTest.o: testfunctions.h
