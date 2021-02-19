/////////////////////////////
/// Soubor: global.h      ///
/// Autor: Miroslav Valka ///
/////////////////////////////

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/**
 * Mnozina ruznych nastaveni, kterych muze aplikace nabyvat.
 */
typedef enum {
  Option_None = 0,
  Option_P = 1,
  Option_H = 2,
  Option_S = 4,
  Option_T = 8,
  Option_6 = 16
} Option;

/**
 * Velikost bufferu pro data, pres ktery spolu aplikace komunikuji.
 */
#define BUFSIZE 1024

typedef enum {
  Action_None = 0,
  Action_Reflect = 1,
  Action_Meter = 2,
  Action_Help = 3
} Action;

/**
 * Struktura obsahujici nactena data z parametru aplikace.
 */
typedef struct {
  Action action;
  char *host;
  int port;
  int size;
  int time;
  int ipv6;
} Param;


/* Definice chyb */

/**
 * Vyctovy typ jednotlivych chyb, ktere mohou v aplikacich nastat.
 */
typedef enum {
  Error_None = 0,
  Error_InputArgs = 1,
  Error_CreateServer = 2,
  Error_RunTimeServer = 3,
  Error_ConnectToServer = 4,
  Error_Sendto = 5,
  Error_Recvfrom = 6,
  Error_Select = 7,
  Error_Timeout = 8,

  Error_Unknown = 99
} Error;

/**
 * Makro ktere vypise formatovanou chybovou hlasku a nasledne ukonci aplikaci s urcitym navratovym kodem.
 * @param  exitCode     Navratovy kod aplikace.
 * @param  msg          Formatovaci textovy retezec.
 * @param  __VA_ARGS__  Dalsi parametry, ktere budou vkladany do formatovaciho textoveho retezce.
 */
#define DIE(exitCode,msg,...) fprintf(stderr, msg, ##__VA_ARGS__); exit(exitCode);

/**
 * Makro vypise formatovany text na stderr.
 * @param  msg         Formatovaci textovy retezec.
 * @param  __VA_ARGS__ Dalsi parametry, ktere budou vkladany do formatovaciho textoveho retezce.
 */
#define INFO(msg,...) fprintf(stderr, msg, ##__VA_ARGS__); fflush(stderr);


/* Funkce */

int SendtoMsg(int* socket, char* buff, int bufsize, struct sockaddr *psaClient, socklen_t *saClientLen);
int RecvfromMsg(int* socket, char* buff, int bufsize, struct sockaddr *psaClient, socklen_t *saClientLen);
int Select(int* socket, struct timeval* waitTime);

/**
 * Zkontroluje jestli je textovy string cislo ci nikoli.
 * @param  str Textovy retezec, ktery muze byt cislem.
 * @return     Vraci 1 pokud str je cislo, jinak 0.
 */
int CheckNumber(char* str);


#endif

/// KonecSouboru: global.h
