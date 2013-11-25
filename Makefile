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

#main program
main: config.o controlthread.o main.o net.o response.o servercmd.o

config.o: config.c config.h
controlthread.o: controlthread.c controlthread.h session.h queue.h
main.o: main.c controlthread.h net.h servercmd.h
net.o: net.c config.h net.h response.h
response.o: response.c net.h response.h
servercmd.o: servercmd.c config.h net.h servercmd.h
session.o: session.c session.h queue.h
queue.o: queue.h queue.c

#Clean up the repository.
.PHONY: clean
clean:
	rm main config.o controlthread.o main.o net.o response.o servercmd.o
