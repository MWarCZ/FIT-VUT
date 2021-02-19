//////////////////////////////////////////
//                                     //
// Projekt: ISA RIP                   //
// Soubor: myripsniffer.c            //
// Autor: Miroslav Valka (xvalka05) //
//                                 //
////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <signal.h>

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

///////////////////
// Definice Typu //
///////////////////

// Akce, ktera se ma vykonavat.
typedef enum {
  TAction_None = 0,
  TAction_Live = 1,
  TAction_Offline = 2,
  TAction_Help = 4
} TAction;

// Uroven vypisovani inforamci.
typedef enum {
  TPrintable_None = 0,
  TPrintable_RIP = 1,
  TPrintable_UDP = 2,
  TPrintable_IP = 4,
  TPrintable_ETH = 8
} TPrintable;

// Parametry aplikace.
typedef struct {
  char *deviceOrFilename;
  TAction action;
  TPrintable print;
} TParam;
 

///////////////////////
// Globalni promene  //
///////////////////////

/**
 * Handler na otevrene pcap spojeni.
 */
pcap_t *handlePcap;
/**
 * Uchovava nastaveni pro volbu informaci, 
 * ktere se vypisuji na vystup.
 */
TPrintable printable;

/**
 * Navratovy kod aplikace.
 */
int returnCode = 0;
  

///////////////////////////
// Funkce pro zpracovani //
///////////////////////////

/**
 * Funkce ETHERNET
 * Funkce zpracovava Ethernet hlavicku paketu.
 * @param printIt Maji se vypisovat inforamce o paketu.
 * @param packet Ukazatel na paket, kde se nachazi dana hlavicka.
 * @param packetWithoutEtherHeader Ukazatel na paket posunuty o zpracovanou hlavicku.
 * @param etherHeaderLen Velikost zpracovane hlavicky.
 * @param etherType Typ ethernetove komunikace (IPv4, IPv6)
 */
void EtherParse(
    int printIt,
    /*in*/ const u_char *packet,
    /*out*/ u_char **packetWithoutEtherHeader, int *etherHeaderLen, int *etherType
  ) {
  
  // ETHERNET
  struct ether_header *etherHeader;
  etherHeader = (struct ether_header *)packet;

  *etherHeaderLen = sizeof(struct ether_header);
  *etherType = ntohs(etherHeader->ether_type);
  *packetWithoutEtherHeader = (u_char *)(packet + *etherHeaderLen);  

  // Je nastaveno tisknuti? 
  if(printIt) {
    printf("==== Ethernet ====\n  Ethernet Type: ");
    switch(*etherType) {
      case ETHERTYPE_IP:
        printf("IPv4\n");
        break;
      case ETHERTYPE_IPV6:
        printf("IPv6\n");
        break;
      default:
        printf("Other\n");
    }
  } // if(printIt)

  return;
}

/**
 * Funkce IP
 * Funkce zpracovava IP hlavicku paketu.
 * @param printIt Maji se vypisovat inforamce o paketu.
 * @param packet Ukazatel na paket, kde se nachazi dana hlavicka.
 * @param packetWithoutIpHeader Ukazatel na paket posunuty o zpracovanou hlavicku.
 * @param ipHeaderLen Velikost zpracovane hlavicky.
 * @param protocol Protokol nasledujici hlavicky (TCP, UDP)
 */
