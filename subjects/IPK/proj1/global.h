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
  Option_N = 4,
  Option_F = 8,
  Option_L = 16,
  Option_6 = 32
} Option;

/**
 * Velikost bufferu pro data, pres ktery spolu aplikace komunikuji.
 */
#define BUFSIZE 1024


/* Definice akci */

/**
 * Vyctovy typ jednotlivych akci, ktere mohou aplikace vykonavat.
 */
typedef enum {
  Action_None = 0,
  Action_Help,
  Action_GetUserInfo,
  Action_GetUserHomeDir,
  Action_GetUsersList
} Action;

/**
 * Struktura obsahujici nactena data z parametru aplikace.
 */
typedef struct {
  Action action;
  char *login;
  char *host;
  int port;
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
  Error_Send = 5,
  Error_Recv = 6,

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
#define INFO(msg,...) fprintf(stderr, msg, ##__VA_ARGS__);


/* Funkce */

/**
 * Zkontroluje jestli je textovy string cislo ci nikoli.
 * @param  str Textovy retezec, ktery muze byt cislem.
 * @return     Vraci 1 pokud str je cislo, jinak 0.
 */
int CheckNumber(char* str);

/**
 * Funkce odesle zpravu zkrze socket, pokud dojde k chybe vypise chybovou hlasku a ukonci aplikaci s chybovym kodem.
 * @param  socket Otevreny socket pres ktery bode poslana zprava.
 * @param  buff   Data ktera budou odeslana.
 * @return        Vraci pocet odeslanych znaku.
 */
int SendMsg(int* socket, char* buff);

/**
 * Funkce ceka na prijem zpravy zkrze socket, pokud dojde k chybe vypise chybovou hlasku a ukonci aplikaci s chybovym kodem.
 * @param  socket Otevreny socket pres ktery je poslouchana zprava.
 * @param  buff   Pole do ktereho budou ukladana prenasena data.
 * @return        Vraci pocet prijatych znaku.
 */
int RecvMsg(int* socket, char* buff);

/**
 * Funkce odesila protokol s kladnou odpovedi.
 * @param  socket Otevreny socket pres ktery je zprava posilana.
 * @return        Vraci pocet odeslanych znaku.
 */
int SendOK(int* socket);

/**
 * Funkce odesila protokol s negativni odpovedi.
 * @param  socket Otevreny socket pres ktery je zprava posilana.
 * @return        Vraci pocet odeslanych znaku.
 */
int SendKO(int* socket);

/**
 * Funkce odesle zpravu a nasledne ceka na odpoved zda data prisla.
 * @param  socket     Otevreny socket pres ktery je komunikovano.
 * @param  inputBuff  Data ktera budou posilana.
 * @param  outputBuff Pole do kterehou budou ukladana data.
 * @return            Vraci pocet prijatych znaku.
 */
int SendAndWait(int *socket, char* inputBuff, char* outputBuff);


#endif

/// KonecSouboru: global.h
