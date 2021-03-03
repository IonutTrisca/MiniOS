/* TRISCA Ionut - 312CB */

#include "header.h"

void CitireRulareComenzi(FILE * in_file, FILE * out_file)
	/*-- Citeste comenzile din fisierul de intrare si apeleaza functiile --*/
{
	/*-- Initializare Memorie --*/
	TMem * Memorie = malloc(sizeof(TMem));
	Memorie->Idle.PID = 0;
	Memorie->Idle.stiva_mem = NULL;
	Memorie->Idle.timp_exec_ramas = 0;
	Memorie->Idle.mem_ocupata = 0;
	Memorie->Idle.dim_mem = 3145728; // reprezinta memoria libera
	Memorie->Procese = NULL;

	/*-- Initializare Cozi Asteptare, Terminat, Rulare --*/
	void * CoadaAstept = InitQ(sizeof(TProces));
	void * CoadaTerminat = InitQ(sizeof(TProces));
	void * CoadaRulare = InitQ(sizeof(TProces));

	/*-- Preluare Informatii --*/
	int cuanta = 0;

	if(fscanf(in_file, "%i", &cuanta) == 0)
		return;

	char * aux = malloc(MAX), * nume_cmd, * cmd = malloc(MAX);

	if(fscanf(in_file, "\n") == 1)
		return;

	/*-- Apelare functii --*/
	while(fgets(aux, MAX, in_file))
	{
		/*-- Eliminare \n datorat functiei fgets --*/
		aux[strlen(aux) - 1] = '\0';

		strcpy(cmd, aux);
		nume_cmd = strtok(aux, SEP);

		/*-- Verificare nume comanda si apelare functie --*/
		if(!strcmp(nume_cmd, "add"))
			Add(out_file, Memorie, cmd, CoadaAstept, CoadaRulare);

		if(!strcmp(nume_cmd, "get"))
			Get(out_file, cmd, CoadaAstept, CoadaRulare, CoadaTerminat);

		if(!strcmp(nume_cmd, "push"))
			PushP(out_file, cmd, CoadaAstept, CoadaRulare);

		if(!strcmp(nume_cmd, "pop"))
			PopP(out_file, cmd, CoadaAstept, CoadaRulare);

		if(!strcmp(nume_cmd, "print"))
		{
			nume_cmd = strtok(NULL, SEP);

			if(nume_cmd[0] == 's')
				PrintStack(out_file, cmd, CoadaAstept, CoadaRulare);

			if(nume_cmd[0] == 'w')
				PrintWaiting(out_file, CoadaAstept);

			if(nume_cmd[0] == 'f')
			{
				PrintFinished(out_file, CoadaTerminat);
				continue;
			}
		}

		if(!strcmp(nume_cmd, "run"))
			Run(cuanta, Memorie, cmd,
				CoadaAstept, CoadaRulare, CoadaTerminat);

		if(nume_cmd[0] == 'f')
			Finish(out_file, CoadaAstept, CoadaRulare, CoadaTerminat);
	}

	/*-- Eliberare stringuri si cozi alocate si memoria --*/
	free(aux);
	free(cmd);
	DistrugeQ(&CoadaTerminat);
	DistrugeQ(&CoadaAstept);
	DistrugeQ(&CoadaRulare);
	DistrugeLDG(&(Memorie->Procese), ElibElV);
	free(Memorie);
}