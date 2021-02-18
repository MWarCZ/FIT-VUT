	
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, říjen 2017
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu, 
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem, 
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu, 
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
** MV: Ehmm. No ok, takže mám komentovat všechny čachry machry s ukazately? Procedury obsahujou bud podmínky, které ukončí proceduru, či právě ono přesouvání ukazatelů. ??? 
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
	// ^ ^ ^ viz popis funkce ^ ^ ^
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;	
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
	tDLElemPtr pNow = NULL, pNext = NULL; 
	pNow = L->First;
	do {
		pNext = pNow->rptr; // Záloha ukazatele na další item.
		free(pNow);
		pNow = pNext; 
	}while(pNow != NULL);
	// Pro jistotku inicializace (Nechceme přeci ukazovat do ******)
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	// No podle mne opět nevhodný návrh typedef. Zbytečne míchá typ ukazatel a typ struktura. Tyto čachry jsou zvikem pro Pascal a Delphi ne pro jazyk C.
	tDLElemPtr pNew = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(pNew == NULL){
		DLError();
		return;
	}
	pNew->data = val;
	pNew->lptr = NULL;
	// Různe akce dle plnosti seznamu.
	if(L->First == NULL) {
		pNew->rptr = NULL;
		L->First = pNew;
		L->Last = pNew;
	}
	else {
		pNew->rptr = L->First;
		L->First->lptr = pNew;
		L->First = pNew;
	}
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 		
	// Viz předchozí funkce jen zdrcadlově otočeno.
	tDLElemPtr pNew = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(pNew == NULL){
		DLError();
		return;
	}
	pNew->data = val;
	pNew->rptr = NULL;
	
	if(L->Last == NULL) {
		pNew->lptr = NULL;
		L->First = pNew;
		L->Last = pNew;
	}
	else {
		pNew->lptr = L->Last;
		L->Last->rptr = pNew;
		L->Last = pNew;
	}
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;

}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	
	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->First == NULL) {
		DLError();
		return;
	}	  
	*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

	if(L->Last == NULL) {
		DLError();
		return;
	}
	*val = L->Last->data;
	
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
	if(L->First == NULL) return;
	// Nastaveni aktivniho prvku.
	L->Act = (L->First == L->Act)?NULL:L->Act;
	// Oplět pouhé prohazování ukazatelů a odlišný postup podle plnosti listu.
	tDLElemPtr pOld = L->First;
	L->First = pOld->rptr;
	if(L->First == NULL) {
		L->Last = NULL;
	}
	else {
		L->First->lptr = NULL;
	}
	free(pOld); // Uvolnění odpadu
	
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 
	// Viz předchozí funkce, jen zrcadlově otočená.
	if(L->Last == NULL) return;
	L->Act = (L->Last == L->Act)?NULL:L->Act;
	tDLElemPtr pOld = L->Last;
	L->Last = pOld->lptr;
	if(L->Last == NULL) {
		L->First = NULL;
	}
	else {
		L->Last->rptr = NULL;
	}
	free(pOld);
		
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	// Stejný princip jako předchozí dvě procedury.
	if(L->Act == NULL || L->Act == L->Last ) return;
	tDLElemPtr pOld = L->Act->rptr;
	L->Act->rptr = pOld->rptr;
	if(pOld->rptr == NULL) {
		L->Last = L->Act;
	}
	else {
		pOld->rptr->lptr = L->Act;
	}
	free(pOld);
		
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if(L->Act == NULL || L->Act == L->First ) return;
	tDLElemPtr pOld = L->Act->lptr;
	L->Act->lptr = pOld->lptr;
	if(pOld->lptr == NULL) {
		L->First = L->Act;
	}
	else {
		pOld->lptr->rptr = L->Act;
	}
	free(pOld);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL) return;
	tDLElemPtr pNew = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(pNew == NULL){
		DLError();
		return;
	}
	pNew->data = val;
	pNew->rptr = L->Act->rptr;
	pNew->lptr = L->Act;
	L->Act->rptr = pNew;
	if(pNew->rptr == NULL){
		L->Last = pNew;
	}
	else {
		pNew->rptr->lptr = pNew;
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if(L->Act == NULL) return;
	tDLElemPtr pNew = (tDLElemPtr)malloc(sizeof(struct tDLElem));
	if(pNew == NULL){
		DLError();
		return;
	}
	pNew->data = val;
	pNew->lptr = L->Act->lptr;
	pNew->rptr = L->Act;
	L->Act->lptr = pNew;
	if(pNew->lptr == NULL){
		L->First = pNew;
	}
	else {
		pNew->lptr->rptr = pNew;
	}
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if(L->Act == NULL) {
		DLError();
		return;
	}
	*val = L->Act->data;
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if(L->Act == NULL) return;
	L->Act->data = val;
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL) return;
	L->Act = L->Act->rptr;
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act == NULL) return;
	L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act == NULL)?0:1;
}

/* Konec c206.c*/
