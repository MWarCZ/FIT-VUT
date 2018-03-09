
#CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
CFLAGS=-std=gnu99 -g -DDEBUG
CC=gcc
NAME=forkme
DEBUG=-DDEBUG

aaa:
	./aaa.o 3000

bbb:
	./bbb.o 127.0.0.1 3000

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

