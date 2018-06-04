#ifndef PROJ2_C
#define PROJ2_C

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include <sys/ipc.h>

#include "ipc_semaphore.h"


// GLOBALNI PROMENE

/// SEMAFORY
int sem_Speak; /// Mohu mluvit?
  // zamika: shm_addr[3]; stdout;
int sem_ProcesCounter;
  // zamika: shm_addr[0]
int sem_EndNow; /// Muzete koncit 
int sem_AllEnd;  /// Uz jsme skoncili papa
int sem_EnterOrLeave; /// Mohu vejit?  
  // zamika: shm_addr[1],shm_addr[2], vstup novych procesu a odchod adult procesu
int sem_CapCenter; /// 1-Kapacita centra

//+ SDILENA PAMET
int shm_id; //+ Identifikator sdilene pameti
int *shm_addr; //+ Adresa sdilene pameti
  //+ [0]-Pocet vsech procesu
  //+ [1]-Pocet dospelich v centru = CA
  //+ [2]-Pocet deti v centru = CC
  //+ [3]-Cislo akce = A
  //+ [4]-Pocet dospelich ktery pracuji nebo nebyli vygenerovani. 0=Neprijde zadny novy Adult
  
FILE *file_output; // Vystupni soubor 

/**
 * Funkce pro "bezpecny" textovy vystup aplikace. Vypisuje informace v zakladnim formatu. ( A : Name I : Action ) 
 * @param name   Nazev volajiciho. (pr. A, C)
 * @param id     Identifikator volajiciho.
 * @param action Nazev akce.
 */
void Speak(char *name, int id, char *action)
{
  semaphore_wait(sem_Speak); // Mohu mluvit?
    shm_addr[3]++; // Pocitadlo promluv
    printf("%d : %s %d : %s\n",shm_addr[3], name, id, action);
    fprintf(file_output,"%d : %s %d : %s\n",shm_addr[3], name, id, action);
    
  semaphore_post(sem_Speak); // Domluvil jsem.
}
/**
 * Funkce pro "bezpecny" textovy vystup aplikace. Vypisuje informace v rozsirenem formatu. ( A : Name I : Action : CA : CC )
 * @param name   Nazev volajiciho (pr. A, C)
 * @param id     Identifikator volajiciho.
 * @param action Nazev akce.
 */
void SpeakCount(char *name, int id, char *action)
{
  semaphore_wait(sem_Speak); // Mohu mluvit?
    shm_addr[3]++; // Pocet promluv
    printf("%d : %s %d : %s : %d : %d\n",shm_addr[3], name, id, action,shm_addr[1], shm_addr[2]);
    fprintf(file_output,"%d : %s %d : %s : %d : %d\n",shm_addr[3], name, id, action,shm_addr[1], shm_addr[2]);
  semaphore_post(sem_Speak); // Domluvil jsem.
}
/**
 * Proces odebere sebe z pocitadla a pokud je posledni, tak da vedet hlavnimu procesu odemknutim semaforu sem_AllEnd.
 */
void AmILastProcess()
{
  semaphore_wait(sem_ProcesCounter);
    int tmp = --shm_addr[0]; // Odebrani sebe z citace procesu
  semaphore_post(sem_ProcesCounter);

  if(tmp == 0) // Jsem posledni? Jestli ano oznam to.
  {
    semaphore_post(sem_AllEnd); 
  }
}
/**
 * Funkce pro zajisteni snchronizace ukonceni vsech procesu.
 * Po otevreni semforu sem_EndNow vypise text, zkontroluje zda je poslednim procesem a nasledni odpoji sdilenou pamet.
 * @param name   [description]
 * @param id     [description]
 * @param action [description]
 */
void SpeakWaitToDie(char *name, int id, char *action)
{
  // Ukoncovaci sekvence procesu
  semaphore_wait(sem_EndNow); // Uz mohu skoncit?
    Speak(name, id, action);

    AmILastProcess(); // jsem posledni kod umira?

    shmdt(shm_addr); // odpojeni sdilene pameti

  semaphore_post(sem_EndNow);
}

/**
 * Hlavni cast procesu Adult. 
 * @param  myid     Identifikator procesu
 * @param  sleep_ms Doba po kterou bude simulovat praci.
 */
