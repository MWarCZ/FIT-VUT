/////////////////////////////
/// Soubor: ipk-mtrip.c   ///
/// Autor: Miroslav Valka ///
/////////////////////////////


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
//#include <fcntl.h>
#include <sys/time.h>
//#include <sys/select.h>
//#include <limits.h>
#include <float.h>
#include <signal.h>

#include "global.h"

#include "debug.h"


/**
 * Funkce zpracuje parametry a zkontroluje kombinace parametru a existenci hodnot parametru.
 * @param  argc Pocet argumentu prikazove radky.
 * @param  argv Argumenty prikazove radky.
 * @return      Vraci strukturu Param, ktera obsahuje nactene hodnoty parametru.
 */
Param GetParams(int argc, char** argv) {
  Param param;
  param.action = Action_None;
  param.host = NULL;
  param.port = 0;
  param.size = 0;
  param.time = 0;
  param.ipv6 = 0;

  Action action = Action_None;
  Option h = Option_None;
  Option p = Option_None;
  Option s = Option_None;
  Option t = Option_None;
  Option ipv6 = Option_None;
  char* hValue = NULL;
  int pValue = 0;
  int sValue = 0;
  int tValue = 0;

  int opt;

  if( (argc > 1) && (strcmp(argv[1], "reflect") == 0) ) {
    action = Action_Reflect;
  }
  else if( (argc > 1) && (strcmp(argv[1], "meter") == 0) ) {
    action = Action_Meter;
  }
  else if( (argc > 1) && (strcmp(argv[1], "help") == 0) ) {
    action = Action_Help;
  }
  else {
    DIE(Error_InputArgs, "Chybna akce. Ocekavano 'reflect', 'meter' nebo 'help'.\n");
  }
  
  while( (opt=getopt( argc,(char* const*) argv, "p:h:s:t:6" )) != -1 ) {
    char *tmp = NULL;

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
        tmp = NULL;
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

      // '-s velikost_sondy'
      case 's':
        // Duplicitne zadany parametr
        if( s ) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: s\n");
        }
        s |= Option_S;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota velikosti sondy.\n");
        }

        // Kontrola zda se jedna o cislo
        if( !CheckNumber(optarg) ) { /// err
          DIE(Error_InputArgs, "Zadana hodnota velikosti sondy neni cislo.\n");
        }
        tmp = NULL;
        sValue = (int)strtol(optarg, &tmp, 10 );
        
        break;

      // '-t doba_mereni'
      case 't':
        // Duplicitne zadany parametr
        if( t ) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: p\n");
        }
        t |= Option_T;

        // Byla zadana hodnota za prepinacem?
        if( !optarg ) { /// err
          DIE(Error_InputArgs, "Chybejici hodnota doby mereni.\n");
        }

        // Kontrola zda se jedna o cislo
        if( !CheckNumber(optarg) ) { /// err
          DIE(Error_InputArgs, "Zadana hodnota doby mereni neni cislo.\n");
        }
        tmp = NULL;
        tValue = (int)strtol(optarg, &tmp, 10 );
        
        break;

      case '6':
        // Duplicitne zadany parametr
        if( ipv6 ) { /// err
          DIE(Error_InputArgs, "Duplicitne zadany prepinac: 6\n");
        }
        ipv6 |= Option_6;
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
  
  if(action == Action_Reflect) {
    if( !p ) { /// err
      debug("p: %i\n", p);
      DIE(Error_InputArgs, "Chybejici parametr -p.\n");
    }
    if( h || s || t ) { /// err
      DIE(Error_InputArgs, "Zadany neplatne parametry pro akci 'reflect'.\n");
    }
    param.port = pValue;
  }
  else if(action == Action_Meter) {
    if( !p ) { /// err
      DIE(Error_InputArgs, "Chybejici parametr -p.\n");
    }
    if( !h ) { /// err
      DIE(Error_InputArgs, "Chybejici parametr -h.\n");
    }
    if( !s ) { /// err
      DIE(Error_InputArgs, "Chybejici parametr -s.\n");
    }
    if( !t ) { /// err
      DIE(Error_InputArgs, "Chybejici parametr -t.\n");
    }
    param.port = pValue;
    param.host = hValue;
    param.size = sValue;
    param.time = tValue;
  }
  else if(action == Action_Help) {
  }
  else {
    DIE(Error_InputArgs, "Chybna akce. Ocekavano 'reflect', 'meter' nebo 'help'.\n");
  }

  param.action = action;

  if( ipv6 & Option_6 ) {
    param.ipv6 = 1;
  }

  return param;
}

