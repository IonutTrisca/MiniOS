/* TRISCA Ionut - 312CB */

#include "header.h"

int VerifProcExist(TMem * Memorie, int PID)
	/*-- Verifica existenta unui proces in memorie--*/ 
{
	TLDG aux = Memorie->Procese;
	while(aux)
	{
		if(((TProces*)(aux->info))->PID == PID)
			return 0;
		aux = aux->urm;
	}
	return 1;
}

int DetPIDProc(TMem * Memorie)
	/*-- Stabileste PID-ul procesului --*/
{
	int i;
	for(i = 1; i < 32768; i++)
	{
		if(VerifProcExist(Memorie, i))
		{
			return i;
		}
	}
	return -1;
}

