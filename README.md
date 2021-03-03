# MiniOS

This program emulates the main functionalitites of an operating system such as Memory Defragmentation and Process Scheduling.

## Program description in Romanian
Programul are urmatoarele functionalitati principale:

"Add" - adauga un proces in memorie. 
	Aceasta parcurge structura memoriei pentru a verifica pentru prima locatie
libera cu marimea necesara procesului de adaugat, dupa care il introduce,
atribuindu-i parametrii din structura comenzii.  
	De asemea procesul este introdus in coada de rulare, daca este primul
proces, respectiv in coada de asteptare daca nu este.  
	Verificarea se face in felul urmator:  
	- se verifica daca exista alte procese in memorie, daca nu, acesta este
	introdus la adresa 0, si este pus in coada de rulare  
	- daca exista procese, se verifica daca adresa 0 a memoriei este ocupata,
	daca nu este, se verifica daca diferenta dintre inceputul memoriei primului
	proces si 0 este mai mare sau egala decat dimensiunea procesului de
	adaugat, in caz pozitiv se introduce procesul  
	- in caz negativ se trece la verificarea locului dintre procese prin
	evaluarea diferentei dintre inceputul memoriei procesului urmator,
	inceputul memoriei procesului curent si dimensiunea memoriei procesului
	curent. Daca aceasta este mai mare sau egala cu dimesiunea procesului de
	introdus, acesta este introdus  
	- in caz contrar, se verifica daca diferenta dintre dimesiunea maxima a
	memoriei, inceputul memoriei procesului si dimensiunea memoriei procesului
	este mai mare decat dimensiunea procesului de introdus, in caz afirmativ se
	introduce procesul  
	- in caz negativ se defragmenteaza memoria, dupa care se compara
	dimensiunea memoriei libere, cu dimensiunea memoriei procesului, daca este
	suficienta se introduce, daca nu se afiseaza un mesaj.  


"PrintWaiting" - parcurge coada de asteptare si afiseaza PID-ul proceselor
impreuna cu prioritatea si timpul de executie ramas.


"PrintFinished" - parcurge coada proceselor terminate si afiseaza PID-ul 
proceselor impreuna cu prioritatea si timpul executat.


"Get" - parcurge toate cozile in cazutarea procesului cu PID-ul cerut, cand
il gaseste ii afiseaza starea (running, waiting, finished).


"PushP" - parcurge cozile de rulare si asteptare in cautarea procesului cu
PID-ul cerut pentru a introduce 4 octeti in stiva sa de memorie. La gasirea
sa, verifica daca stiva este plina caz in care nu introduce nimic si
afiseaza un mesaj. Daca nu este plina va introduce cei 4 octeti.


"PopP" - asemanatoare cu PushP, doar ca elimina primii 4 octeti de pe
stiva, si daca aceasta este goala afiseaza un mesaj.


"PrintStack" - asemanatoare cu PopP, doar ca aceasta afiseaza continutul
stivei procesului, iar daca este goala afiseaza un mesaj.


"Defrag" - defragmenteaza memoria. Parcurge lista de procese. Ii seteaza
primului proces inceputul memoriei la 0, iar pentru celelalte, acesta este
setat ca suma dintre dimensiunea si inceputul memoriei procesului
anterior.


"Run" - ruleaza pe procesor un numar dat de unitati de timp. Initial
verifica daca coada de rulare este goala, echivalent cu procesorul ruleaza
procesul Idle, in caz pozitiv acesta extrage din coada de asteptare. Daca
nu sunt elemente in coada de asteptare se opreste.  
	Dupa ce a extras un proces functia ii scade din timpul
ramas si din unitatile totale de timp si ii adauga la timpul rulat, cat
timp numarul de unitati de timp este mai mare decat 0, timpul rulat al
procesului este mai mic decat cuanta si timpul executat ramas este mai mare
decat 0.   
	Dupa terminarea rularii, verifica daca mai are timp ramas pentru rulare.
Daca nu mai are il pune in coada cu procese terminate si il elimina din
memorie. Daca mai are, verifica daca a depasit cuanta maxima de unitati. 
Daca nu, acesta este introdus inapoi in coada de rulare. Daca da, este extras
urmatorul proces din coada de asteptare si procesul anterior este pus in
coada de asteptare. Daca coada de asteptare este goala, procesul este pus
inapoi in coada de rulare.  


"Finish" - trece toate procesele ramase in coada de rulare si in cea de
asteptare, in coada de procese terminate. De asemenea aduna timpul de
executat ramas pentru fiecare proces intr-o variabila pe care o afiseaza la
final.