void IpParse(
    int printIt,
    /*in*/ const u_char *packet,
    /*out*/ u_char **packetWithoutIpHeader, int *ipHeaderLen, u_char *protocol
  ) {
  
  // IP
  struct ip *ipHeader;
  ipHeader = (struct ip *)packet;
  
  int ipVersion = ipHeader->ip_v;

  // Ziskani informaci z IPv4
  if(ipVersion == 4) {
    *ipHeaderLen = ipHeader->ip_hl * 4;
    *packetWithoutIpHeader = (u_char *)(packet + *ipHeaderLen);  
    *protocol = ipHeader->ip_p;
  }
  // Ziskani informaci z IPv6
  else if(ipVersion == 6) {
    struct ip6_hdr *ip6Header = (struct ip6_hdr *)ipHeader;

    *ipHeaderLen = sizeof(struct ip6_hdr);
    *packetWithoutIpHeader = (u_char *)(packet + *ipHeaderLen);  
    *protocol = ip6Header->ip6_nxt;

    // TODO - extend headers u ipv6
  }

  // Je nastaveno tisknuti? 
  if(printIt) {
    char src4[INET_ADDRSTRLEN];
    char dst4[INET_ADDRSTRLEN];
    char src6[INET6_ADDRSTRLEN];
    char dst6[INET6_ADDRSTRLEN];
    printf("==== IPv%d ====\n  Header Length: %d B\n", ipVersion, *ipHeaderLen);

    // Vytisk dle verze IP
    switch(ipVersion) {
      case 4:
        printf("  Source: %s\n", inet_ntop(AF_INET, &(ipHeader->ip_src), src4, INET_ADDRSTRLEN));
        printf("  Destination: %s\n", inet_ntop(AF_INET, &(ipHeader->ip_dst), dst4, INET_ADDRSTRLEN));
        break;
      case 6:
        printf("  Source: %s\n", inet_ntop(AF_INET6, &( ((struct ip6_hdr *)ipHeader)->ip6_src ), src6, INET6_ADDRSTRLEN));
        printf("  Destination: %s\n", inet_ntop(AF_INET6, &( ((struct ip6_hdr *)ipHeader)->ip6_dst ), dst6, INET6_ADDRSTRLEN));
        break;
      default:
        printf("  Other IP (not v4 or v6)\n");
    }

    // Vytisknout jaky protokol naslduje.
    switch(*protocol) {
      case IPPROTO_TCP: 
        printf("  Protocol: TCP\n");
        break;
      case IPPROTO_UDP:
        printf("  Protocol: UDP\n");
        break;
      default:
        printf("  Protocol: NA\n");
    }
  } // if(printIt)

  return;
}

/**
 * Funkce UDP
 * Funkce zpracovava UDP hlavicku paketu. 
 * @param printIt Maji se vypisovat inforamce o paketu.
 * @param packet Ukazatel na paket, kde se nachazi dana hlavicka.
 * @param packetWithoutUdpHeader Ukazatel na paket posunuty o zpracovanou hlavicku.
 * @param udpHeaderLen Velikost zpracovane hlavicky.
 * @param packetWithoutUdpSize Velikost zbitku nezpracovaneho paketu.
 */
void UdpParse(
    int printIt,
    /*in*/ const u_char *packet,
    /*out*/ u_char **packetWithoutUdpHeader, int *udpHeaderLen, size_t *packetWithoutUdpSize
  ) {

  // UDP
  struct udphdr *udpHeader;
  udpHeader = (struct udphdr *)packet;
  *udpHeaderLen = sizeof(struct udphdr);
  *packetWithoutUdpHeader = (u_char *)(packet + *udpHeaderLen);
  u_int16_t ulen = ntohs(udpHeader->uh_ulen);

  *packetWithoutUdpSize = ulen - *udpHeaderLen;

  // Je nastaveno tisknuti?
  if(printIt) {
    printf("==== UDP ====\n  Length: %d\n", ulen);
    printf("       Source Port: %d\n", ntohs(udpHeader->uh_sport) );
    printf("  Destination Port: %d\n", ntohs(udpHeader->uh_dport) );
    printf("            Length: %d\n", ulen );
  }

  return;
}
 
/**
 * Pole pro prevod rip comandu do textove podoby.
 */
char RIP_COMMAND[6][10] = {
    "None", "Request", "Response", "Traceon", "Traceoff", "Reserved"
};
/**
 * Funkce prevede cislo comandu do jeho textove podoby.
 * @param command Hodnota rip commandu.
 * @return Vraci textovou reprezentaci commandu.
 */