/////////////////////////////////////////////
//        UDP

/**
 * Funkce navaze spojeni se serverem.
 * @param serverHostname Jmeno/adresa serveru.
 * @param portNumber     Cislo portu.
 * @param clientSocket   Ukazatel na socket ktery bude otevren.
 * @param serverAddress  Ukazatel na strukturu, ktera bude slouzit ke komunikaci se serverem.
 */
void OpenClientUDP(const char *serverHostname, int portNumber, int* clientSocket, struct sockaddr_in *serverAddress) {
  struct hostent *server;
  //struct sockaddr_in serverAddress;

  if ((server = gethostbyname(serverHostname)) == NULL) {
    DIE(Error_ConnectToServer,"Host '%s' nebyl nalezen.\n", serverHostname);
  }

  bzero( (char *)serverAddress, sizeof(*serverAddress) );
  serverAddress->sin_family = AF_INET;
  bcopy( (char *)server->h_addr, (char *)&(serverAddress->sin_addr.s_addr), server->h_length );
  serverAddress->sin_port = htons(portNumber);

  if ((*clientSocket = socket(AF_INET, SOCK_DGRAM, 0)) <= 0) {
    DIE(Error_ConnectToServer,"Nepodarilo se vytvorit socket. ( socket )\n");
  }
}

/**
 * Funkce vytvori socket serveru na danem portu zahaji naslouchani na danem portu.
 * @param  portNumber    Cislo portu.
 * @param  welcomeSocket Ukazatel na socket na kterem bude otevrena komunikace.
 * @param  ipv6          Pri hodnote 0 bude vytvorena IPv4 komunikace, pokud bude hodnota nenulova bude vytvorena IPv6 komunikace.
 */
void OpenServerUDP(int portNumber, int* welcomeSocket, int ipv6) {
  int rc;
  struct sockaddr_in6 sa6;
  struct sockaddr_in sa4;
  struct sockaddr* psa;

  int pf_inet_4x6 = (ipv6)?PF_INET6:PF_INET;

  if ((*welcomeSocket = socket(pf_inet_4x6, SOCK_DGRAM, 0)) < 0) {
    DIE(Error_CreateServer, "Chyba pri vytvareni socketu.\n");
  }
  
  int on = 1;
  rc = setsockopt(*welcomeSocket, SOL_SOCKET,  SO_REUSEADDR, (const void *)&on, sizeof(int));
  if (rc < 0) {
    close(*welcomeSocket);
    DIE(Error_CreateServer, "Chyba pri nastavovani socketu.\n");
  }
  
  socklen_t saLen;
  if(ipv6) {
    saLen = sizeof(sa6);
    memset(&sa6,0,sizeof(sa6));
    sa6.sin6_family = AF_INET6;
    sa6.sin6_addr = in6addr_any;
    sa6.sin6_port = htons(portNumber); 
    psa = (struct sockaddr*)&sa6;
  } 
  else {
    saLen = sizeof(sa4);
    memset(&sa4,0,sizeof(sa4));
    sa4.sin_family = AF_INET;
    sa4.sin_addr.s_addr = htonl(INADDR_ANY);
    sa4.sin_port = htons(portNumber); 
    psa = (struct sockaddr*)&sa4;
  }

  if ((rc = bind(*welcomeSocket, psa, saLen)) < 0) {
    DIE(Error_CreateServer, "Chyba pri otvirani portu. ( bind )\n");
  }
    
  /*
  if ((rc = listen(*welcomeSocket, 1)) < 0) {
    DIE(Error_CreateServer, "Chyba pri naslouchani na portu. ( listen )\n");  
  }
  */
}
/////////////////////////////////////////////
//           TCP
/**
 * Funkce navaze spojeni se serverem.
 * @param serverHostname Jmeno/adresa serveru.
 * @param portNumber     Cislo portu.
 * @param clientSocket   Ukazatel na socket ktery bude otevren.
 */
