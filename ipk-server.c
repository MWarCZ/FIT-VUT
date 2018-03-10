
#include <stdio.h>
#include <stdlib.h>
#include <string.h>//
#include <fcntl.h>
#include <errno.h>//
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <ctype.h> //


#include "protokol.h"
#include "global.h"

#include "debug.h"


// ./ipk-server -p port 
Param ServerApp(int argc, char** argv) {
  Param param;
  param.action = Action_None;
  param.login = NULL;
  param.host = NULL;
  param.port = 0;

  Option p = Option_None;
  int pValue = 0;

  int opt;
  
  while( (opt=getopt( argc,(char* const*) argv, "p:" )) != -1 ) {

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
  if( !p ) { /// err
    debug("p: %i\n", p);
    DIE(Error_InputArgs, "Chybejici parametr -p.\n");
  }

  
  param.port = pValue;

  return param;
}


//
int OpenServer(int portNumber, int* welcomeSocket) {
  int rc;
  struct sockaddr_in6 sa;

  if ((*welcomeSocket = socket(PF_INET6, SOCK_STREAM, 0)) < 0) {
    DIE(Error_CreateServer, "Chyba pri vytvareni socketu.\n");
  }
  
  int on = 1;
  rc = setsockopt(*welcomeSocket, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
  if (rc < 0) {
    close(*welcomeSocket);
    DIE(Error_CreateServer, "Chyba pri nastavovani socketu.\n");
  }
  
  memset(&sa,0,sizeof(sa));
  sa.sin6_family = AF_INET6;
  sa.sin6_addr = in6addr_any;
  sa.sin6_port = htons(portNumber);  

  if ((rc = bind(*welcomeSocket, (struct sockaddr*)&sa, sizeof(sa))) < 0) {
    DIE(Error_CreateServer, "Chyba pri otvirani portu. ( bind )\n");
  }
    
  if ((rc = listen(*welcomeSocket, 1)) < 0) {
    DIE(Error_CreateServer, "Chyba pri naslouchani na portu. ( listen )\n");  
  }

  return 0;
}

// -n značí, že bude vráceno plné jméno uživatele včetně případných dalších informací pro uvedený login (User ID Info);
int SendUserInfo(int* comSocket, char* login) {
  int result = 0;
  struct passwd *pw;
  char buff[BUFSIZE];
  int res = 0;

  setpwent();

  if( (pw = getpwnam( login ))!=(struct passwd *)0 ) {
    // login nalezen
    
      // debug("Login: %s\n", pw->pw_name);
      // debug("Passwd: %s\n", pw->pw_passwd);
      // debug("UID: %i\n", pw->pw_uid);
      // debug("GID: %i\n", pw->pw_gid);
      // debug("Gecos: %s\n", pw->pw_gecos);
      // debug("Dir: %s\n", pw->pw_dir);
      // debug("Shell: %s\n", pw->pw_shell);

    // Odesilani dat

    // Login
    res = SendAndWait(comSocket, pw->pw_name, buff);

    // UID
    snprintf( buff, sizeof(buff), "%i",pw->pw_uid);
    res = SendAndWait(comSocket, buff, buff);

    // Gecos - Name + Info
    res = SendAndWait(comSocket, pw->pw_gecos, buff);

    SendOK(comSocket);
    result = 1;
  }
  else {
    // login nenalezen
    SendKO(comSocket);
    result = 0;
  }

  endpwent();
  return result;
}

int SendUsersList(int* comSocket, char* login) {
  struct passwd *pw;
  char buff[BUFSIZE];
  int res = 0;

  setpwent();
  while( (pw=getpwent() )!=(struct passwd *)0 ) {
    if( strncmp(login, pw->pw_name, strlen(login) ) == 0 ) {
      snprintf( buff, sizeof(buff), "%s",pw->pw_name );
      debug(">%s\n", buff);
      res = SendAndWait(comSocket, buff, buff);
    }

  }  
  endpwent();

  SendOK(comSocket);
  return 1;
}

// -f značí, že bude vrácena informace o domácím adresáři uživatele pro uvedený login (Home directory);
int SendHomeDir(int* comSocket, char* login ) {
  int result = 0;
  struct passwd *pw;
  char buff[BUFSIZE];
  int res = 0;

  setpwent();

  if( (pw = getpwnam( login ))!=(struct passwd *)0 ) {
    // login nalezen
    debug(">%s\n", pw->pw_dir);
    res = SendAndWait(comSocket, pw->pw_dir, buff);

    SendOK(comSocket);
    result = 1;
  }
  else {
    // login nenalezen
    debug(">Login '%s' nenalezen.\n", login);
    SendKO(comSocket);
    result = 0;
  }

  endpwent();
  return result;
}


//
int MainServer(int * welcomeSocket) {
  struct sockaddr_in6 saClient;
  char str[INET6_ADDRSTRLEN];
  socklen_t saClientLen = sizeof(saClient);
  
  while(1) {
    int comSocket = accept(*welcomeSocket, (struct sockaddr*)&saClient, &saClientLen); 

    if (comSocket <= 0)
      continue;

    int pid = fork();
    if (pid < 0) {    
      DIE(Error_RunTimeServer, "Chyba pri rozdelovani procesu. ( fork )"); 
    }
    
    if (pid == 0) {
      int child_pid = getpid(); 
      INFO("\n############################\n");  
      INFO("%d: Nove pripojeni - Vytvoren obsluzny proces %d.\n", child_pid, child_pid);

      if(inet_ntop(AF_INET6, &saClient.sin6_addr, str, sizeof(str))) {
        INFO("%d: Klient: %s:%d\n",child_pid, str, htons(saClient.sin6_port) );
      }
      
      char buff[BUFSIZE];
      int res = 0;

      for (;;) { 
        res = recv(comSocket, buff, BUFSIZE, 0);        
        if (res > 0) {
          // Splnuji prijeta data minimalni delku, aby slo o platny request?
          if(res >= PARSE_DATA) {
            // Parsovani prijatych dat
            buff[res] = '\0';
            buff[PARSE_NAME_END] = '\0';
            buff[PARSE_VERSION_END] = '\0';
            buff[PARSE_GET_END] = '\0';

            // Kontrolni vypis
            debug(":>Protokol: %s\n", &buff[PARSE_NAME] );
            debug(":>Verze: %s\n", &buff[PARSE_VERSION] );
            debug(":>Akce: %s\n", &buff[PARSE_GET] );
            debug(":>Login: %s\n\n", &buff[PARSE_DATA] );

            // Overeni komunikacniho protokolu
            if( strcmp(&buff[PARSE_NAME], P_NAME) == 0 ) {
              // Overeni verze protokolu
              if( strcmp(&buff[PARSE_VERSION], P_VERSION) == 0 ) {
                // Rozpoznani typu zadosti o data
                if( strcmp(&buff[PARSE_GET], P_GET_USER_INFO ) == 0 ) {
                  SendUserInfo(&comSocket, &buff[PARSE_DATA] );
                }
                else if( strcmp(&buff[PARSE_GET], P_GET_USER_HOME_DIR ) == 0 ) {
                  SendHomeDir(&comSocket, &buff[PARSE_DATA] );
                }
                else if( strcmp(&buff[PARSE_GET], P_GET_USERS_LIST ) == 0 ) {
                  SendUsersList(&comSocket, &buff[PARSE_DATA] );
                }
                else {
                  // Neznama zadost na data
                  SendKO(&comSocket);
                }
              }
              else {
                // Neznama verze protokolu
                SendKO(&comSocket);
              }
            }
            else {
              // Neznami protokol
              SendKO(&comSocket);
            }
          }
          else {
            // Prijata sprava nema dostatecnou velikost, aby mohla byt povazovana za platnou
            SendKO(&comSocket);
          }
        }
        else {
          break;                        
        }
      } // for(;;)
      
      INFO("%d: Konec pripojeni - Klient: %s\n",child_pid, str);
      close(comSocket);     
      exit(0);
    }
    else {
      close(comSocket);  
    }

  } // while(1)
}


//main
int main(int argc, char** argv) {

  Param param = ServerApp(argc, argv);

  int welcomeSocket;

  INFO("Zapinani serveru...\n");

  OpenServer(param.port, &welcomeSocket);

  INFO("Server spusten na portu: %d\n", param.port);

  MainServer(&welcomeSocket);

  INFO("Vypinani serveru.\n");

  return 0;
}

