//////////////////////////////////////////
// Projekt: IFJ17                       //
// Soubor: src/main.c                   //
// Tým:                                 //
//   xvalka05 Miroslav Válka            //
//   xtrnen03 Jan Trněný                //
//   xproko37 Lukáš Prokop              //
//   xbarto93 Pavel Bartoň              //
//////////////////////////////////////////

#include <stdio.h>

#include "main.h"
#include "sample.h"
#include "parser.h"
#include "error.h"
#include "token.h"


/**
 * @brief Hlavni funkce main.
 * @param argc Počet argumentu v argv.
 * @param argv Pole argumentů získaných z příkazové řádky.
 * @return Vraci 0 pokud aplikace proběhla vpořádku.
 */
int main() {
	SayHelloWorld();
  TTokenType tmp=0;
  do {
    TToken* tkn = GetNextToken();
    PrintTokenType(tkn->type);
    tmp = tkn->type;
    TokenDestroy(tkn);
  } while(tmp != TK_EOF && !ERR_EXIT_STATUS);
	return 0;
}

// Soubor: src/main.c