void OpenClientTCP(const char *serverHostname, int portNumber, int* clientSocket) {
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
//
/**
 * Funkce vytvori socket serveru na danem portu zahaji naslouchani na danem portu.
 * @param  portNumber    Cislo portu.
 * @param  welcomeSocket Ukazatel na socket na kterem bude otevrena komunikace.
 * @param  ipv6          Pri hodnote 0 bude vytvorena IPv4 komunikace, pokud bude hodnota nenulova bude vytvorena IPv6 komunikace.
 */
void OpenServerTCP(int portNumber, int* welcomeSocket, int ipv6) {
  int rc;
  struct sockaddr_in6 sa6;
  struct sockaddr_in sa4;
  struct sockaddr* psa;

  int pf_inet_4x6 = (ipv6)?PF_INET6:PF_INET;

  if ((*welcomeSocket = socket(pf_inet_4x6, SOCK_STREAM, 0)) < 0) {
    DIE(Error_CreateServer, "Chyba pri vytvareni socketu.\n");
  }
  
  int on = 1;
  rc = setsockopt(*welcomeSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0) {
    close(*welcomeSocket);
    DIE(Error_CreateServer, "Chyba pri nastavovani socketu.\n");
  }
  
  socklen_t saLen;
  if(ipv6) {
    saLen = sizeof(sa6);
    memset(&sa6,0,sizeof(sa6));
    sa6.sin6_family = AF_INET6;
    sa6.sin6_addr = in6addr_any;
    sa6.sin6_port = htons(portNumber); 
    psa = (struct sockaddr*)&sa6;
  } 
  else {
    saLen = sizeof(sa4);
    memset(&sa4,0,sizeof(sa4));
    sa4.sin_family = AF_INET;
    sa4.sin_addr.s_addr = htonl(INADDR_ANY);
    sa4.sin_port = htons(portNumber); 
    psa = (struct sockaddr*)&sa4;
  }

  if ((rc = bind(*welcomeSocket, psa, saLen)) < 0) {
    DIE(Error_CreateServer, "Chyba pri otvirani portu. ( bind )\n");
  }
    
  if ((rc = listen(*welcomeSocket, 1)) < 0) {
    DIE(Error_CreateServer, "Chyba pri naslouchani na portu. ( listen )\n");  
  }
}

/**
 * Uchovava prumerny rtt paketu.
 */
static double rtt = -1;
/**
 * Uchovava prumernou sirku pasma.
 */
static double bandwidth = -1;
/**
 * Uchovava maximalni sirku pasma.
 */
static double bandwidthMax = -1;
/**
 * Uchovava minimalni sirku pasma.
 */
static double bandwidthMin = DBL_MAX;
/**
 * Uchovava pocet obdrzenych bajtu
 */
static int allBytes = 0;
/**
 * Uchovava pocet mereni.
 */
static unsigned long int countMeasure = 0;
/**
 * Uchovava priblizny rozptyl pro pozdejsi vypocet odchylky.
 */
static double deviation = 0;

//
/**
 * Funkce odecte od sebe dva casy ve strukture timeval a vrati vysledek v sekundach.
 * @param  res Ukazatel na strukturu timeval do ktere se ulozi vysledek.
 * @param  x   Cas od ktereho se bude odecitat.
 * @param  y   Cas ktery se odecita.
 * @return     Vraci se rozdil casu v sekundach.
 */
double SubTimeval(struct timeval *res, struct timeval *x, struct timeval *y) {
  res->tv_sec = x->tv_sec - y->tv_sec;
  res->tv_usec = x->tv_usec - y->tv_usec;

  // 22 - 3 => 20 + (2 - 3) =>
  // 20 + (-1 < 0) => (20 - 10) + ( -1 + 10) =>
  // (10) + (9) => 19 
  if (res->tv_usec < 0) {
    res->tv_sec--; 
    res->tv_usec += 1000000;
  }

  double diff = res->tv_sec;
  diff += res->tv_usec/1000000.0;

  return diff;
}

// RTT:
// https://www.quora.com/How-does-TCP-round-trip-time-RTT-estimation-work-How-different-is-the-implementation-across-operating-systems

/**
 * Funkce pro pocitani prumerneho RTT. Pocita tzv. harmonicky prumer.
 * @param  start Cas kdy se zacalo merit.
 * @param  end   Cas kdy se prestalo merit.
 * @return       Vraci aktualni hodnotu prumerneho RTT.
 */
double RTT(struct timeval *start, struct timeval *end) {

  struct timeval difftv;
  double tmprtt = SubTimeval(&difftv, end, start );

  if(tmprtt < 0) { 
    // Zaporne rozdily nezapocitavat - asi doslo k chybe
    return rtt;
  }

  if(rtt < 0) { // prvni hodnota mereni
    rtt = tmprtt;
    return rtt;
  }
  // EstimatedRTT = 0.875 * EstimatedRTT + 0.125 * SampleRTT
  rtt = 0.875 * rtt + 0.125 * tmprtt;
  return rtt;
}
//

/**
 * Funkce pro pocitani sirky pasma. Jako vedlejsi efekt uklada i maximalni a minimalni sirku pasma.
 * @param  start Cas zacatku mereni.
 * @param  end   Cas konce mereni.
 * @param  bytes Pocet bajtu prijatych merenim.
 * @return       Aktualni prumerna sirka pasma.
 */
double MeasureBandwidth(struct timeval *start, struct timeval *end, int bytes) {

  struct timeval difftv;
  double diff = SubTimeval(&difftv, end, start);

  if(diff < 0) { 
    // Zaporne rozdily nezapocitavat - asi doslo k chybe
    return bandwidth;
  }

  allBytes += bytes;
  double tmpbw = (bytes) / diff; // ??

  //double tmpbw = (allBytes) / diff; // B/s
  //double tmpbw = (allBytes / 1024) / diff; // kB/s
  //double tmpbw = (allBytes / (1024 * 1024)) / diff; // MB/s

  // min a max
  if(tmpbw < bandwidthMin) {
    bandwidthMin = tmpbw;
  }
  if(tmpbw > bandwidthMax) {
    bandwidthMax = tmpbw;
  }

  if(bandwidth < 0) { // prvni hodnota mereni
    bandwidth = tmpbw;
  }
  else {
    // avg - harmonicky prumer
    
    // (Pocet mereni +1)/(pocet mereni / dosavadni rychlost) + (pocet mereni=1)/(aktualni rychlost)
    bandwidth = (countMeasure + 1) / ( (countMeasure/bandwidth) + (1/tmpbw) );

  }

  // dev
  deviation = deviation + (tmpbw - bandwidth)*(tmpbw - bandwidth);

  countMeasure++;

  return bandwidth;
}
//
/**
 * Funkce pro tisknuti vysledku mereni.
 */
void PrintMeasurea() {

  printf("Pocet uspesne merenych paketu '%li'\n", countMeasure );
  if(countMeasure > 0) {
    printf("Bandwidth Max: %20.6f B/s\n",bandwidthMax);
    printf("Bandwidth AVG: %20.6f B/s\n",bandwidth);
    printf("Bandwidth MIN: %20.6f B/S\n",bandwidthMin);
    printf("RTT AVG: %f s\n",rtt);
  }
  else {
    printf("Neni dostatek dat pro vypocty.\n" );
  }
}

//
//

/**
 * Hlavni cast klientske aplikace.Provadi posilani dat na server a mereni.
 * @param socket        Ukazatel na otevreny socket.
 * @param ipv6          Informace jestli se pouziva IPv6 nebo IPv4 komunikace.
 * @param serverAddress Ukazatel na nastavenou strukturu, ktera slouzi ke komunikaci se serverem.
 * @param size          Velikost jednoho testovaciho paketu.
 * @param time          Cas v sekundach, jak dlouho ma trvat mereni.
 */
void MainClient3(int* socket, int ipv6, struct sockaddr_in *serverAddress, int size, int time) {
  struct sockaddr* psaClient;
  socklen_t saClientLen;

  // Vyber pripojeni ipv4 vs ipv6
  psaClient = (struct sockaddr*)serverAddress;
  if(ipv6) {
    saClientLen = sizeof(struct sockaddr_in6);
  }
  else {
    saClientLen = sizeof(struct sockaddr_in);
  }

  // deklarace potrebnych promenych pro mereni:
  struct timeval stop, start, timeout; /// cas
  struct timeval tmptv, measureUntil, measureNow;
  double rttl = 0.0, bandwidthl = 0.0;
  long int lastPrint;

  int res;
  char buffSend[size+1];
  char buffRecv[size+1];
  // naplneni nejakymi daty
  for(int i=0;i<size;i++) {
    buffSend[i]='a';
  }
  buffRecv[size] = '\0';

  gettimeofday(&measureNow, NULL);

  measureUntil.tv_sec = measureNow.tv_sec + time; 
  measureUntil.tv_usec = measureNow.tv_usec;

  lastPrint = measureNow.tv_sec;

  debug("M_Start:\n");

  // untilTime > nowTime; untilTime - nowTime > 0
  while( SubTimeval(&tmptv, &measureUntil, &measureNow) > 0 ) {
    
    debug("P_Start:\n");

    timeout.tv_sec = 1; 
    timeout.tv_usec = 0;

    gettimeofday(&start, NULL); /// cas mereni start

    res = SendtoMsg(socket, buffSend, size, (struct sockaddr *)psaClient, &saClientLen);

    if ( Select(socket, &timeout) ) {
      res = RecvfromMsg(socket, buffRecv, size, (struct sockaddr *)psaClient, &saClientLen);

      gettimeofday(&stop, NULL); /// cas mereni end

      buffRecv[res] = '\0';

      // vypocty
      rttl = RTT(&start,&stop);
      bandwidthl = MeasureBandwidth(&start,&stop,size);

      debug("rec: %i\n",res);
      debug("rtt: %f\n",rttl);
      debug("bw_max: %f\n",bandwidthMax/1024/1024);
      debug("bw_avg: %f\n",bandwidth/1024/1024);
      debug("bw_min: %f\n",bandwidthMin/1024/1024);
    }
    else {
      //DIE(Error_Timeout, "Vyprsel cas pri cekani na odezvu. ( timeout )\n");
      debug("Vyprsel cas pri cekani na odezvu. ( timeout )\n");
    }
    // Prubezne vypisovani hodnot
    if(lastPrint != measureNow.tv_sec) {
      printf("----------------------------------------\n" );
      PrintMeasurea();
      lastPrint = measureNow.tv_sec;
    }

    // aktualizace aktualniho casu
    gettimeofday(&measureNow, NULL);

    debug("P_End:\n");
  }

  // tisknuti finalniho vysledku mereni
  printf("========================================\n" );
  printf("|           FINALNI VYSLEDEK           |\n" );
  printf("========================================\n" );
  PrintMeasurea();

  debug("M_End:\n");
}
//

/**
 * Hlavni cast serverove aplikace. Provadi reflektivni odesilani dat zpet klientovy.
 * @param socket Ukazatel na otevreny socket.
 * @param ipv6   Informace jestli se pouziva IPv6 nebo IPv4 komunikace.
 * @param size   Maximalni velikost prijmanych dat pro jeden packet.
 */
void MainServer3(int * socket, int ipv6, int size) {
  struct sockaddr_in6 saClient6;
  struct sockaddr_in saClient4;
  char str[INET6_ADDRSTRLEN];
  struct sockaddr* psaClient;
  socklen_t saClientLen;

  if(ipv6) {
    psaClient = (struct sockaddr*)&saClient6;
    saClientLen = sizeof(saClient6);
  }
  else {
    psaClient = (struct sockaddr*)&saClient4;
    saClientLen = sizeof(saClient4);
  }

  int res;
  char buff[size+1];

  while(1) {

    res = RecvfromMsg(socket, buff, size, (struct sockaddr *)psaClient, &saClientLen);

    debug("----------------------------\n");
    buff[res] = '\0';
    debug("res: %i\n",res);
    debug("recv: %s\n",buff);

    res = SendtoMsg(socket, buff, size, (struct sockaddr *)psaClient, &saClientLen);

  } // while(1)
}
//

/**
 * Funkce pro vypsani napovedy k programu.
 */
void PrintHelp() {
  printf("\n========================================\n" );
  printf("|               NAPOVEDA               |\n" );
  printf("========================================\n" );
  printf("Pouziti: ipk-mtrip Role [Prepinace]\n");
  printf("\nRole: \n");
  printf("  help\t\t Vypise tuto napovedu.\n");
  printf("  reflect\t Aktivuje rezim serveru, ktery jen odesila prijata data.\n");
  printf("  meter\t\t Odesila data na server a meri hodnoty Bandwidth a RTT.\n");
  printf("\nPrepinace: \n");
  printf("  -h Host\t Adresa serveru na ktery budou posilany testovaci data.\n");
  printf("  -p Port\t Port na kterem nasloucha server nebo port\n  \t\t na kterem ma server naslouchat.\n");
  printf("  -t Time\t Doba po kterou se bude merit. Udavana v sekundach.\n");
  printf("  -s Size\t Velikost testovacich paketu. \n  \t\t Velikost paketu muze nabyvat hodnot 1 - 65500.\n");
}

//
/**
 * Funkce pro zpracovavani signalu preruseni.
 * Pro SIGINT Vypise namerena data a ukonci aplikaci.
 * @param sign Kod preruseni.
 */
void SignalHandler(int sign) {
  if(sign == SIGINT) {
    printf("\n========================================\n" );
    printf("|          FINALNI VYSLEDEK            |\n" );
    printf("========================================\n" );
    PrintMeasurea();
    exit(0);
  }
}

////////////////////////////////////////////
//     MAIN

/**
 * Hlavni telo programu. Koriguje nacteni parametru a vyber prislusneho podprogramu klient/server/napoveda a zajisti podprogramum sitove prostredky.
 * @param  argc Pocet vstupnich parametru.
 * @param  argv Pole vstupnich parametru.
 */
int main(int argc, char** argv) {

  Param param = GetParams(argc, argv);

  int socket;

  // 
  if( param.action == Action_Reflect) {
    INFO("Zapinani serveru...");

    debug("\nport: %i %s\n",param.port,(param.ipv6)?"ipv6":"ipv4");
    // Otevreni pripojeni
    OpenServerUDP(param.port, &socket, param.ipv6);

    INFO("\rServer spusten na portu: %d\n", param.port);

    //MainServer(&socket, param.ipv6);
    MainServer3(&socket, param.ipv6, 65500);
  }
  else if( param.action == Action_Meter) {
    if(param.size < 1 || param.size > 65500) {
      DIE(Error_InputArgs,"Parametr -s muze nabyvat jen hodnot 1 - 65500.");
    }

    struct sockaddr_in serverAddress;

    printf("Pripojovani...");
    // Otevreni pripojeni
    OpenClientUDP( param.host, param.port, &socket, &serverAddress);
    printf("\rPripojen.     \n");

    signal(SIGINT, SignalHandler);

    //MainClient(&socket, param.ipv6, &serverAddress);
    MainClient3(&socket, param.ipv6, &serverAddress, param.size, param.time);
    debug("Data odeslana.\n");
  }
  else if( param.action == Action_Help) {
    PrintHelp();
  }

      
  close(socket);

  return 0;
}

/// KonecSouboru: ipk-mtrip.c