char* RipCommandToStr(u_int8_t command) {
   return (command>5)?RIP_COMMAND[0]:RIP_COMMAND[command];
}
/**
 * Pole pro prevod rip typu autentizace do textove podoby.
 */
char RIP_AUTH_TYPE[5][30] = {
  "None", "Authentication Trailer", "Plain-text password", "Cryptographic Hash Function", "Unassigned"
};
/**
 * Funkce prevede cislo typu autentizace do jeho textove podoby.
 * @param type Hodnota rip typu autentizace.
 * @return Vraci textovou reprezentaci typu autentizace.
 */
char* RipAuthTypeToStr(u_int16_t type) {
   return (type>5)?RIP_AUTH_TYPE[4]:RIP_AUTH_TYPE[type];
}

/**
 * Funkce RIP
 * Funkce zpracovava RIP hlavicku paketu.
 * @param printIt Maji se vypisovat inforamce o paketu.
 * @param packet Ukazatel na paket, kde se nachazi dana hlavicka.
 * @param packetSize Velikost zpracovaneho paketu.
 * @param etherTye_IPvX Verze ip protokolu.
 */
void RipParse(
    int printIt,
    /*in*/ const u_char *packet, int etherType_IPvX, size_t packetSize
  ) {
 
  // RIP
  TRipHeader *ripHeader;
  ripHeader = (TRipHeader *)packet;
  int ripHeaderLen = sizeof(TRipHeader);
  // Velikost paketu obsahujici RIP Entry
  int ripEntryCount = packetSize - ripHeaderLen;


  if(etherType_IPvX == ETHERTYPE_IP) {
    TRipAuth *ripAuth;
    TRipEntry *ripEntry;
    ripEntry = (TRipEntry *)( (u_char *)(ripHeader)+sizeof(TRipHeader));
    char strIp4[INET_ADDRSTRLEN];
    int authType = 0;

    if(printIt) {
      printf("==== RIPv%d  %s ====\n",ripHeader->version, RipCommandToStr(ripHeader->command) );

      while(ripEntryCount > 0) {
        switch(ripEntry->addrFamilyId) {
          case 0xFFFF:
            ripAuth = (TRipAuth *)ripEntry;
            ripEntry++;
            ripEntryCount -= sizeof(TRipAuth);

            if(authType) {
              printf("  [Authentication Data]\n");
              break;
            }

            // TODO print auth type
            authType = ntohs(ripAuth->type);
            printf("  [Authentication (Type - %s)]\n",
               RipAuthTypeToStr(authType)
            );
            if( authType == 2) {
              printf("      Password: %s\n", ripAuth->auth);
            }
            else if( authType == 3) {
              printf("      { TODO: Vypis detailu }\n");
            }


            break;
          default:
            printf("  [Route (Family/Tag - %d/%d)] \n", 
              ntohs(ripEntry->addrFamilyId), 
              ntohs(ripEntry->routeTag) );
            printf("      IP Addres: %s\n", 
              inet_ntop(AF_INET, &(ripEntry->ipAddr), 
              strIp4, INET_ADDRSTRLEN));
        
            if(ripHeader->version == 2) {
              printf("    Subnet Mask: %s\n", 
                inet_ntop(AF_INET, &(ripEntry->subnetMask), 
                strIp4, INET_ADDRSTRLEN));
              printf("       Next Hop: %s\n", 
                inet_ntop(AF_INET, &(ripEntry->nextHop), 
                strIp4, INET_ADDRSTRLEN));
            }
        
            u_int32_t metric = ntohl(ripEntry->metric);
            printf("         Metric: %X\n", metric );
            // RIP Entry next
            ripEntry++;
            ripEntryCount-=sizeof(TRipEntry);

        } // switch
      } // While
    } // if(printIt)
  }
  else if( etherType_IPvX == ETHERTYPE_IPV6) {
    // RIPng
    TRipngEntry *ripngEntry;
    //ripngEntry = (TRipngEntry *)ripHeader;
    ripngEntry = (TRipngEntry *)( (u_char *)(ripHeader)+sizeof(TRipHeader));
     
 
    if(printIt) {
      char strIp6[INET6_ADDRSTRLEN];
      printf("==== RIPng %s ====\n", RipCommandToStr(ripHeader->command) );

      while(ripEntryCount > 0) {
        // Hop vs Route
        if(ripngEntry->metric == 0xff) {
          printf("  [Next Hop]\n");
          printf("    Hop Address: %s\n", inet_ntop(AF_INET6, ripngEntry->ipPrefix, strIp6, INET6_ADDRSTRLEN) );
        }
        else {
          printf("  [Route (Tag - %d)] \n", ntohs(ripngEntry->routeTag) );
          printf("    IP Address: %s/%d\n", inet_ntop(AF_INET6, ripngEntry->ipPrefix, strIp6, INET6_ADDRSTRLEN), ripngEntry->prefixLen );
          printf("        Metric: %d\n", ripngEntry->metric );
        
        }
        
        // RIP Entry next
        ripngEntry++;
        ripEntryCount-=sizeof(TRipngEntry);
      } // while
      
    } // if(printIt)

  }
  else {
    // ERR
    fprintf(stderr,"RIP not IPv4 or IPv6\n");
  }
  
  return;
}


