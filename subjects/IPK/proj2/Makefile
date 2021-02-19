
#CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
CFLAGS=-std=gnu99 -g
CC=gcc
DEBUG_FLAG=

CLIENT_O=global.o ipk-client.o
SERVER_O=global.o ipk-server.o
ZIP_FILES=Makefile ipk-client.c ipk-server.c global.c global.h protokol.h debug.h dokumentace.md README.md .editorconfig


.PHONY: clean all

# all: ipk-client ipk-server
all: ipk-mtrip

debug: CFLAGS += -DDEBUG -g
debug: debug.h all

clean:
	rm -f ipk-mtrip;
	rm -f ipk-mtrip.o;
	rm -f global.o;
	rm -f xvalka05.zip

global.o: global.c global.h
	$(CC) $(CFLAGS) -c $(DEBUG_FLAG) -o $@ $<

ipk-mtrip.o: ipk-mtrip.c
	$(CC) $(CFLAGS) -c $(DEBUG_FLAG) -o $@ $<

ipk-mtrip: global.o ipk-mtrip.o
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $@ global.o ipk-mtrip.o

zip: $(ZIP_FILES)
	zip xvalka05.zip $(ZIP_FILES)


