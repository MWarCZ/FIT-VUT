
typedef enum {
  Option_None = 0,
  Option_P = 1,
  Option_H = 2,
  Option_N = 4,
  Option_F = 8,
  Option_L = 16
} Option;

#define BUFSIZE 1024

/* Definice akci */

typedef enum {
  Action_None = 0,
  Action_Help,
  Action_GetUserInfo,
  Action_GetUserHomeDir,
  Action_GetUsersList
} Action;

typedef struct {
  Action action;
  char *login;
  char *host;
  int port;
} Param;

/* Definice chyb */

typedef enum {
  Error_None = 0,
  Error_InputArgs = 1,
  Error_CreateServer = 2,
  Error_RunTimeServer = 3,
  Error_ConnectToServer = 4,
  
  Error_Unknown = 99
} Error;

#define DIE(exitCode,msg,...) fprintf(stderr, msg, ##__VA_ARGS__); exit(exitCode);

#define INFO(msg,...) fprintf(stderr, msg, ##__VA_ARGS__);
