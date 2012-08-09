CC = gcc
CFLAGS=-g3
CPPFLAGS = -I. -I$(HOME)/include/ -I$(HOME)/include/libxml2/ -I../../Src -I/sc -I/sc/include -I/usr/include/libxml2/ -ITestFunctions -DG77_LINUX

SOURCES =  		  openDocument.c            \
                          closeDocument.c           \
                          getElement.c              \
                          removeElement.c           \
                          getAttribute.c            \
                          other.c                   \
                          createDocument.c          \
                          addElement.c              \
                          addComplexElements.c      \
                          addAttribute.c	    \
			  externaldataOpenMode.c    \
			  xslt.c			\
			  xpaths.c			\
			  validation.c			\
			  externaldataOpenMode.c	\
			  internal.c			\
			  vectors.c			\
		   	  arrays.c			\
		   	  fortranInterface.c		\
			  removeAttributes.c \
			  uid.c

LIB = ../libtestfunctions.a

.SUFFIXES: .c .o
OBJECTS = $(SOURCES:.c=.o)

.c.$(O):
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $*.$(O) $<

all:        $(LIB)


$(LIB):	$(OBJECTS)
	ar r $(LIB) $(OBJECTS) 
clean:
	$(RM) *.o
# DO NOT DELETE

addComplexElements.o: ../../Src/tixi.h ../../Src/tixiData.h
addElement.o: ../../Src/tixi.h ../../Src/tixiData.h
closeDocument.o: ../../Src/tixi.h ../../Src/tixiData.h
createDocument.o: ../../Src/tixi.h ../../Src/tixiData.h
getAttribute.o: ../../Src/tixi.h ../../Src/tixiData.h
getElement.o: ../../Src/tixi.h ../../Src/tixiData.h
removeElement.o: ../../Src/tixi.h ../../Src/tixiData.h
openDocument.o: ../../Src/tixi.h ../../Src/tixiData.h ../../Src/webMethods.h
other.o: ../../Src/tixi.h ../../Src/tixiData.h
externaldataOpenMode.o: ../../Src/tixi.h ../../Src/tixiData.h
removeAttributes.o: ../../Src/tixi.h ../../Src/tixiData.h
xpaths.o:../../Src/tixi.h ../../Src/tixiData.h                     
validation.o:../../Src/tixi.h ../../Src/tixiData.h                
externaldataOpenMode.o:../../Src/tixi.h ../../Src/tixiData.h     
xslt.o:../../Src/tixi.h ../../Src/tixiData.h   
internal.o:../../Src/tixi.h ../../Src/tixiData.h
vectors.o:../../Src/tixi.h ../../Src/tixiData.h
arrays.o:../../Src/tixi.h ../../Src/tixiData.h
fortranInterface.o:../../Src/tixi.h ../../Src/tixiData.h