void Adult(int myid, int sleep_ms)
{

  Speak("A",myid,"started");

  // Etapa: vchazeni
  semaphore_wait(sem_EnterOrLeave); // Mohu vejit?
    Speak("A",myid,"enter");
    ++shm_addr[1];  // Zviseni poctu dospelich v centru.
    semaphore_post_value(sem_CapCenter, 3); // Zviseni kapacity centra
  semaphore_post(sem_EnterOrLeave);

  // Etapa: Vykon prace
  usleep(sleep_ms);

  // Etapa: Odchod
  semaphore_wait(sem_EnterOrLeave);  // Mohu Odejit?
    Speak("A",myid,"trying to leave");

    if( (shm_addr[1]*3-shm_addr[2]) < 3)  // Musim cekat na odejiti?
    {
      SpeakCount("A",myid,"waiting");
      do{
        semaphore_post(sem_EnterOrLeave);  // Vracim moznost vchazet/odchazet do centra
          usleep(0);  // Inspirace z prednasky :D vyuzivam to jako ochranu pred tym, aby proces namohl sebrat semafor sem_EnterOrLeave ihned pote co ho vratil.
        semaphore_wait(sem_EnterOrLeave);  // Nekdo uvolnil semafor. Zkontroluj podminku, mozna budes moct odejit.
      }while(shm_addr[1]*3-shm_addr[2] < 3);  // Cekej dokud nebudes moct odejit
    }

    semaphore_wait_value(sem_CapCenter, 3); // Sniz kapacitu centra
    --shm_addr[1]; // Sniz pocet procesu Adult v centru
    Speak("A",myid,"leave");
    shm_addr[4]--; // Sniz pocet procesu Adult z celkoveho poctu moznych.
  semaphore_post(sem_EnterOrLeave);

  AmILastProcess(); // jsem posledni kod pracoval?

  SpeakWaitToDie("A",myid,"finished"); // cekej na povel smrti a rekni umiram
  
  exit(0);
}

/**
 * Hlavni cast procesu Child. 
 * @param  myid     Identifikator procesu
 * @param  sleep_ms Doba po kterou bude simulovat praci.
 */
void Child(int myid, int sleep_ms)
{
  Speak("C",myid,"started");

  // Etapa: vchazeni
  semaphore_wait(sem_EnterOrLeave); // Mohu vejit?

    if( (shm_addr[1]*3-shm_addr[2]) <= 0 ) // Je v centru dostatechna kapacita na to abych vesel?
    {
      SpeakCount("C",myid,"waiting");
      do{
        semaphore_post(sem_EnterOrLeave);  // Vracim moznost vchazet/odchazet do centra
        usleep(0); // viz Adult. Zabraneni aby proces se nezaciklil.
        semaphore_wait(sem_EnterOrLeave); // Nekdo uvolnil semafor. Zkontroluj podminku, mozna budes moct vejit.

        if(shm_addr[4] <= 0)  // Pokud jiz nepridou nove procesy Adult tak se pust sam. 
        {
          semaphore_post(sem_CapCenter);
          break;
        }
      }while( (shm_addr[1]*3-shm_addr[2] <= 0 ) ); // Cekej dokud nebudes moct vejit.
       
    }
    semaphore_wait(sem_CapCenter); // Sniz kapacitu centra
    ++shm_addr[2]; // Zvis pocet deti v centru
    Speak("C",myid,"enter");
  semaphore_post(sem_EnterOrLeave);
  
  // Etapa: Vykon prace
  usleep(sleep_ms);

  // Etapa: Odchod
  semaphore_wait(sem_EnterOrLeave);
    Speak("C",myid,"trying to leave");
    --shm_addr[2]; // Sniz pocet deti v centru
    semaphore_post(sem_CapCenter);
    Speak("C",myid,"leave");

  semaphore_post(sem_EnterOrLeave);


  AmILastProcess(); // jsem posledni kod pracoval?

  SpeakWaitToDie("C",myid,"finished"); // cekej na povel smrti a rekni umiram
  
  exit(0);
}

/**
 * Hlavni cast procesu pro generovani procesu Adult.
 * @param countAdult    Pocet procesu Adult k vygenerovani.
 * @param genTimeAdult  Horni hranice rozptylu mezi vygenerovanim procesu Adult.
 * @param workTimeAdult Horni hranice pro dobu prace vygenerovanych procesu Adult.
 */
