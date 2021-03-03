/* TRISCA Ionut - 312CB */

#include "header.h"

TLG AlocCelulaG(void * ax, size_t d)
	/*-- Alocare memorie pentru o celula de lista generica simpla --*/
{
	TLG aux;
	aux = (TLG)malloc(sizeof(TCelulaG));

	if(!aux)
	{
		return NULL;
	}

	aux->urm = NULL;
	aux->info = malloc(d);

	if(!aux->info)
	{
		free(aux);
		return NULL;
	}

	memcpy(aux->info, ax, d);
	return aux;
}

TLDG AlocCelulaDG(void * ax, size_t d)
	/*-- Alocare memorie pentru o celula de lista generica dubla --*/
{
	TLDG aux;
	aux = (TLDG)malloc(sizeof(TCelulaDG));

	if(!aux)
	{
		return NULL;
	}

	aux->urm = NULL;
	aux->ant = NULL;
	aux->info = malloc(d);
	
	if(!aux->info)
	{
		free(aux);
		return NULL;
	}

	memcpy(aux->info, ax, d);
	return aux;
}

int cmp(void * a, void * b)
	/*-- Functie de comparatie a PID-urilor --*/
{
	return (((TProces *)a)->PID == ((TProces *)b)->PID);
}

void ElibElV(void * Proces)
	/*-- Functie de eliberare a unui proces --*/
{
	free(Proces);
}

void ElimElDG(ALDG aL, void * Proces, void (* ElibEl)(void *),
			int (* cmp)(void *, void *))
	/*-- Eliminare a unui element dintr-o lista dubla --*/
{
	TLDG aux = *aL;

	while(aux)
	{
		if(cmp(Proces, aux->info))
		{

			if(aux->ant)
			{
				if(aux->urm)
				{
					aux->ant->urm = aux->urm;
					aux->urm->ant = aux->ant;
				}
				else
				{
					aux->ant->urm = aux->urm;
				}
			}
			else
			{
				*aL = (*aL)->urm;
				if((*aL))
					if((*aL)->ant)
					(*aL)->ant = NULL;
			}

			ElibEl(aux->info);
			free(aux);
			return;
		}
		aux = aux->urm;
	}
}

int IntrDupaRef(ALDG aL, void * ref, void * ae, int (* cmp)(void *, void *))
	/*-- Introduce element in lista dubla dupa un element de referinta --*/
{
	TLDG aux = AlocCelulaDG(ae, sizeof(TProces)), p = *aL;

	if(!aux)
	{
		return 0;
	}

	while(p)
	{
		if(cmp(p->info, ref))
		{
			if(p->urm)
			{
				aux->urm = p->urm;
				p->urm->ant = aux;
				p->urm = aux;
				aux->ant = p;
			}
			else
			{
				p->urm = aux;
				aux->ant = p;
			}
			return 1;
		}
		p = p->urm;
	}

	return 0;
}

int IntrInainteRef(ALDG aL, void * ref, void * ae, 
					int (* cmp)(void *, void *))
	/*-- Introducere in lista dubla inaintea unui element de referinta --*/
{

	TLDG aux = AlocCelulaDG(ae, sizeof(TProces));
	TLDG p = *aL;

	if(!aux)
	{
		return 0;
	}

	while(p)
	{
		if(cmp(p->info, ref))
		{
			if(p->ant);

			else
			{
				aux->urm = p;
				p->ant = aux;
				(*aL) = aux;
			}
			return 1;
		}
		p = p->urm;
	}

	return 0;
}

void DistrugeLDG(ALDG aL, void (* ElibEl)(void *))
	/*-- Distrugere lista dublu inlantuita --*/
{
	TLDG aux = NULL;
	while(*aL)
	{
		aux = *aL;
		*aL = (*aL)->urm;
		ElibEl(aux->info);
		free(aux);
	}
}