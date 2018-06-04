
#ifndef IPC_SEMAPHORE_H
#define IPC_SEMAPHORE_H

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>

#include <sys/shm.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/sem.h>

/**
 * Kontrola definovani union semun
 */
#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
  // union semun by mela byt deklarovana v <sys/sem.h> 
#else
  // Pokud neni deklarujeme
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};
#endif

/* ziskani id semaforu ke klici key, nebo alokace noveho */
int semaphore_alloc(key_t key, int sem_flags);

/* dealokace semaforu */
int semaphore_dealloc(int semid);

/* inicilizace semaforu */
int semaphore_init(int semid, int value);

/* Funkce pro operaci wait */
int semaphore_wait(int semid);

/* operace wait s hodnotou */
int semaphore_wait_value(int semid, int value);

/* operace ceka na 0*/
int semaphore_wait_to_zero(int semid);

/* operace post */
int semaphore_post(int semid);

/* operace post s hodnotou */
int semaphore_post_value(int semid, int value);


#endif