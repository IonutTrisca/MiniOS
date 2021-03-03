/* TRISCA Ionut - 312CB */

#include "header.h"

int main(int argc, char** argv)
	/*-- Deschidere fisiere intrare si iesire, apelare functie de citire --*/
{
	/*-- Verificare numar de argumente --*/
	if(argc != 3)
	{
		printf("Acest program accepta doar doua argumente!\n");
		return -1;
	}

	char * in = malloc(30), * out = malloc(30);

	strcpy(in, argv[1]);
	strcpy(out, argv[2]);
	
	FILE * in_file = fopen(in, "rt");
	FILE * out_file = fopen(out, "wt");

	/*-- Verificare existenta fisier de intrare --*/
	if(!in_file)
	{
		printf("Nu pot accesa fisierul de intrare!\n");
		fclose(in_file);
		fclose(out_file);
		return -1;
	}

	CitireRulareComenzi(in_file, out_file);

	/*-- Inchiderea fisierelor si eliberarea stringurilor --*/
	fclose(in_file);
	fclose(out_file);

	free(in);
	free(out);

	return 0;
}