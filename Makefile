###############################################################################
# Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
# Course: CMPT-361
# Assignment #3 - ftp server
# File: Makefile
# Date: November 2013
###############################################################################
CC=gcc
CFLAGS=-g -pedantic -pthread -std=c99 -Wall -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L
LDFLAGS=-pthread

all: main

#main program
main: main.o net.o response.o config.o

config.o: config.c config.h
main.o: main.c linkedlist.h threadparam.h
net.o: net.c net.h response.h
response.o: response.c net.h response.h

#Clean up the repository.
.PHONY: clean
clean:
	rm main *.o