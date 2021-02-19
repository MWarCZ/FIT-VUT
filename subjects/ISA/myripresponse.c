//////////////////////////////////////////
//                                     //
// Projekt: ISA RIP                   //
// Soubor: myripresponse.c           //
// Autor: Miroslav Valka (xvalka05) //
//                                 //
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>

#include <pcap.h>

#include "riplib.h"

// https://www.devdungeon.com/content/using-libpcap-c

//##################################################

// /myripresponse 
// -i <rozhraní> 
// -r <IPv6>/[16-128] 
// {-n <IPv6>} 
// {-m [0-16]} 
// {-t [0-65535]}

///////////////////
// Definice typu //
///////////////////

typedef enum {
  TOption_None = 0,
  TOption_Help = 1,
  TOption_I = 2,
  TOption_R = 4,
  TOption_N = 8,
  TOption_M = 16,
  TOption_T = 32
} TOption;

typedef struct {
  TOption options; 
  char *interface;
  char *ipRoute;
  u_int8_t prefixLen;
  char *nextHop;
  u_int8_t metric;
  u_int16_t routeTag;
} TParam;
 

///////////////////////////
// Funkce pro zpracovani //
///////////////////////////

/**
 * Funkce zpracuje argumenty a nastavi parametry, 
 * ktere aplikace prijima.
 * Pokud dojde k chybe ve zpracovani je ukoncena cela aplikace (exit(1)).
 * @param argc  Pocet argumentu v argv.
 * @param argv  Argumenty predane aplikaci pri spusteni.
 * @return Vraci strukturu obsahujici nastaveni parametru pro beh aplikace.
 */
TParam getParam(int argc, char** argv) {
  TParam param;
  param.options = TOption_None;
  param.interface = NULL;
  param.ipRoute = NULL;
  param.prefixLen = 0;
  param.nextHop = NULL;
  param.metric = 1;
  param.routeTag = 0;

  int opt;
  char *ptr; // Pomocny ukoztel
  long int tmpLong; // Pomocne cislo (prevody str2num)
  struct in6_addr tmpIn6Addr;

  while( (opt=getopt(argc, (char* const*)argv, "i:r:n:m:t:h")) != -1 ) {
    switch(opt) {
      case 'i': 
        if(param.options & TOption_I) {
          fprintf(stderr," -i: Parametr byl zadan duplicitne.\n");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr," -i: Je ocekavana hodnota. (eth0,wlan0,...)\n");
          exit(1);
        }
        param.interface = optarg;
        param.options |= TOption_I;
        break;

      case 'r':
        if(param.options & TOption_R) {
          fprintf(stderr," -r: Parametr byl zadan duplicitne.\n");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr, " -r: Je ocekavana IPv6 adresa a delka prefixu.. (napr.: fe01::/64)\n");
          exit(1);
        }

        ptr = strchr(optarg, '/');
        if(!ptr) {
          // ERR Znak '/' nenalezen
          fprintf(stderr," -r: Nebyl nalezena delka prefixu.\n");
          exit(1);
        }
        if(strlen(ptr)<3) {
          // ERR kratky prefix /15 je min(3znaky)
          fprintf(stderr, " -r: Nebyl nalezen platny prefix.\n");
          exit(1);
        }
        ptr[0] = 0; // Rozdeleni na dva retezce.
        ptr++;

        tmpLong = strtol(ptr, &ptr, 10);
        if(tmpLong < 16 || tmpLong > 128) {
          // ERR mimo rozsah
          fprintf(stderr, " -r: Prefix '%ld' je mimo rozsah 16-128.\n", tmpLong);
          exit(1);
        }
        if(inet_pton(AF_INET6, optarg, &tmpIn6Addr) == 0){
          fprintf(stderr," -r: Neplatna IPv6 adresa.\n");
          exit(1);
        }

        param.prefixLen = tmpLong;
        param.ipRoute = optarg;
        param.options |= TOption_R;
        break;

      case 'n': 
        if(param.options & TOption_N) {
          fprintf(stderr," -n: Parametr byl zadan duplicitne.\n");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr," -n: Je ocekavana hodnota pro Next Hop. \n");
          exit(1);
        }
        if(inet_pton(AF_INET6, optarg, &tmpIn6Addr) == 0){
          fprintf(stderr," -n: Neplatna IPv6 adresa.\n");
          exit(1);
        }
        param.nextHop = optarg;
        param.options |= TOption_N;
        break;

      case 'm': 
        if(param.options & TOption_M) {
          fprintf(stderr," -m: Parametr byl zadan duplicitne.\n");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr," -m: Je ocekavana hodnota.\n");
          exit(1);
        }
        
        tmpLong = strtol(optarg, &ptr, 10);
        if(tmpLong < 0 || tmpLong > 16) {
          // ERR mimo rozsah
          fprintf(stderr, " -m: Hodnota '%ld' je mimo rozsah 0-16.\n", tmpLong);
          exit(1);
        }
        param.metric = tmpLong;
        param.options |= TOption_M;
        break;

      case 't': 
        if(param.options & TOption_T) {
          fprintf(stderr," -t: Parametr byl zadan duplicitne.\n");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr," -t: Je ocekavana hodnota.\n");
          exit(1);
        }
        
        tmpLong = strtol(optarg, &ptr, 10);
        if(tmpLong < 0 || tmpLong > 65535) {
          // ERR mimo rozsah
          fprintf(stderr, " -t: Hodnota '%ld' je mimo rozsah 0-65535.\n", tmpLong);
          exit(1);
        }
        param.routeTag = tmpLong;
        param.options |= TOption_T;
        break;

      default:
        param.options = TOption_Help;

    }
  } // while

  return param;
}


