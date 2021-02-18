/////////////////////////////
/// Soubor: protokol.h    ///
/// Autor: Miroslav Valka ///
/////////////////////////////

#ifndef __PROTOKOL_H__
#define __PROTOKOL_H__

/* Definice protokolu */

/**
 * Nazev protokolu.
 * @type char[]
 */
#define P_NAME "mwarcz"
/**
 * Delka nazvu protokolu.
 * @type int
 */
#define LEN_NAME 6


/**
 * Verze protokolu.
 * @type char[]
 */
#define P_VERSION "01"
/**
 * Delka verze protokolu.
 * @type int
 */
#define LEN_VERSION 2


/**
 * Oddelovac, kterym jsou oddeleny jednotlive casti protokolu.
 * @type char[]
 */
#define P_SEP ":"
/**
 * Delka oddvelovace. 
 * @type int
 */
#define LEN_SEP 1


/**
 * Cast protokolu pro kladnou odpoved.
 * @type char[]
 */
#define P_SUCC_RES "ok"
/**
 * Delka casti protokolu pro kladnou odpoved.
 * @type int
 */
#define LEN_SUCC_RES 2


/**
 * Cast protokolu pro negativni odpoved.
 * @type char[]
 */
#define P_FAIL_RES "ko"
/**
 * Delka casti protokolu pro negativni odpoved.
 * @type int
 */
#define LEN_FAIL_RES 2


/**
 * Cast protokolu pro zadost o informace o uzivateli.
 * @type char[]
 */
#define P_GET_USER_INFO "n"
/**
 * Cast protokolu pro zadost o domovsky adresar uzivatele.
 * @type char[]
 */
#define P_GET_USER_HOME_DIR "f"
/**
 * Cast protokolu pro zadost o seznam loginu uzivatelu.
 * @type char[]
 */
#define P_GET_USERS_LIST "l"
/**
 * Delka cast protokolu pro zadost.
 * @type int
 */
#define LEN_GET 1


/**
 * Hlavicka protokolu pro zadosti.
 * @type char[]
 */
#define P_REQ_HEAD P_NAME P_SEP P_VERSION
// mwarcz:01

/**
 * Protokol - Kladna odpoved.
 * @type char[]
 */
#define PROT_SUCC_RES P_NAME P_SEP P_SUCC_RES
// mwarcz:ok

/**
 * Protokol - Negativni odpoved.
 * @type char[]
 */
#define PROT_FAIL_RES P_NAME P_SEP P_FAIL_RES
// mwarcz:ko

/**
 * Protokol - Zadost o zaslani informacich o ozivateli.
 * @type char[]
 */
#define PROT_REQ_GET_USER_INFO P_REQ_HEAD P_SEP P_GET_USER_INFO P_SEP
// mwarcz:01:n:

/**
 * Protokol - Zadost o zaslani domovskeho adresare uzivatele.
 * @type char[]
 */
#define PROT_REQ_GET_USER_HOME_DIR P_REQ_HEAD P_SEP P_GET_USER_HOME_DIR P_SEP
// mwarcz:01:f:

/**
 * Protokol - Zadost o zaslani seznamu loginu uzivatelu.
 * @type char[]
 */
#define PROT_REQ_GET_USERS_LIST P_REQ_HEAD P_SEP P_GET_USERS_LIST P_SEP
// mwarcz:01:l:


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


#endif

/// KonecSouboru: protokol.h
