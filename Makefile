SHELL=/bin/bash

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -pthread -g
LDFLAGS=-g -pthread

all: main

main: main.o

main.o: main.c linkedlist.h threadparam.h



clean:
	$(RM) main main.o