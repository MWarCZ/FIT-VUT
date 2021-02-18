/////////////////////////////
/// Soubor: ipk-clietn.c  ///
/// Autor: Miroslav Valka ///
/////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>


#include "protokol.h"
#include "global.h"

#include "debug.h"

// ./ipk-client -h host -p port [-n|-f|-l] login

/**
 * Funkce zpracuje parametry a zkontroluje kombinace parametru a existenci hodnot parametru.
 * @param  argc Pocet argumentu prikazove radky.
 * @param  argv Argumenty prikazove radky.
 * @return      Vraci strukturu Param, ktera obsahuje nactene hodnoty parametru.
 */
Param ClientApp(int argc, char** argv) {
  Param param;
  param.action = Action_None;
  param.login = NULL;
  param.host = NULL;
  param.port = 0;

  Option p = Option_None, h = Option_None, nfl = Option_None;
  int pValue = 0;
  char* hValue = NULL;
  char* login = "";

  int opt;
  
  while( (opt=getopt( argc,(char* const*) argv, "p:h:n:f:l" )) != -1 ) {

    switch(opt) {

      // '-p port'
      case 'p':
        // Duplicitne zadany parametr
        if( p ) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: p\n");
        }
        p |= Option_P;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota portu.\n");
        }

        // Kontrola zda se jedna o cislo
        if( !CheckNumber(optarg) ) { /// err
          DIE(Error_InputArgs, "Zadana hodnota portu neni cislo.\n");
        }
        char *tmp = NULL;
        pValue = (int)strtol(optarg, &tmp, 10 );
        
        break;

      // '-h hostname/ipv4/ipv6'
      case 'h':
        // Duplicitne zadany parametr
        if( h ) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: h\n");
        }
        h |= Option_H;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota hostname, IPv4 nebo IPv6.\n");
        }
        hValue = optarg;

        break;

      // '-n login'
      case 'n':
        // Jiz byl zadan jiny prepinac '-n','-f','-l'
        if(nfl) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: n, f, l\n");
        }
        nfl |= Option_N;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota login.\n");
        }
        login = optarg;

        break;

      // '-f login'
      case 'f':
        // Jiz byl zadan jiny prepinac '-n','-f','-l'
        if(nfl) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: n, f, l\n");
        }
        nfl |= Option_F;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota login.\n");
        }
        login = optarg;

        break;

      // '-l prefix_login'
      case 'l':
        // Jiz byl zadan jiny prepinac '-n','-f','-l'
        if(nfl) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: n, f, l\n");
        }
        nfl |= Option_L;

        // Prefix loginu jako volitelna hodnota
        if( !optarg && NULL != argv[optind] && '-' != argv[optind][0] ) {
          login = argv[optind++];
        }

        break;

      case ':': // Kdyz neco chybi - hodnota
        if( isprint(optopt) ) { /// err
          DIE(Error_InputArgs, "Nebila zadana hodnota pro: %c\n", optopt);
        }
        DIE(Error_InputArgs, "Nebila zadana hodnota pro parametr.\n");
        break;

      case '?': // Kdyz neco chybi - parametr
        if( isprint(optopt) ) { /// err
          DIE(Error_InputArgs, "Zadan neznami parametr: %c\n", optopt);
        }
        DIE(Error_InputArgs, "Zadan neznami parametr.\n");
        break;

      default: /// err
        DIE(Error_InputArgs, "Neznama chyba pri zpracovani parametru: %s\n", optarg);
        break;
    }
  } // while

  // Kontrloly kombinaci parametru
  if( !(p && h && nfl) ) { /// err
    debug(">p: %i, h: %i, nfl: %i\n", p, h, nfl);
    DIE(Error_InputArgs, "Byla zadana neplatna kombinace parametru.\n");
  }

  param.login = login;
  param.host = hValue;
  param.port = pValue;

  if( nfl & Option_N ) {
    param.action = Action_GetUserInfo;
  }
  else if( nfl & Option_F ) {
    param.action = Action_GetUserHomeDir;
  }
  else if( nfl & Option_L ) {
    param.action = Action_GetUsersList;
  }

  return param;
}


/**
 * Funkce navaze spojeni se serverem.
 * @param serverHostname Jmeno/adresa serveru.
 * @param portNumber     Cislo portu.
 * @param clientSocket   Ukazatel na socket ktery bude otevren.
 */