//--------------------------------

int checkDeviceExist(char *device) {
  // Zjisteni existence rozhrani/zarizeni (eth0, wlan0, ...)
  
  char errbuf[PCAP_ERRBUF_SIZE];
  bpf_u_int32 ip;
  bpf_u_int32 subnetMask;

  if(pcap_lookupnet(device, &ip, &subnetMask, errbuf) < 0) {
    return 2; // Unexist
  }
  return 0; // Exist
}

int sendRipPacket(char *device, char *buffer, int packetLen) {

  // Zjisteni existence rozhrani/zarizeni (eth0, wlan0, ...)
  
  char errbuf[PCAP_ERRBUF_SIZE];
  bpf_u_int32 ip;
  bpf_u_int32 subnetMask;

  if(pcap_lookupnet(device, &ip, &subnetMask, errbuf) < 0) {
    fprintf(stderr,"Chybejici informace o zarizeni: %s\n",device);
    return 2;
  }


  // OPEN Otevreni udp pro ipv6
  int sock;
  if( (sock = socket(AF_INET6, SOCK_DGRAM, 0)) <= 0 ) {
    // ERR    
    fprintf(stderr,"Nepodarilo se otevrit udp socket.\n");
    return 3;
  }

  // ADDRESS Nastaveni odesilatele
  struct sockaddr_in6 meAddress;
  bzero( (char *)&meAddress, sizeof(meAddress) );
  meAddress.sin6_family = AF_INET6;
  meAddress.sin6_port = htons(521);
  meAddress.sin6_addr = in6addr_any;

  if( (bind(sock, (struct sockaddr*)&meAddress, sizeof(meAddress)) ) < 0) {
    // ERR
    fprintf(stderr, "bind error\n");
    return 4;
  }

  // Nasteveni poctu hopu pro udp paket
  int hops = 255;
  if (setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &hops, sizeof(hops))) {
    // ERR
    return 5;
  }

  // Nastaveni rozhrani pro odesilani.
  if( setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, device, strlen(device)) ) {
    // ERR
    return 6;
  }

  // ADDRESS nastaveni prijemce
  struct sockaddr_in6 address;
  bzero( (char *)&address, sizeof(address) );
  address.sin6_family = AF_INET6;
  address.sin6_port = htons(521);
  inet_pton(AF_INET6, "ff02::9", &address.sin6_addr);


  // SENDTO Odeslani paketu
  sendto(sock, buffer, packetLen, 0, (struct sockaddr*)&address, sizeof address);

  return 0;
}


