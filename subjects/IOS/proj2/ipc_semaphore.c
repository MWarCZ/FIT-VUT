#ifndef IPC_SEMAPHORE_C
#define IPC_SEMAPHORE_C

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>

#include <sys/shm.h>
#include <sys/stat.h>

#include <sys/ipc.h>
#include <sys/sem.h>

#include "ipc_semaphore.h"


/* ziskani id semaforu ke klici key, nebo alokace noveho */
int semaphore_alloc(key_t key, int sem_flags)
{
  return semget(key, 1, sem_flags);
}

/* dealokace semaforu */
int semaphore_dealloc(int semid)
{
  union semun ignored_arg;
  
  return semctl(semid, 1, IPC_RMID, ignored_arg);
}

/* inicilizace semaforu */
int semaphore_init(int semid, int value)
{
  union semun arg;
  unsigned short values[1];
  values[0] = value;
  arg.array = values;
  return semctl(semid, 0, SETALL, arg);
}

/* Funkce pro operaci wait */
int semaphore_wait(int semid)
{
  struct sembuf operations[1];

  operations[0].sem_num = 0;
  operations[0].sem_op = -1;
  operations[0].sem_flg = SEM_UNDO;
  
  return semop(semid, operations, 1);
}

/* operace wait s hodnotou */
int semaphore_wait_value(int semid, int value)
{
  struct sembuf operations[1];

  operations[0].sem_num = 0;
  operations[0].sem_op = -value;
  operations[0].sem_flg = SEM_UNDO;
  
  return semop(semid, operations, 1);
}
/* operace ceka na 0*/
int semaphore_wait_to_zero(int semid)
{
  struct sembuf operations[1];

  	operations[0].sem_num = 0;
  	operations[0].sem_op = 0;
  	operations[0].sem_flg = SEM_UNDO;
  
  return semop(semid, operations, 1);
}
/* operace post */
int semaphore_post(int semid)
{
  struct sembuf operations[1];

  operations[0].sem_num = 0;
  operations[0].sem_op = 1;
  operations[0].sem_flg = SEM_UNDO;
  
  return semop(semid, operations, 1);
}
/* operace post s hodnotou */
int semaphore_post_value(int semid, int value)
{
  struct sembuf operations[1];

  operations[0].sem_num = 0;
  operations[0].sem_op = value;
  operations[0].sem_flg = SEM_UNDO;
  
  return semop(semid, operations, 1);
}


#endif