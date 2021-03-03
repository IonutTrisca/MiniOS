/* TRISCA Ionut - 312CB */

#include "header.h"

void * InitQ(size_t d)
	/*-- Initializare Coada --*/
{
	TCoada * c;
	c = (TCoada *)malloc(sizeof(TCoada));

	if(!c)
	{
		return NULL;
	}

	c->dime = d;
	c->ic = NULL;
	c->sc = NULL;

	return (void *)c;
}

int IntrQ(void * c, void * ae)
	/*-- Introducere Element in Coada --*/
{
	TLG aux;
	aux = AlocCelulaG(ae, ((TCoada *)c)->dime);

	if(!aux)
	{
		return 0;
	}

	if(((TCoada *)c)->sc)
	{
		((TCoada *)c)->sc->urm = aux;
		((TCoada *)c)->sc = aux;
	}
	else
	{
		((TCoada *)c)->sc = aux;
	}

	if(!((TCoada *)c)->ic)
	{
		((TCoada *)c)->ic = aux;
	}

	return 1;
}

int ExtrQ(void * c, void * ae)
	/*-- Extragere element din coada --*/
{
	if(!((TCoada *)c)->ic)
	{
		return 0;
	}

	TLG aux;
	aux = ((TCoada *)c)->ic;

	memcpy(ae, ((TCoada *)c)->ic->info, ((TCoada *)c)->dime);

	((TCoada *)c)->ic = aux->urm;

	if(!((TCoada *)c)->ic)
		((TCoada *)c)->sc = NULL;

	free(aux->info);
	free(aux);
	return 1;
}

void IntrSort(void * c, void * ae)
	/*-- Introducere sortata a unui element in coada --*/
{
	void * caux = InitQ(sizeof(TProces));
	TProces proc, elem = (*((TProces *)ae)), 
			pr2 = *((TProces*)ae), pr3 = *((TProces*)ae);

	int i = ExtrQ(c, &proc), pid = 0, introd = 0, pr2ck = 0;

	do
	{
		/*-- Verificare coada vida --*/
		if(i == 0) 
		{
			IntrQ(caux, ae);
			introd = 1;
			break;
		}

		/*-- Verificare conditii ale sortarii -*/
		if(elem.prioritate >= proc.prioritate)
			if(elem.prioritate == proc.prioritate)
				if(elem.timp_exec_ramas == proc.timp_exec_ramas)
				{
					if(elem.PID < proc.PID)
					{
						IntrQ(caux, ae);
						IntrQ(caux, &proc);
						introd = 1;
						break;
					}
					else
					{
						IntrQ(caux, &proc);
						pid = 1;
						pr3 = proc;
						continue;
					}
				}
				else
				{
					if(elem.timp_exec_ramas < proc.timp_exec_ramas)
					{
						IntrQ(caux, ae);
						IntrQ(caux, &proc);
						introd = 1;
						break;
					}
					else
					{
						IntrQ(caux, &proc);
						pr2ck = 1;
						continue;
					}
				}
			else
			{
				if(elem.prioritate > proc.prioritate)
				{
					IntrQ(caux, ae);
					IntrQ(caux, &proc);
					introd = 1;
					break;
				}
				else
				{
					IntrQ(caux, &proc);
					IntrQ(caux, ae);
					introd = 1;
					break;
				}
			}
		else
			IntrQ(caux, &proc);

	} while(ExtrQ(c, &proc));

	/*-- Verficare daca elementul este neintrodus deoarece este ultimul --*/
	if(introd == 0)
	{
		if(elem.prioritate == pr3.prioritate &&
			elem.timp_exec_ramas == pr3.timp_exec_ramas && pid == 1)
		{
			IntrQ(caux, ae);
			pr2ck = 0;
			introd = 1;
		}

		if(elem.prioritate == pr2.prioritate &&
			elem.timp_exec_ramas == pr2.timp_exec_ramas && pr2ck == 1)
		{
			IntrQ(caux, ae);
			introd = 1;
		}

		if(introd == 0)
			IntrQ(caux, ae);
	}
	
	/*-- Adaugarea restului cozii --*/
	while(ExtrQ(c, &proc))
		IntrQ(caux, &proc);

	while(ExtrQ(caux, &proc))
		IntrQ(c, &proc);

	DistrugeQ(&caux);
}

void DistrugeQ(void ** c)
	/*-- Distrugere Coada --*/
{
	TProces ae;

	while(ExtrQ(*c, &ae))
	{
		DistrugeS(&(ae.stiva_mem));
	}

	free(*c);
}