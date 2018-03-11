/////////////////////////////
/// Soubor: debug.h       ///
/// Autor: Miroslav Valka ///
/////////////////////////////

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef DEBUG
/**
 * Makro vypisujici na stderr formatovany text pokud je definovano markro DEBUG. 
 * Pokud makro DEBUG neexistuje, tak neni vypsano nic.
 * @param  str         Formatovaci textovy retezec.
 * @param  __VA_ARGS__ Dalsi parametry, ktere budou vkladany do formatovaciho textoveho retezce.
 */
#define debug(str,...) fprintf(stderr, str, ##__VA_ARGS__);
#else
#define debug(...) 
#endif

#endif

/// KonecSouboru: debug.h
