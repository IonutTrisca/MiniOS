/* TRISCA Ionut - 312CB */

#include "header.h"

void Add(FILE * out_file, TMem * Memorie, char * cmd, 
		void * CoadaAstept, void * CoadaRulare)
	/*-- Adauga un proces in memorie --*/
{
	TProces proc;
	char * parte_comanda;
	int flag = 0;

	/*-- Setare parametri proces --*/
	proc.mem_ocupata = 0;
	proc.timp_rulat = 0;
	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	proc.dim_mem = atoi(parte_comanda);

	parte_comanda = strtok(NULL, SEP);
	proc.timp_exec = atoi(parte_comanda);
	proc.timp_exec_ramas = proc.timp_exec;

	parte_comanda = strtok(NULL, SEP);
	proc.prioritate = atoi(parte_comanda);

	proc.stiva_mem = NULL;

	/*-- Verificare memorie goala --*/
	if(Memorie->Procese)
	{

		/*-- Determinare PID --*/
		proc.PID = DetPIDProc(Memorie);

		TLDG aux = Memorie->Procese;

		/*-- Verficare daca adresa 0 a memoriei este ocupata --*/
		if(((TProces *)(aux->info))->inc_mem > 0 && aux->ant == NULL)
			if(((TProces *)(aux->info))->inc_mem >= proc.dim_mem)
			{
				proc.inc_mem = 0;

				/*-- Introducere proces in memorie --*/
				Memorie->Idle.dim_mem -= proc.dim_mem;
				IntrInainteRef(&(Memorie->Procese), aux->info, &proc, cmp);

				void * s = InitS(sizeof(int));
				proc.stiva_mem = s;
				fprintf(out_file, "Process created successfully: PID: %i, ",
						proc.PID);
				fprintf(out_file, "Memory starts at 0x%x.\n", proc.inc_mem);

				/*-- Introducere proces in coada de asteptare --*/
				IntrSort(CoadaAstept, &proc);

				flag = 1;
			}

		/*-- Verificare spatiu liber in memorie aflat intre procese --*/
		while(aux->urm && flag == 0)
		{

			if(((TProces *)(aux->urm->info))->inc_mem - 
				((TProces *)(aux->info))->inc_mem - 
				((TProces *)(aux->info))->dim_mem >= proc.dim_mem)
			{
				proc.inc_mem = ((TProces *)(aux->info))->inc_mem + 
								((TProces *)(aux->info))->dim_mem;

				Memorie->Idle.dim_mem -= proc.dim_mem;

				/*-- Introducere proces in memorie --*/
				IntrDupaRef(&(Memorie->Procese), aux->info, &proc, cmp);
				void * s = InitS(sizeof(int));
				proc.stiva_mem = s;
				fprintf(out_file, "Process created successfully: PID: %i, ",
						proc.PID);
				fprintf(out_file, "Memory starts at 0x%x.\n", proc.inc_mem);

				/*-- Introducere proces in coada de asteptare --*/
				IntrSort(CoadaAstept, &proc);
				flag = 1;
			}

			aux = aux->urm;
		}

		/*-- Verificare spatiu liber dupa ultimul proces --*/
		if(!flag)
		{
			if(3145728 - ((TProces *)(aux->info))->inc_mem - 
				((TProces *)(aux->info))->dim_mem - proc.dim_mem > 0)
			{
				proc.inc_mem = (((TProces *)(aux->info))->inc_mem + 
								((TProces *)(aux->info))->dim_mem);
				Memorie->Idle.dim_mem -= proc.dim_mem;

				/*-- Introducere proces in memorie --*/
				IntrDupaRef(&(Memorie->Procese), aux->info, &proc, cmp);
				void * s = InitS(sizeof(int));
				proc.stiva_mem = s;
				fprintf(out_file, "Process created successfully: PID: %i, ",
						proc.PID);
				fprintf(out_file, "Memory starts at 0x%x.\n", proc.inc_mem);

				/*-- Introducere proces in coada de asteptare --*/
				IntrSort(CoadaAstept, &proc);
			}
			else
			{
				/*-- Defragmentare memorie --*/
				Defrag(Memorie);

				/*-- Reverificare spatiu liber dupa ultimul proces --*/
				if(Memorie->Idle.dim_mem - proc.dim_mem > 0)
				{
					proc.inc_mem = Memorie->Idle.dim_mem;
					Memorie->Idle.dim_mem -= proc.dim_mem;

					/*-- Introducere proces in memorie --*/
					IntrDupaRef(&(Memorie->Procese), aux->info, &proc, cmp);
					void * s = InitS(sizeof(int));
					proc.stiva_mem = s;
					fprintf(out_file, "Process created ");
					fprintf(out_file, "successfully: PID: %i, ", proc.PID);
					fprintf(out_file, "Memory starts at 0x%x.\n", 
							proc.inc_mem);

					/*-- Introducere proces in coada de asteptare --*/
					IntrSort(CoadaAstept, &proc);
				}
				else
				{
					fprintf(out_file, "Cannot reserve memory for PID %i.\n",
							proc.PID);
				}
			}
		}
	}
	else
	{
		/*-- Introducerea primului proces in starea running --*/
		proc.PID = 1;
		proc.inc_mem = 0;
		Memorie->Idle.dim_mem -= proc.dim_mem; 
		TLDG aux = AlocCelulaDG(&proc, sizeof(TProces));
		if(!aux)
			return;
		Memorie->Procese = aux;
		void * s = InitS(sizeof(int));
		proc.stiva_mem = s;
		fprintf(out_file, "Process created successfully: PID: %i, ",
				proc.PID);
		fprintf(out_file, "Memory starts at 0x%x.\n", proc.inc_mem);
		IntrQ(CoadaRulare, &proc);

	}
}

