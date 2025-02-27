CC = gcc
CFLAGS = -Wall -g  # Compiler flags: -Wall (enable all warnings), -g (enable debugging)

all: sheet

sheet: clab/init.o clab/dependent.o clab/display.o
	$(CC) $(CFLAGS) clab/init.o clab/dependent.o clab/display.o -o sheet

clab/init.o: clab/init.c clab/init.h clab/display.h clab/io.h clab/process.h clab/dependent.h
	$(CC) $(CFLAGS) -c clab/init.c -o clab/init.o

clab/dependent.o: clab/dependent.c clab/dependent.h clab/init.h clab/io.h clab/process.h clab/stack.h
	$(CC) $(CFLAGS) -c clab/dependent.c -o clab/dependent.o

clab/display.o: clab/display.c clab/display.h clab/init.h
	$(CC) $(CFLAGS) -c clab/display.c -o clab/display.o

clean:
	rm -f sheet clab/*.o