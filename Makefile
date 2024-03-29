###############################################################################
# Students: Evan Myers, Justin Slind, Alex Tai, James Yoo
# Course: CMPT-361
# Assignment #3 - ftp server
# File: Makefile
# Date: November 2013
###############################################################################
CC	=	gcc
CFLAGS	=	-g -pedantic -pthread -std=c99 -Wall -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE
LDFLAGS	=	-pthread


#main program


main: 	config.o cmdstrparser.o cmd_switch.o ctrlthread.o main.o md5.o net.o response.o \
servercmd.o session.o queue.o users.o cmd_stor.o cmd_misc.o cmd_retr.o path.o filemanip.o \
cmd_help.o
		$(CC) $(LDFLAGS) -o ftpd $^

main.o:		main.c config.h ctrlthread.h net.h servercmd.h


cmd_switch.o:	cmd_switch.c cmd_switch.h cmd_string_parser.h net.h session.h users.h \
cmd_misc.h users.h cmd_retr.h filemanip.h cmd_help.h cmd_stor.h
		$(CC) $(CFLAGS) -c -o $@ $<

cmdstrparser.o:	cmd_string_parser.c cmd_string_parser.h
		$(CC) $(CFLAGS) -c -o $@ $<

config.o:	config.c config.h

ctrlthread.o:	ctrlthread.c ctrlthread.h response.h session.h
		$(CC) $(CFLAGS) -c -o $@ $<

md5.o:		md5.c common.h md5.h

net.o:		net.c config.h net.h response.h

path.o:		path.c path.h response.h session.h

response.o:	response.c net.h response.h

servercmd.o:	servercmd.c config.h ctrlthread.h net.h servercmd.h

session.o:	session.c cmd_switch.h net.h session.h queue.h

queue.o:	queue.h queue.c

users.o:	users.c config.h md5.h net.h session.h users.h

cmd_stor.o: 	cmd_stor.c cmd_stor.h net.h path.h response.h session.h

filemanip.o: 	filemanip.c filemanip.h path.h net.h response.h session.h

cmd_misc.o: 	cmd_misc.c cmd_misc.h net.h session.h

cmd_retr.o:	cmd_retr.c cmd_retr.h net.h path.h response.h session.h
		$(CC) $(CFLAGS) -c -o $@ $<

cmd_help.o:	cmd_help.c cmd_help.h net.h session.h
		$(CC) $(CFLAGS) -c -o $@ $<


#Clean up the repository.
.PHONY:		clean
clean:
		$(RM) ftpd cmdstrparser.o cmd_switch.o config.o ctrlthread.o main.o md5.o \
		net.o path.o response.o servercmd.o session.o queue.o users.o filemanip.o \
		cmd_stor.o cmd_misc.o cmd_retr.o cmd_help.o
