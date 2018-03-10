
#CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
CFLAGS=-std=gnu99 -g
CC=gcc
NAME=forkme
DEBUG_FLAG=

.PHONY: clean all

all: ipk-client ipk-server

debug: CFLAGS += -DDEBUG -g
debug: debug.h all

clean:
	rm ipk-client;
	rm ipk-server;
	rm ipk-client.o;
	rm ipk-server.o;
	rm global.o;

global.o: CFLAGS += -c 
global.o: global.c global.h protokol.h
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $<

ipk-client.o: CFLAGS += -c 
ipk-client.o: ipk-client.c  protokol.h
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $<

ipk-server.o: CFLAGS += -c 
ipk-server.o: ipk-server.c  protokol.h
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $<

ipk-client: global.o ipk-client.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $^

ipk-server: global.o ipk-server.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $<

