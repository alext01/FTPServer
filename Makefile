###############################################################################
# Students: Evan Myers, Justin Slind, Alex Tai, James Yoo (alphabetical)
# Course: CMPT-361
# Assignment #3 - ftp server
# File: Makefile
# Date: November 2013
###############################################################################
SHELL=/bin/bash

CC=gcc
CFLAGS=-g -pedantic -pthread -std=c99 -Wall -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L
LDFLAGS=-g -pthread

all: main

main: main.o

main.o: main.c linkedlist.h threadparam.h


#This portion of the makefile creates nettest: a place to test netcode.
nettest: fakemain.o net.o response.o
	$(CC) $(LDFLAGS) -o $@ $^
fakemain.o: fakemain.c net.h
net.o: net.c net.h response.h
response.o: response.c net.h response.h

.PHONY: clean
clean:
	$(RM) main main.o fakemain.o net.o response.o nettest