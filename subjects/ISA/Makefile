##########################################
##                                     ##
## Projekt: ISA RIP                   ##
## Soubor: Makefile                  ##
## Autor: Miroslav Valka (xvalka05) ##
##                                 ##
####################################

CC=gcc
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic

.PHONY: clean

all: myripsniffer myripresponse 

# Pokusy
test: test.o
	$(CC) $(CFLAGS) $< -o $@ -lpcap

test.o: test.c riplib.h
	$(CC) $(CFLAGS) -c $< -o $@

# Bod 2 
myripsniffer: myripsniffer.o
	$(CC) $(CFLAGS) $< -o $@ -lpcap 

myripsniffer.o: myripsniffer.c riplib.h
	$(CC) $(CFLAGS) -c $< -o $@

# Bod 3
myripresponse: myripresponse.o
	$(CC) $(CFLAGS) $< -o $@ -lpcap 

myripresponse.o: myripresponse.c riplib.h
	$(CC) $(CFLAGS) -c $< -o $@

tar:
	tar -cvf xvalka05.tar Makefile README myripresponse.c myripsniffer.c riplib.h manual.pdf

clean:
	rm -f *.o
	rm -f myripsniffer 
	rm -f myripresponse 

## Soubor: Makefile 
