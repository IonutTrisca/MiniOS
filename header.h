/* TRISCA Ionut - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 200
#define SEP " "

#ifndef _TEMA_2_SD_
#define _TEMA_2_SD_

/*-- Structura celulli unei liste simplu inlatuite generice --*/
typedef struct celulag
{
	void * info;
	struct celulag * urm;
} TCelulaG, *TLG, **ALG;

/*-- Structura celulli unei liste dublu inlatuite generice --*/
typedef struct celuladg
{
	void * info;
	struct celuladg * urm, * ant;
} TCelulaDG, *TLDG, **ALDG;

/*-- Structura unei stive --*/
typedef struct 
{
	size_t dime;
	TLG ref;
} TStiva;

/*-- Structura unei cozi --*/
typedef struct 
{
	size_t dime;
	TLG ic, sc;
} TCoada;

/*-- Structura unui proces --*/
typedef struct 
{
	short PID; // identificator proces
	char prioritate; // prioritatea procesului
	unsigned int timp_exec; // timpul de executie necesar
	int timp_exec_ramas; // timpul de executie ramas
	int inc_mem; // adresa de inceput a memoriei
	void * stiva_mem; // stiva de memorie a procesului 
	int mem_ocupata; // cantitatea ocupata din stiva
	int timp_rulat; // timpul rulat pe procesor
	int dim_mem; // dimensiunea totala a memoriei
} TProces;

/*-- Structura memoriei --*/
typedef struct 
{
	TProces Idle; // procesul Idle
	TLDG Procese; // lista de procese din memorie
} TMem;


/*-- Functii Liste --*/
TLG AlocCelulaG(void *, size_t);
TLDG AlocCelulaDG(void * ax, size_t d);
int cmp(void *, void *);
void ElimElDG(ALDG, void *, void (* ElibEl)(void *),
				int (* cmp)(void *, void *));
int IntrDupaRef(ALDG, void *, void *, int (* cmp)(void *, void *));
int IntrInainteRef(ALDG, void *, void *, int (* cmp)(void *, void *));
void DistrugeLDG(ALDG, void (* ElibEl)(void *));
void ElibElV(void *);

/*-- Functii Stiva --*/
void * InitS(size_t);
int Push(void *, void *);
int Pop(void *, void *);
void DistrugeS(void **);

/*-- Functii Coada --*/
void * InitQ(size_t);
int IntrQ(void *, void *);
int ExtrQ(void *, void *);
void IntrSort(void *, void *);
void DistrugeQ(void **);

/*-- Functii Prelucrare Procese--*/
void CitireRulareComenzi(FILE *, FILE *);
void Add(FILE *, TMem *, char *, void *, void *);
void Defrag(TMem *);
void PrintWaiting(FILE *, void *);
void PrintFinished(FILE *, void *);
void Get(FILE *, char *, void *, void *, void *);
void PushP(FILE *, char *, void *, void *);
void PopP(FILE *, char *, void *, void *);
void PrintStack(FILE *, char *, void *, void *);
void Run(int, TMem *, char *, void *, void *, void *);
void Finish(FILE *, void *, void *, void *);

/*-- Functii Auxiliare --*/
int DetPIDProc(TMem *);

#endif