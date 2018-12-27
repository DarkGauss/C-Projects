#******************************
# Make file for dup2
# Made by Robert Goes (2018)
#******************************

CC = gcc
CFLAGS = -Wall -std=gnu99

.PHONY: all clean debug
#*****************************

all: clean dup2

debug: CFLAGS += -g
debug: dup2

clean:
	-rm -f *.o dup2
#******************************
dup2: main.o error.o trace.o utility.o files.o
	$(CC) $(CFLAGS) -o dup2 main.o error.o trace.o utility.o files.o

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