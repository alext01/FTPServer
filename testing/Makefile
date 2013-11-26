#Testing makefile. Do not submit this.
CC=gcc
CFLAGS=-g -pedantic -pthread -std=c99 -Wall -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L
LDFLAGS=-g -pthread

all: net.test #config.test 

#Test connections, sending/recieving data, PASV and PORT command.
net.test:  netmain.o ../config.o ../net.o ../response.o
	$(CC) $(LDFLAGS) -o $@ $^
netmain.o: netmain.c ../net.h

#Test the config file.
#config.test:  confmain.o ../config.o
#	$(CC) $(LDFLAGS) -o $@ $^
#confmain.o: confmain.c ../config.h

#o-files
../config.o: ../config.c ../config.h
../net.o: ../net.c ../net.h ../response.h
../response.o: ../response.c ../net.h ../response.h


.PHONY: clean
clean:
	rm net.test config.test *.o