/**
 * Smycka pro zpracovavani prijmanych paketu.
 * Parametry viz. callback u pcap_loop.
 */
void pcapLoopHandler( u_char *args, const struct pcap_pkthdr *header, const u_char *packet) {

  (void)args; (void)header, (void) packet;
  
  int printIt = 0; // Pro urceni zda tisknou inforamce

  printf(">====> START PACKET <====<\n");
  
  // Velikosti prijateho paketu
  if(printIt) { 
    printf(" Packet capture len: %d\n Packet total len: %d\n", header->caplen, header->len);
  }

  // Ukazatele pro zpracovani paketu
  u_char *tmpPacket, *retPacket;

  /// Ethernet
  int etherType; // ETHERTYPE_IP || ETHERTYPE_IPV6
  int etherHeaderLen;
  
  printIt = printable & TPrintable_ETH; // Tisknout info o ethernetu.
  EtherParse(printIt, packet, &retPacket, &etherHeaderLen, &etherType);
  tmpPacket = retPacket;

  /// IP
  int ipHeaderLen;
  u_char protocol; // IPPROTO_UDP || IPPROTO_TCP
 
  printIt = printable & TPrintable_IP; // Tisknout info o ip.
  IpParse(printIt, tmpPacket, &retPacket, &ipHeaderLen, &protocol );
  tmpPacket = retPacket;

  /// UDP
  int udpHeaderLen;
  size_t payloadLen;

  printIt = printable & TPrintable_UDP; // Tisknout info o udp.
  if(protocol == IPPROTO_UDP) {
    printIt = printable & TPrintable_UDP;
    UdpParse(printIt, tmpPacket, &retPacket, &udpHeaderLen, &payloadLen);
    tmpPacket = retPacket;
  }
  else {
    // ERR
    if(printIt) {
      printf("==== NOT UDP ====\n");
    }
  }

  /// RIP
  printIt = printable & TPrintable_RIP; // Tisknout info o rip.
  RipParse( printIt, tmpPacket, etherType, payloadLen); 
  
  // Ukonceni paketu.
  printf(">====> END PACKET <====<\n"); 
  printf("\n");


}

/**
 * Funkce otevre spojeni pro odposlech na rozhrani.
 * @param handle Otevrene spojeni bude vraceno zde.
 * @param device Rozhrani na kterem se bude poslouchat.
 * @param timeout_limit Timeout pro pokus o spojeni.
 * @param errbuf Buffer pro chybove hlasky.
 * @param ip Funkce sem vlozi ip daneho rozhrani.
 * @param subnetMask Funkce sem vlozi masku daneho rozhrani.
 */