void AdultGenerator(int countAdult, int genTimeAdult, int workTimeAdult)
{
  int rand_ms;

  for(int i = 1; i<=countAdult; i++)
  {
    if(genTimeAdult > 0)  
      usleep(rand()%genTimeAdult);

    rand_ms = (workTimeAdult > 0) ? (rand()%workTimeAdult) : 0;

    if(fork() == 0)
    {
      Adult(i,rand_ms);
    } 
  }

  exit(0);
}

/**
 * Hlavni cast procesu pro generovani procesu Child.
 * @param countChild    Pocet procesu Child k vygenerovani.
 * @param genTimeChild  Horni hranice rozptylu mezi vygenerovanim procesu Child.
 * @param workTimeChild Horni hranice pro dobu prace vygenerovanych procesu Child.
 */
void ChildGenerator(int countChild, int genTimeChild, int workTimeChild)
{
  int rand_ms;

  for(int i = 1; i<=countChild; i++)
  {
    if(genTimeChild > 0)
      usleep(rand()%genTimeChild);

    rand_ms = (workTimeChild > 0) ? (rand()%workTimeChild) : 0;

    if(fork() == 0)
    {
      Child(i,rand_ms);
    }
  }

  exit(0);
}

/**
 * Funkce alokuje a inicializuje veskere potrebne veci jako je: 
 *   Vypnuti "buffrovani" vystupu
 *   Zalozeni a nastaveni semaforu a sdilene pameti
 *   Vytvoreni a otevreni souboru pro vypis
 */
