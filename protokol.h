
/* Definice protokolu */

#define P_NAME "mwarcz"
#define LEN_NAME 6

#define P_VERSION "01"
#define LEN_VERSION 2

#define P_SEP ":"
#define LEN_SEP 1

#define P_SUCC_RES "ok"
#define LEN_SUCC_RES 2

#define P_FAIL_RES "ko"
#define LEN_FAIL_RES 2

#define P_GET_USER_INFO "n"
#define P_GET_USER_HOME_DIR "f"
#define P_GET_USERS_LIST "l"
#define LEN_GET 1

// mwarcz:01
#define P_REQ_HEAD P_NAME P_SEP P_VERSION

// mwarcz:ok
#define PROT_SUCC_RES P_NAME P_SEP P_SUCC_RES

// mwarcz:ko
#define PROT_FAIL_RES P_NAME P_SEP P_FAIL_RES

// mwarcz:01:n:
#define PROT_REQ_GET_USER_INFO P_REQ_HEAD P_SEP P_GET_USER_INFO P_SEP

// mwarcz:01:f:
#define PROT_REQ_GET_USER_HOME_DIR P_REQ_HEAD P_SEP P_GET_USER_HOME_DIR P_SEP

// mwarcz:01:l:
#define PROT_REQ_GET_USERS_LIST P_REQ_HEAD P_SEP P_GET_USERS_LIST P_SEP


// Parsovani PROT_REQ_...

// index od-do pro ziskani NAME
#define PARSE_NAME 0
#define PARSE_NAME_END ( LEN_NAME )

// index od-do pro ziskani VERSION
#define PARSE_VERSION ( LEN_NAME+LEN_SEP )
#define PARSE_VERSION_END ( LEN_NAME+LEN_SEP+LEN_VERSION )

// index od-do pro ziskani akce GET
#define PARSE_GET ( LEN_NAME+LEN_SEP+LEN_VERSION+LEN_SEP )
#define PARSE_GET_END ( LEN_NAME+LEN_SEP+LEN_VERSION+LEN_SEP+LEN_GET )

// index od pro ziskani dat
#define PARSE_DATA ( LEN_NAME+LEN_SEP+LEN_VERSION+LEN_SEP+LEN_GET+LEN_SEP )





//---------

