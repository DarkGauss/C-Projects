#******************************
# Make file for sim
# Made by Robert Goes (2018)
#******************************

CC = gcc
CFLAGS = -Wall -std=gnu99

.PHONY: all clean debug
#*****************************

all: clean sim

debug: CFLAGS += -g
debug: sim

clean:
	-rm -f *.o sim
#******************************
sim: main.o
	$(CC) $(CFLAGS) -o sim main.o
main.o: main.c
	$(CC) $(CFLAGS) -c -o main.o main.c
