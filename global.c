
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/types.h>

#include "protokol.h"
#include "global.h"


/**
 * Zkontroluje jestli je textovy string cislo ci nikoli.
 * @param  str Textovy retezec, ktery muze byt cislem.
 * @return     Vraci 1 pokud str je cislo, jinak 0.
 */
int CheckNumber(char* str) {
  int len = strlen(str);
  for(int i = 0; i < len; i++ ) {
    if(isdigit(str[i]) == 0)
      return 0;
  }
  return 1;
}


int SendMsg(int* socket, char* buff) {
  int request = send(*socket, buff, strlen(buff), 0);
  if (request < 0) {
    DIE(Error_Send,"Doslo k chybe pri odesilani dat. ( send )\n");
  }
  return request;
}
int RecvMsg(int* socket, char* buff) {
  int reply = recv(*socket, buff, BUFSIZE, 0);
  if (reply < 0) {
    DIE(Error_Recv,"Doslo pri chybe pri prijmani dat. ( recv )\n");
  }
  return reply;
}

int SendOK(int* socket) {
  char* msgEnd = PROT_SUCC_RES;
  return SendMsg(socket, msgEnd);
}

int SendKO(int* socket) {
  char* msgEnd = PROT_FAIL_RES;
  return SendMsg(socket, msgEnd);
}

int SendAndWait(int *socket, char* inputBuff, char* outputBuff) {
  int res = 0;

  res = SendMsg(socket, inputBuff);
  res = RecvMsg(socket, outputBuff);

  return res;
}

