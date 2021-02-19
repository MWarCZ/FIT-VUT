/////////////////////////////
/// Soubor: global.c      ///
/// Autor: Miroslav Valka ///
/////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "global.h"


int CheckNumber(char* str) {
  int len = strlen(str);
  for(int i = 0; i < len; i++ ) {
    if(isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}


int SendtoMsg(int* socket, char* buff, int bufsize, struct sockaddr *psaClient, socklen_t *saClientLen) {
  int request = sendto(*socket, buff, bufsize, 0, psaClient, *saClientLen);
  if (request < 0) {
    DIE(Error_Sendto,"Doslo k chybe pri odesilani dat. ( sendto )\n");
  }
  return request;
}

int RecvfromMsg(int* socket, char* buff, int bufsize, struct sockaddr *psaClient, socklen_t *saClientLen) {
  int reply = recvfrom(*socket, buff, bufsize, 0, psaClient, saClientLen);
  if (reply < 0) {
    DIE(Error_Recvfrom,"Doslo k chybe pri prijmani dat. ( recvfrom )\n");
  }
  return reply;
}

int Select(int* socket, struct timeval* waitTime) {
  fd_set mySet;

  FD_ZERO(&mySet);
  FD_SET(*socket, &mySet);
  if (select(*socket + 1, &mySet, NULL, NULL, waitTime) < 0) {
    DIE(Error_Select,"Doslo k chybe pri cekani na data. ( select )\n");
  }
  if (!FD_ISSET(*socket, &mySet)) {
    return 0;
  }
  return 1;
}

/// KonecSouboru: global.c