void OpenClient(const char *serverHostname, int portNumber, int* clientSocket) {
  struct hostent *server;
  struct sockaddr_in serverAddress;

  if ((server = gethostbyname(serverHostname)) == NULL) {
    DIE(Error_ConnectToServer,"Host '%s' nebyl nalezen.\n", serverHostname);
  }

  bzero( (char *)&serverAddress, sizeof(serverAddress) );
  serverAddress.sin_family = AF_INET;
  bcopy( (char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length );
  serverAddress.sin_port = htons(portNumber);

  if ((*clientSocket = socket(AF_INET, SOCK_STREAM, 0)) <= 0) {
    DIE(Error_ConnectToServer,"Nepodarilo se vytvorit socket. ( socket )\n");
  }

  if (connect(*clientSocket, (const struct sockaddr *)&serverAddress, sizeof(serverAddress)) != 0) { 
    DIE(Error_ConnectToServer,"Nepodarilo se navazat spojeni se serverem. ( connect )\n");     
  }
}

/**
 * Hlavni funkce klientske aplikace. Komunikuje se serverem a zpracovava data od serveru.
 * @param clientSocket Ukazatel na otevreny socket.
 * @param action       Akce kterou bude klient vykonavat.
 * @param login        Data, ktera budou posilana serveru. (login uzivatele / prefix loginu)
 */
void MainClient(int* clientSocket, Action action, const char* login) {
  int bytestx, bytesrx;
  int request, reply;
  char buff[BUFSIZE];

  //mwarcz:01:%c:%s
  //0-6:7-9:10-11:12-res

  // Nastaveni odesilane zpravy zadosti.
  switch(action) {
    case Action_GetUserInfo: 
      snprintf( buff, sizeof(buff), "%s%s", PROT_REQ_GET_USER_INFO,login );
      break;
    case Action_GetUserHomeDir:
      snprintf( buff, sizeof(buff), "%s%s", PROT_REQ_GET_USER_HOME_DIR,login );
      break;
    case Action_GetUsersList:
      snprintf( buff, sizeof(buff), "%s%s", PROT_REQ_GET_USERS_LIST,login );
      break;
    default:
      DIE(Error_Unknown,"Chyba. Doslo k nezname akci.\n");
      break;
  }

  debug(">%s\n",buff);

  // Odeslani zadosti serveru
  request = SendMsg(clientSocket, buff);

  unsigned int recv_counter = 1;
  while(recv_counter) {
    reply = RecvMsg(clientSocket, buff);
    buff[reply] = '\0';

    // Server ukoncil komunikaci uspesne
    if( strcmp(&buff[0], PROT_SUCC_RES) == 0 ) {
      debug(">OK odpoved.\n");
      break;
    }
    // Server ukoncil komunikaci neuspesne
    else if( strcmp(&buff[0], PROT_FAIL_RES) == 0 ) {
      debug(">KO odpoved.\n");
      INFO("Server odmitl poslat data.\n");
      break;
    }

    // Vykonani akce
    switch(action) {
      case Action_GetUserInfo:
        if(recv_counter == 1) {
          //printf("Login: %s/n", buff);
        }
        else if(recv_counter == 2) {
          //printf("UID: %s,", buff);
        }
        else if(recv_counter == 3) {
          printf("%s\n", buff);
        }
        else {
          //printf("%s\n", buff);
        }
        break;
      case Action_GetUserHomeDir: 
        printf("%s\n", buff);
        break;
      case Action_GetUsersList: 
        printf("%s\n", buff);
        break;
      default:
        DIE(Error_Unknown,"Chyba. Doslo k nezname akci.\n");
        break;
    }

    // Odeslani potvrzeni serveru, ze data dorazila a muze posilat dalsi.
    SendOK(clientSocket);

    recv_counter++;
  } // while(recv_counter)
}


// main
int main(int argc, char** argv) {

  Param param = ClientApp(argc, argv);

  int clientSocket;

  // Otevreni pripojeni
  OpenClient( param.host, param.port, &clientSocket);

  // Zpracovavani dat a komunikace
  MainClient(&clientSocket, param.action, param.login);
      
  close(clientSocket);

  return 0;
}

/// KonecSouboru: ipk-client.c 
