/* TRISCA Ionut - 312CB */

#include "header.h"

void * InitS(size_t d)
	/*-- Initializare Stiva --*/
{
	TStiva * s;
	s = (TStiva *)malloc(sizeof(TStiva));

	if(!s)
	{
		return NULL;
	}

	s->dime = d;
	s->ref = NULL;

	return (void *)s;
}

int Push(void * s, void * ae)
	/*-- Introducere element in stiva --*/
{
	TLG aux;
	aux = AlocCelulaG(ae, ((TStiva * )s)->dime);

	if(!aux)
	{
		return 0;
	}

	aux->urm = ((TStiva * )s)->ref;
	((TStiva * )s)->ref = aux;
	return 1;
}

int Pop(void * s, void * ae)
	/*-- Extragere element din stiva --*/
{
	if(!((TStiva *)s)->ref)
	{
		return 0;
	}

	TLG aux;

	memcpy(ae, ((TStiva * )s)->ref->info, ((TStiva * )s)->dime);
	aux = ((TStiva * )s)->ref;

	((TStiva * )s)->ref = aux->urm;

	free(aux->info);
	free(aux);
	return 1;
}

void DistrugeS(void ** s)
	/*-- Distrugere stiva --*/
{
	int ae;

	while(Pop(*s, &ae));
	
	free((*s));
}