void broadcast(char *device, char *ipRoute, u_int8_t prefixLen, char *nextHop, u_int8_t metric, u_int16_t routeTag ){
  
  // RIP Packet
  char buffer[44];

  int packetLen;
  bzero( (char *)buffer, sizeof(buffer) );

  TRipHeader *ripHeader; //  4B
  TRipngEntry *ripEntry; // 20B
  TRipngEntry *ripEntryHop; // 20B
  ripHeader = (TRipHeader *)buffer;
  //ripEntryHop = (TRipngEntry *)(buffer + sizeof(TRipHeader) + sizeof(TRipngEntry) );
  
  ripEntry = (TRipngEntry *)(buffer + sizeof(TRipHeader) );
  ripEntryHop = ripEntry;
  
  // RIP Header
  packetLen = sizeof(TRipHeader);
  ripHeader->command = 2; // Response
  ripHeader->version = 1; // version RIPng_v1
  ripHeader->zeros = 0;
    
  // RIP Next Hop
  if(nextHop != NULL){
    packetLen += sizeof(TRipngEntry);
    //strcpy((char *)ripEntryHop->ipPrefix, nextHop);
    inet_pton(AF_INET6, nextHop, (char *)ripEntryHop->ipPrefix );
    ripEntryHop->prefixLen = 0;
    ripEntryHop->routeTag = 0;
    ripEntryHop->metric = ~(0);
    // Posunuti rip entry
    ripEntry = (TRipngEntry *)(buffer + sizeof(TRipHeader) + sizeof(TRipngEntry) );
  }

  // RIP Entry
  packetLen += sizeof(TRipngEntry);
  inet_pton(AF_INET6, ipRoute, (char *)ripEntry->ipPrefix );
  ripEntry->routeTag = routeTag; // -t
  ripEntry->prefixLen = prefixLen; // prefix
  ripEntry->metric = metric; // -m

  // SEND RIP
  if( sendRipPacket(device, buffer, packetLen) ) {
    fprintf(stderr, "Doslo k chybe pri odesilani RIP paketu.\n");
  }

}


//--------------------------------

// ./myripresponse 
// -i <rozhraní> 
// -r <IPv6>/[16-128] 
// {-n <IPv6>} 
// {-m [0-16]} 
// {-t [0-65535]}

void printHelp(FILE* output) {
  fprintf(output, "\nPOUZITI: \n");
  fprintf(output, "  ./myripresponse -i <rozhrani> -r <IPv6>/[16-128] {-n <IPv6>} {-m [0-16]} {-t [0-65535]}\n");
  fprintf(output, "  ./myripresponse -h\n");
  fprintf(output, "\nPREPINACE:\n");
  fprintf(output, "  -i <rozhrani>\n");
  fprintf(output, "  \tRozhrani sitove karty, ze ktereho je odesilan paket.\n");
  fprintf(output, "  -r <IPv6>/[16-128]\n");
  fprintf(output, "  \tNastaveni podvrhavane routy.\n");
  fprintf(output, "  \tFormat hodnoty je IPv6 adresa site lomeno prefix site.\n");
  fprintf(output, "  -n <IPv6>\n");
  fprintf(output, "  \tZadani adresy pro next-hop.\n");
  fprintf(output, "  -m [0-16]\n");
  fprintf(output, "  \tZadani metriky.\n");
  fprintf(output, "  -h\n");
  fprintf(output, "  \tVypsani napovedy.\n");
  fprintf(output, "\n");
}

//=================================================

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

  TParam param = getParam(argc, argv);
  if( param.options & TOption_Help ) {
    printHelp(stdout);
  }
  else if( (param.options & TOption_I) && ( param.options & TOption_R) ) {
    broadcast( param.interface, param.ipRoute, param.prefixLen, param.nextHop, param.metric, param.routeTag );
  }
  else {
    // Print Help 
    fprintf(stderr," Parametry -i a -r nabyly nalezeny.\n");
    printHelp(stderr);
  }

	return 0;
}

// Soubor: myripresponse.c
