#******************************
# Make file for ftor
# Made by Robert Goes (2018)
#******************************

CC = gcc
CFLAGS = -Wall -std=gnu99
MAIN_INCLUDES = error.o trace.o utility.o files.o runs.o general.o freq.o chksum.o
.PHONY: all clean debug
#*****************************

all: clean ftor

debug: CFLAGS += -g
debug: ftor

clean:
	-rm -f *.o ftor
#******************************
ftor: main.o $(MAIN_INCLUDES)
	$(CC) $(CFLAGS) -o ftor main.o $(MAIN_INCLUDES)

main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

error.o: error.c error.h trace.o
	$(CC) $(CFLAGS) -c -o error.o error.c

trace.o: trace.c trace.h
	$(CC) $(CFLAGS) -c -o trace.o trace.c

utility.o: utility.c utility.h error.o trace.o
	$(CC) $(CFLAGS) -c -o utility.o utility.c

files.o: files.c files.h error.o utility.o trace.o
	$(CC) $(CFLAGS) -c -o files.o files.c

runs.o: runs.c runs.h error.o utility.o trace.o
	$(CC) $(CFLAGS) -c -o runs.o runs.c

general.o: general.c general.h error.o trace.o
	$(CC) $(CFLAGS) -c -o general.o general.c

freq.o: freq.c freq.h error.o utility.o trace.o
	$(CC) $(CFLAGS) -c -o freq.o freq.c

chksum.o: chksum.c chksum.h error.o utility.o trace.o
	$(CC) $(CFLAGS) -c -o chksum.o chksum.c