int pcapOpenLive(pcap_t** handle, char* device, int timeout_limit, char* errbuf, bpf_u_int32* ip, bpf_u_int32* subnetMask ){

  // Zjisteni informaci o zarizeni
  if(pcap_lookupnet(device, ip, subnetMask, errbuf) == -1) {
    fprintf(stderr,"Chybejici informace o zarizeni: %s\n",device);
    ip = 0;
    subnetMask = 0;
  }

  // Otevreni naslouchani na sitove karte.
  *handle = pcap_open_live( 
      device, 
      BUFSIZ, 
      1, // Povoleni monitorovani  
      timeout_limit, 
      errbuf
    ); 
  // Kontrola zda se podarilo otevrit naslouchani na zarizeni
  if(*handle == NULL) {
    fprintf(stderr, "Nepodarilo se otevrit zarizeni '%s': %s\n", device, errbuf);
    return 2;
  }
  return 0;
}

/**
 * Funkce otevre spojeni pro odposlech na rozhrani.
 * @param handle Otevrene spojeni bude vraceno zde.
 * @param pcapFilemane Nazev souboru pcap/cap.
 * @param errbuf Buffer pro chybove hlasky.
 * @param ip Funkce sem vlozi ip daneho rozhrani.
 * @param subnetMask Funkce sem vlozi masku daneho rozhrani.
 */
int pcapOpenOffline(pcap_t** handle, char* pcapFilename, char* errbuf, bpf_u_int32* ip, bpf_u_int32* subnetMask ) {
  
  // Nastaveni neznamich informaci o zarizeni.(nutne pro filtr)
  *ip = PCAP_NETMASK_UNKNOWN;
  *subnetMask = PCAP_NETMASK_UNKNOWN;

  // Otevreni souboru pro zpracovani.
  *handle = pcap_open_offline(pcapFilename, errbuf);
  // Kontrola zda se podarilo otevrit soubor pro zpracovani.
  if(*handle == NULL) {
    fprintf(stderr, "Nepodarilo se otevrit soubor '%s': %s\n.", pcapFilename, errbuf);
    return 2;
  }
  return 0;
}


/**
 * Funkce obsahuje a ridi zpusob odposlechu a zpracovavani paketu.
 * @param pcapOnline Zpusob naslouchani.(0=pcap_open_offline; 1=pcap_open_online)
 * @param deviceOrFilename Nazev soubotu nebo rozhrani. (eth0|wlan0 or file.pcap)
 */
int tmpv3(int pcapOnline, char *deviceOrFilename) {
  char errbuf[PCAP_ERRBUF_SIZE];
  int timeout_limit = 1000; // milliseconds
  u_char *my_args = NULL;

  bpf_u_int32 subnetMask, ip;

  if(pcapOnline){
    // Otevreni naslouchani na zarizeni.
    if(pcapOpenLive(&handlePcap, deviceOrFilename, timeout_limit, errbuf, &ip, &subnetMask) ){
      // ERR
      return 2;
    };
    // OK 
  } 
  else {
    // Otevreni naslouchani ze souboru.
    if(pcapOpenOffline(&handlePcap, deviceOrFilename, errbuf, &ip, &subnetMask)) {
      // ERR
      return 2;
    };
    // OK
  }

  // Filtr
  struct bpf_program filter;
  // Nastaveni filtru na odchytavani RIP paketu
  char filterExp[] = "udp port 520 or udp port 521";

  if( pcap_compile(handlePcap, &filter, filterExp, 0, ip) == -1 ) {
    fprintf(stderr, "Spatny filtr - %s\n",pcap_geterr(handlePcap) );
    return 3;
  }
  if( pcap_setfilter(handlePcap, &filter) == -1 ) {
    fprintf(stderr, "Nepodarilo se nastavit filtr - %s\n",pcap_geterr(handlePcap) );
    return 3;
  }

  // dump
  pcap_loop(handlePcap, 0, pcapLoopHandler, my_args);

  return 0;
}

