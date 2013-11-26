###############################################################################
# Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
# Course: CMPT-361
# Assignment #3 - ftp server
# File: Makefile
# Date: November 2013
###############################################################################
CC	=	gcc
CFLAGS	=	-g -pedantic -pthread -std=c99 -Wall -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L
LDFLAGS	=	-pthread

#main program

main: 	config.o cmdstrparser.o cmdswitch.o controlthread.o main.o net.o response.o servercmd.o session.o queue.o users.o


cmdswitch.o: 	cmd_switch.c cmd_switch.h cmd_string_parser.h net.h session.h users.h
		$(CC) $(CFLAGS) -c -o $@ $<

cmdstrparser.o:	cmd_string_parser.c cmd_string_parser.h
		$(CC) $(CFLAGS) -c -o $@ $<

config.o: config.c config.h

controlthread.o: controlthread.c controlthread.h session.h

main.o: main.c controlthread.h net.h servercmd.h

net.o: net.c config.h net.h response.h

response.o: response.c net.h response.h

servercmd.o: servercmd.c config.h controlthread.h net.h servercmd.h

session.o: session.c session.h queue.h

queue.o: queue.h queue.c

users.o: users.c users.h session.h net.h


#Clean up the repository.
.PHONY: clean
clean:

	$(RM) main cmdstrparser.o cmdswitch.o config.o controlthread.o main.o net.o response.o servercmd.o session.o queue.o users.o