void PrintWaiting(FILE * out_file, void * CoadaAstept)
	/*-- Afiseaza procesele din lista de asteptare --*/
{
	fprintf(out_file, "Waiting queue:\n[");

	TProces proc;
	void * caux = InitQ(sizeof(TProces));

	while(ExtrQ(CoadaAstept, &proc))
	{
		fprintf(out_file, "(%i: priority = %i, remaining_time = %i)",
				proc.PID, proc.prioritate, proc.timp_exec_ramas);
		IntrQ(caux, &proc);
		if(((TCoada *)CoadaAstept)->ic)
		{
			fprintf(out_file, ",\n");
		}
	}

	fprintf(out_file, "]\n");

	while(ExtrQ(caux, &proc))
	{
		IntrQ(CoadaAstept, &proc);
	}

	DistrugeQ(&caux);
}

void PrintFinished(FILE * out_file, void * CoadaTerminat)
	/*-- Afiseaza procesele terminate --*/
{
	fprintf(out_file, "Finished queue:\n[");

	TProces proc;
	void * caux = InitQ(sizeof(TProces));

	while(ExtrQ(CoadaTerminat, &proc))
	{
		fprintf(out_file, "(%i: priority = %i, executed_time = %i)",
				proc.PID, proc.prioritate, proc.timp_exec);
		IntrQ(caux, &proc);
		if(((TCoada *)CoadaTerminat)->ic)
		{
			fprintf(out_file, ",\n");
		}
	}

	fprintf(out_file, "]\n");

	while(ExtrQ(caux, &proc))
	{
		IntrQ(CoadaTerminat, &proc);
	}

	DistrugeQ(&caux);
}

