//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
	pHead = NULL;
}

PriorityQueue::~PriorityQueue()
{
	while(pHead != NULL)
	{
		Remove(pHead->value);
	}
}

void PriorityQueue::Insert(int value)
{

	Element_t *pNewEl = new Element_t();
	pNewEl->value = value;
	
	if(pHead == NULL)
	{
		pNewEl->pPrev = NULL;
		pNewEl->pNext = NULL;
		pHead = pNewEl;
		return;
	}
	
	bool insertBefore = true;
	Element_t *pEl = pHead;
	while( pEl->value < value )
	{
		if(pEl->pNext !=NULL)
		{
			pEl = pEl->pNext;
		}
		else
		{
			insertBefore = false;
			break;
		}
	}
	
	if(insertBefore)
	{
		pNewEl->pPrev = pEl->pPrev;
		pEl->pPrev = pNewEl;
		pNewEl->pNext = pEl;
		if(pNewEl->pPrev != NULL)
		{
			pNewEl->pPrev->pNext = pNewEl;
		}
		else
		{
			pHead = pNewEl;
		}
	}
	else
	{
		pNewEl->pNext = pEl->pNext;
		pNewEl->pPrev = pEl;
		pEl->pNext = pNewEl;
		if(pNewEl->pNext!=NULL)
		{
			pNewEl->pNext->pPrev = pNewEl;
		}
	}
}

bool PriorityQueue::Remove(int value)
{
	Element_t *pEl = Find(value);
	if(pEl == NULL)
	{	
		return false;
	}
	if(pEl->pPrev != NULL)
	{
		(pEl->pPrev)->pNext = pEl->pNext;
	}
	else
	{
		pHead = pEl->pNext;
	}
	if(pEl->pNext != NULL)
	{
		pEl->pNext->pPrev = pEl->pPrev;
	}
	delete pEl;
	return true;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
	Element_t *pEl = pHead;
	while( pEl != NULL && pEl->value != value )
	{
		pEl = pEl->pNext;
	}
	return pEl;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
	return pHead;
}

/*** Konec souboru tdd_code.cpp ***/
