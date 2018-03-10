
#CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
CFLAGS=-std=gnu99 -g
CC=gcc
NAME=forkme
DEBUG_FLAG=

CLIENT_O=global.o ipk-client.o
SERVER_O=global.o ipk-server.o

.PHONY: clean all

all: ipk-client ipk-server

debug: CFLAGS += -DDEBUG -g
debug: debug.h all

clean:
	rm -f ipk-client;
	rm -f ipk-server;
	rm -f ipk-client.o;
	rm -f ipk-server.o;
	rm -f global.o;

global.o: global.c global.h protokol.h
	$(CC) $(CFLAGS) -c $(DEBUG_FLAG) -o $@ $<

ipk-client.o: ipk-client.c  protokol.h
	$(CC) $(CFLAGS) -c $(DEBUG_FLAG) -o $@ $<

ipk-server.o: ipk-server.c  protokol.h
	$(CC) $(CFLAGS) -c $(DEBUG_FLAG) -o $@ $<

ipk-client: $(CLIENT_O)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $(CLIENT_O)

ipk-server: $(SERVER_O)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ $(SERVER_O)