void Get(FILE * out_file, char * cmd, void * CoadaAstept,
		void * CoadaRulare, void * CoadaTerminat)
	/*-- Afiseaza statusul unui proces --*/
{
	char * parte_comanda;
	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	int PID = atoi(parte_comanda), flag = 0;

	void * caux = InitQ(sizeof(TProces));
	TProces ae;

	/*-- Verificare coada rulare --*/
	while(ExtrQ(CoadaRulare, &ae))
	{
		if(ae.PID == PID)
		{
			fprintf(out_file, "Process %i is running (remaining_time: %i).\n",
					ae.PID, ae.timp_exec_ramas);
			flag = 1;
		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaRulare, &ae);

	/*-- Verificare coada asteptare --*/
	if(flag == 0)
	{
		while(ExtrQ(CoadaAstept, &ae))
		{
			if(ae.PID == PID)
			{
				fprintf(out_file, "Process %i is waiting (remaining_time: %i).\n",
						ae.PID, ae.timp_exec_ramas);
				flag = 1;
			}
			IntrQ(caux, &ae);
		}

		while(ExtrQ(caux, &ae))
			IntrQ(CoadaAstept, &ae);
	}

	/*-- Verificare coada procese terminate --*/
	while(ExtrQ(CoadaTerminat, &ae))
	{
		if(ae.PID == PID)
		{
			fprintf(out_file, "Process %i is finished.\n", ae.PID);
			flag = 1;
		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaTerminat, &ae);

	DistrugeQ(&caux);

	if(flag == 0)
		fprintf(out_file, "Process %i not found.\n", PID);
}

void PushP(FILE * out_file, char * cmd, void * CoadaAstept,
		void * CoadaRulare)
	/*-- Adauga 4 octeti la stiva unui proces --*/
{
	char * parte_comanda;
	void * caux = InitQ(sizeof(TProces));
	int PID, info, flag = 0;

	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	PID = atoi(parte_comanda);

	parte_comanda = strtok(NULL, SEP);
	info = atoi(parte_comanda);

	TProces ae;

	/*-- Verificare coada asteptare --*/
	while(ExtrQ(CoadaAstept, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva plina --*/
			if(ae.mem_ocupata == ae.dim_mem)
			{
				fprintf(out_file, "Stack overflow PID %i.\n", ae.PID);
			}
			else
			{
				Push(ae.stiva_mem, &info);
				ae.mem_ocupata += 4;
			}
			flag = 1;

		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaAstept, &ae);


	if(flag) 
	{
		DistrugeQ(&caux);
		return;
	}

	/*-- Verificare coada rulare --*/
	while(ExtrQ(CoadaRulare, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva plina --*/
			if(ae.mem_ocupata == ae.dim_mem)
			{
				fprintf(out_file, "Stack overflow PID %i.\n", ae.PID);
			}
			else
			{
				Push(ae.stiva_mem, &info);
				ae.mem_ocupata += 4;
			}
			flag = 1;
		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaRulare, &ae);

	DistrugeQ(&caux);

	if(flag == 0)
		fprintf(out_file, "PID %i not found.\n", PID);
}

void PopP(FILE * out_file, char * cmd, void * CoadaAstept, void * CoadaRulare)
	/*-- Elimina 4 octeti din stiva unui proces --*/
{
	char * parte_comanda;
	void * caux = InitQ(sizeof(TProces));
	int PID, info, flag = 0;

	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	PID = atoi(parte_comanda);

	TProces ae;

	/*-- Verificare coada asteptare --*/
	while(ExtrQ(CoadaAstept, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva goala --*/
			if(ae.mem_ocupata == 0)
			{
				fprintf(out_file, "Empty stack PID %i.\n", ae.PID);
			}
			else
			{
				Pop(ae.stiva_mem, &info);
				ae.mem_ocupata -= 4;
			}
			flag = 1;

		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaAstept, &ae);


	if(flag) 
	{
		DistrugeQ(&caux);
		return;
	}

	/*-- Verificare coada rulare --*/
	while(ExtrQ(CoadaRulare, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva goala --*/
			if(ae.mem_ocupata == 0)
			{
				fprintf(out_file, "Empty stack PID %i.\n", ae.PID);
			}
			else
			{
				Pop(ae.stiva_mem, &info);
				ae.mem_ocupata -= 4;
			}
			flag = 1;
		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaRulare, &ae);

	DistrugeQ(&caux);

	if(flag == 0)
		fprintf(out_file, "PID %i not found.\n", PID);
}

void PrintStack(FILE * out_file, char * cmd, void * CoadaAstept,
				void * CoadaRulare)
	/*-- Afiseaza stiva de memorie a unui proces --*/
{
	char * parte_comanda;
	void * caux = InitQ(sizeof(TProces)), * saux = InitS(sizeof(int));
	int PID, info, flag = 0;

	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	parte_comanda = strtok(NULL, SEP);
	PID = atoi(parte_comanda);

	TProces ae;

	/*-- Verificare coada asteptare --*/
	while(ExtrQ(CoadaAstept, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva goala --*/
			if(ae.mem_ocupata == 0)
			{
				fprintf(out_file, "Empty stack PID %i.\n", ae.PID);
				DistrugeS(&saux);
			}
			else
			{
				fprintf(out_file, "Stack of PID %i:", PID);

				while(Pop(ae.stiva_mem, &info))
					Push(saux, &info);
				while(Pop(saux, &info))
				{
					fprintf(out_file, " %i", info);
					Push(ae.stiva_mem, &info);
				}
				fprintf(out_file, ".\n");
				DistrugeS(&saux);
			}
			flag = 1;

		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaAstept, &ae);

	if(flag) 
	{
		DistrugeQ(&caux);
		return;
	}

	/*-- Verificare coada rulare --*/
	while(ExtrQ(CoadaRulare, &ae))
	{
		if(ae.PID == PID)
		{
			/*-- Verificare stiva goala --*/
			if(ae.mem_ocupata == 0)
			{
				fprintf(out_file, "Empty stack PID %i.\n", ae.PID);
				DistrugeS(&saux);
			}
			else
			{
				fprintf(out_file, "Stack of PID %i:", PID);

				while(Pop(ae.stiva_mem, &info))
					Push(saux, &info);
				while(Pop(saux, &info))
				{
					fprintf(out_file, " %i", info);
					Push(ae.stiva_mem, &info);
				}
				fprintf(out_file, ".\n");
				DistrugeS(&saux);
			}
			flag = 1;
		}
		IntrQ(caux, &ae);
	}

	while(ExtrQ(caux, &ae))
		IntrQ(CoadaRulare, &ae);

	DistrugeQ(&caux);

	if(flag == 0)
	{
		DistrugeS(&saux);
		fprintf(out_file, "PID %i not found.\n", PID);
	}

}

void Defrag(TMem * Memorie)
	/*-- Defragmenteaza memoria --*/
{
	TLDG aux = Memorie->Procese;

	((TProces *)(aux->info))->inc_mem = 0;
	aux = aux->urm;
	while(aux)
	{
		((TProces *)(aux->info))->inc_mem = 
				((TProces *)(aux->ant->info))->inc_mem + 
				((TProces *)(aux->ant->info))->dim_mem;
		aux = aux->urm;
	}
}

void Run(int cuanta, TMem * Memorie, char * cmd, 
		void * CoadaAstept, void * CoadaRulare, void * CoadaTerminat)
	/*-- Ruleaza un numar de unitati pe procesor --*/
{
	int nr_unit_timp;
	char * parte_comanda;
	void * caux = InitQ(sizeof(TProces));

	parte_comanda = strtok(cmd, SEP);

	parte_comanda = strtok(NULL, SEP);
	nr_unit_timp = atoi(parte_comanda);

	TProces ae;

	while(nr_unit_timp)
	{
		/*-- Verificare coada goala --*/
		if(ExtrQ(CoadaRulare, &ae) == 0)
			if(ExtrQ(CoadaAstept, &ae) == 0)
			{
				DistrugeQ(&caux);
				return;
			}

		/*-- Rulare proces si verificare reguli impuse de rulare--*/
		while(nr_unit_timp > 0 && ae.timp_rulat < cuanta 
				&& ae.timp_exec_ramas > 0)
		{
			ae.timp_exec_ramas--;
			nr_unit_timp--;
			ae.timp_rulat++;
		}

		/*-- Introducere procese terminate in coada de procese terminate --*/
		if(ae.timp_exec_ramas == 0)
		{
			IntrQ(CoadaTerminat, &ae);
			ElimElDG(&(Memorie->Procese), &ae, ElibElV, cmp);
			Memorie->Idle.dim_mem -= ae.dim_mem;
		}
		else
		{
			/*-- Verificare daca procesul mai poate fi rulat in continuare --*/
			if(ae.timp_rulat < cuanta)
			{
				IntrQ(CoadaRulare, &ae);
			}
			else
			{

				ae.timp_rulat = 0;
				IntrQ(caux, &ae);
				if(ExtrQ(CoadaAstept, &ae) == 0)
				{
					ExtrQ(caux, &ae);
					IntrQ(CoadaRulare, &ae);
				}
				else
				{
					IntrQ(CoadaRulare, &ae);
					ExtrQ(caux, &ae);
					IntrSort(CoadaAstept, &ae);
				}
			}

		}
	}

	DistrugeQ(&caux);

}

void Finish(FILE * out_file, void * CoadaAstept,
			void * CoadaRulare, void * CoadaTerminat)
	/*-- Termina de rulat toate procesele ramase --*/
{
	TProces ae;
	int timp_total = 0;

	/*-- Verificare procese ramase in coada de rulare --*/
	if(ExtrQ(CoadaRulare, &ae))
	{
		timp_total += ae.timp_exec_ramas;
		ae.timp_exec_ramas = 0;
		IntrQ(CoadaTerminat, &ae);
	}

	while(ExtrQ(CoadaAstept, &ae))
	{
		timp_total += ae.timp_exec_ramas;
		ae.timp_exec_ramas = 0;
		IntrQ(CoadaTerminat, &ae);
	}

	fprintf(out_file, "Total time: %i\n", timp_total);
}