void InitApp()
{
	setbuf(stdout,NULL);
  setbuf(stderr,NULL);

  char *path = "/tmp/xvalka05_ios_proj2";

  // Vytvoreni pomocneho souboru pro vygenerovani klice
  fclose(fopen(path, "w+"));

  // Otevreni a nastaveni souboru pro vypis.
  file_output = fopen("proj2.out", "w+");
  setbuf(file_output,NULL);


  /// Alokace semaforu
  sem_Speak = semaphore_alloc( ftok( path,'a'), IPC_CREAT | 0660);
  sem_EndNow = semaphore_alloc( ftok( path,'b'), IPC_CREAT | 0660);
  sem_AllEnd = semaphore_alloc( ftok( path,'c'), IPC_CREAT | 0660);
  sem_EnterOrLeave = semaphore_alloc( ftok( path,'d'), IPC_CREAT | 0660);
  sem_ProcesCounter = semaphore_alloc( ftok( path,'e'), IPC_CREAT | 0660);
  sem_CapCenter = semaphore_alloc( ftok( path,'f'), IPC_CREAT | 0660);
  

  /// Inicializace semaforu   // 0-zamceno, 1-odemceno
  semaphore_init(sem_Speak, 1); // mluveni povoleno
  semaphore_init(sem_EndNow, 0); // konec zamitnut
  semaphore_init(sem_AllEnd, 0); // vsichni skoncili - jeste ne
  semaphore_init(sem_EnterOrLeave, 1); // vstup do centra povolen
  semaphore_init(sem_ProcesCounter, 1); // menit pocet vsech procesu povoleno
  semaphore_init(sem_CapCenter, 0); // kapacita centra

  //+ vytvoreni sdilene pameti 
  shm_id = shmget(ftok(path,'g'), 1024, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  //+ Pripojeni sdilene pameti (kazdy proces zvlast.)
  shm_addr = (int *)shmat(shm_id, NULL, 0);

}
/**
 * Funkce dealokuje a deinicializuje veskere veci jako je: 
 *   Zavreni a odstraneni pomocneho souboru pro generovani klice
 *   Dealokoci semaforu a sdilene pameti
 */
void DeinitApp()
{

  fclose(file_output); // Zavreni souboru
  remove("/tmp/xvalka05_ios_proj2"); // odstraneni pomocneho souboru

  //+ odpojeni sdilene pameti
  shmdt(shm_addr);
  //+ dealokovani sdilene pameti
  shmctl(shm_id, IPC_RMID, NULL);

  /// Dealokace semaforu
  semaphore_dealloc(sem_CapCenter);
  semaphore_dealloc(sem_ProcesCounter);
  semaphore_dealloc(sem_AllEnd);
  semaphore_dealloc(sem_EnterOrLeave);
  semaphore_dealloc(sem_EndNow);
  semaphore_dealloc(sem_Speak);

}

/**
 * Hlavni cast prvniho proceshu, ktery zavola inicializaci, vytvori generatory a nasledne ceka na dokonceni vsech akci. Pote zavola deinicializaci a konci.
 * @param  countAdult    Pocet procesu Adult ke generovani.
 * @param  countChild    Pocet procesu Chilt ke generovani.
 * @param  genTimeAdult  Horni hranice rozptylu mezi vygenerovanim procesu Adult.
 * @param  genTimeChild  Horni hranice rozptylu mezi vygenerovanim procesu Child.
 * @param  workTimeAdult Horni hranice pro dobu prace vygenerovanych procesu Adult.
 * @param  workTimeChild Horni hranice pro dobu prace vygenerovanych procesu Child.
 */
int First( int countAdult, int countChild, int genTimeAdult, int genTimeChild, int workTimeAdult, int workTimeChild)
{

  InitApp(); 

  shm_addr[0] = countAdult + countChild; //+ [0]-Pocet vsech procesu
  shm_addr[1] = 0; //+ [1]-pocet dospelich v centru = CA
  shm_addr[2] = 0; //+ [2]-Pocet deti v centru = CC
  shm_addr[3] = 0; //+ [3]-Cislo akce = A
  shm_addr[4] = countAdult; //+ [4]-Budou nejaci dospeli? 0 ne, Kazdy adult se odecte po konci prace

  // Vytvoreni generatoru dospelich
  pid_t adult_gen_pid = fork();
  if(adult_gen_pid == 0)
  {
    AdultGenerator(countAdult, genTimeAdult, workTimeAdult);
  }

  // Vytvoreni generatoru deti
  pid_t child_gen_pid = fork();
  if(child_gen_pid == 0)
  {
    ChildGenerator(countChild, genTimeChild, workTimeChild);
  }
  
  semaphore_wait(sem_AllEnd); // dokoncili vsichni praci?

  // nastavi se odpocet procesu aby posledni proces opet mohl rici ze je posledni
  semaphore_wait(sem_ProcesCounter);
    shm_addr[0] = countAdult + countChild;
  semaphore_post(sem_ProcesCounter);

  semaphore_post(sem_EndNow); // Zacnete umirat pro mne me deti.

  semaphore_wait(sem_AllEnd); // umreli jste vsichni?


  DeinitApp();
  // OK take umiram
  return 0;
}

/**
 * Prevod textu na cislo.
 * @param  str Vstupni text pro prevod.
 * @param  out Vystup do ktereho bude ulozeno prevedene cislo.
 * @return     Vraci 1 pokud se prevod povedl nebo 0 pokud se nepovedl.
 */
int StrToInt(char *str, int *out) 
{
  char *end;
  errno = 0;
  long l = strtol(str, &end, 10);

  if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
    return 0;
  if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
    return 0;
  if (*end != '\0')
    return 0;

  // Prevod se povedl :D Huraaa
  *out = l;
  return 1;
}

/**
 * Funkce zpracovava argumenty a nasledne vola funkci s hlavni casti programu.
 * @param  argc Pocet argumentu.
 * @param  argv Argumenty.
 * @return      Vraci 0 pokud byli zadany platne argumenty. 
 */
int ParseArgs(int argc, char **argv)
{
  // ./proj2 A C AGT CGT AWT CWT
  // AGT >= 0 && AGT < 5001.
  // AWT >= 0 && AWT < 5001.
  if(argc == 7)
  {
    int countAdult, countChild, genTimeAdult, genTimeChild, workTimeAdult, workTimeChild;
    // Podminky ktere musi vstup splnovat
    if(StrToInt(argv[1], &countAdult) && StrToInt(argv[2], &countChild) && 
      StrToInt(argv[3], &genTimeAdult) && StrToInt(argv[4], &genTimeChild) &&
      StrToInt(argv[5], &workTimeAdult) &&StrToInt(argv[6], &workTimeChild) &&
      countAdult > 0 && countChild > 0 &&
      genTimeAdult < 5001 && genTimeAdult >= 0 &&
      genTimeChild < 5001 && genTimeChild >= 0 &&
      workTimeAdult < 5001 && workTimeAdult >= 0 &&
      workTimeChild < 5001 && workTimeChild >= 0
       )
    {
      return First(countAdult, countChild, genTimeAdult, genTimeChild, workTimeAdult, workTimeChild);
    }
  }

  // Podminky pro overeni vstupu selhaly
  fprintf(stderr,"Zadany neplatne argumenty.\n");
  //Neni potreba deinicializovat, nebot inicializace jeste neprobehla.
  return 1;
}



int main(int argc, char **argv)
{
  return ParseArgs(argc, argv);;
}


#endif