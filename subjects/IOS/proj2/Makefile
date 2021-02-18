
CFLAGS=-std=gnu99 -Wall -Wextra -pedantic -g
CC=gcc
NAME=proj2

all: proj2

proj2: proj2.o 
	cp $^ $@

#main.o: main.c ipc_semaphore.o
#	$(CC) $(CFLAGS) $^ -o $@

proj2.o: proj2.c ipc_semaphore.o
	$(CC) $(CFLAGS) $^ -o $@

ipc_semaphore.o: ipc_semaphore.c ipc_semaphore.h
	$(CC) $(CFLAGS) -c $< -o $@

failed:
	./clean_after_failed.sh

check:
	./check-syntax.sh proj2.out

kill: 
	sudo pkill proj2.o

clear:
	rm -f *.o
	rm -f proj2
	rm -f proj2.out