//=================================================

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
  
  param.action = TAction_None;
  param.deviceOrFilename = NULL;
  param.print = TPrintable_None;

  int opt;

  while( (opt=getopt(argc, (char* const*)argv, "i:f:hEIUR")) != -1 ) {
    switch(opt) {
      case 'i': 
        if(param.action) {
          fprintf(stderr," Neni mozne kombinovat parametry.");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr," -i: Je ocekavana hodnota. (eth0,wlan0,...)\n");
          exit(1);
        }
        param.deviceOrFilename = optarg;
        param.action = TAction_Live;
        break;
      case 'f':
        if(param.action) {
          fprintf(stderr," Neni mozne kombinovat parametry.");
          exit(1);
        }
        if(!optarg) {
          fprintf(stderr, " -f: Je ocekavana hodnota. (cesta k souboru .pcap)\n");
          exit(1);
        }
        param.deviceOrFilename = optarg;
        param.action = TAction_Offline;
        break;
      case 'E':
        param.print|=TPrintable_ETH;
        break;
      case 'I':
        param.print|=TPrintable_IP;
        break;
      case 'U':
        param.print|=TPrintable_UDP;
        break;
      case 'R':
        param.print|=TPrintable_RIP;
        break;
      default:
      param.action = TAction_Help;

    }
  } // while

  return param;
}

void printHelp(FILE* output) {
  fprintf(output, "\nPOUZITI: \n");
  fprintf(output, "  ./myripsniffer -i <rozhrani> {-E} {-I} {-U}\n");
  fprintf(output, "  ./myripsniffer -f <soubor.pcap> {-E} {-I} {-U}\n");
  fprintf(output, "  ./myripsniffer -h\n");
  fprintf(output, "\nPREPINACE:\n");
  fprintf(output, "  -i <rozhrani>\n");
  fprintf(output, "  \tRozhrani sitove karty na kterem se bude naslouchat.\n");
  fprintf(output, "  \tNelze kombinovat s -f.\n");
  fprintf(output, "  -f <soubor.pcap>\n");
  fprintf(output, "  \tNazev soboru obsahujici zachycenou sitovou komunikaci.\n");
  fprintf(output, "  \tNelze kombinovat s -i.\n");
  fprintf(output, "  -E\n");
  fprintf(output, "  \tBudou se vypisovat i nektere informace z eternetove hlavicky.\n");
  fprintf(output, "  -I\n");
  fprintf(output, "  \tBudou se vypisovat i nektere informace z ip hlavicky.\n");
  fprintf(output, "  -U\n");
  fprintf(output, "  \tBudou se vypisovat i nektere informace z udp hlavicky.\n");
  fprintf(output, "  -h\n");
  fprintf(output, "  \tVypise napovedu.\n");
  fprintf(output, "\n");
}

/**
 * Funkce obsluhujici preruseni.
 * Tatu funkce musi byt spustena pred ukoncenim aplikace.
 * @param sig Hodnota signalu preruseni.
 */
void signalClose(int sig) {
  printf("Ukoncovani ...");
  if(handlePcap != NULL) {
    pcap_close(handlePcap);
  }
  printf("\rAplikace ukoncena. (sig:%d)\n",sig);
  exit(returnCode);
}

//=================================================
int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

  // Nastaveni obsluhy signalu pro korektni ukonceni aplikace.
  signal(SIGINT, signalClose);
  signal(SIGTERM, signalClose);
  signal(SIGQUIT, signalClose);

  // Ziskani parametru.
  TParam param = getParam(argc, argv);
  returnCode = 0;
  
  // Vzdy chceme tisknout info z casti paketu o RIP.
  printable = param.print | TPrintable_RIP;


  // Nacitani paketu ze souboru
  if(param.action == TAction_Offline) {
    returnCode = tmpv3(0, param.deviceOrFilename); 
  }
  // Nacitani paketu zive z rozhrani
  else if(param.action == TAction_Live) {
    returnCode = tmpv3(1, param.deviceOrFilename); 
  }
  // Vypsani napovedy
  else {
    printHelp(stdout);
    exit(0);
  }

  signalClose(0); 

	return returnCode;
}

// Soubor: myripsniffer.c
