	Elemente de graficã pe calculator - Tema 3
******************************************************************************************

	Ski Jumping
	Despa Diana Alexandra 331CA

******************************************************************************************

	Cuprins
1. Cerinta
2. Utilizare
3. Implementare
4. Testare
5. Probleme Aparute
6. Continutul Arhivei
7. Functionalitati
8. Mentiuni

******************************************************************************************

1. Cerinta
	Tema presupune implementarea unei scene 3D pentru a modela sariturile unor
personaje de pe trambulina.

******************************************************************************************

2. Utilizare
	Programul se ruleaza prin lansarea executabilului Tema3.exe, din folderul Debug,
sau din interiorul Visual Studio.
	
Taste:
	Esc - inchidere program
	SPACE - reincarca shaderul si recalculeaza locatiile
	A - plaseaza pe quad personajul principal
	E - declanseaza cutremur
	R - opreste cutremur
	C - comuta intre camere
	W - desenare wireframe
	S - desenare solid
	T - muta ultima camera inainte
	Y - muta ultima camera inapoi
	U - muta ultima camera in jos
	I - muta ultima camera in sus
	O - muta ultima camera la stanga
	P - muta ultima camera la dreapta
	1 - roteste ultima camera FPS in sens antitrigonometric
	2 - roteste ultima camera FPS in sens trigonometric
	3 - roteste ultima camera FPS stanga
	4 - roteste ultima camera FPS dreapta
	5 - roteste ultima camera FPS inainte
	6 - roteste ultima camera FPS inapoi
	Q - schimba modul special cu modul normal si invers
	F1 - activeaza full screen
	F2 - dezactiveaza full screen
	Sageata stanga - roteste personaj principal stanga
	Sageata dreapta - roteste personaj principal dreapta
	Sageata sus - muta personaj principal inaite
	Sageata jos - muta personaj principal inapoi(doar pe quad)

******************************************************************************************

3. Implementare
	Ca IDE, am folosit Microsoft Visual Studio 2013, instalat pe Windows 7, insa am
implementat aplicatia folosind Visual C++ 2010, ca sa ma asigur ca functioneaza cu
versiuni mai recente de Visual C++.
	Am pornit cu implementarea de la framework-ul laboratorului 6 si 7.
	Obiectele ce compun scena sunt construite folosind VAO, VBO si IBO.
	Pentru incarcarea tuturor obiectelor folosite pentru personaje folosesc clasa
ObjectBuilder, care retine intr-o lista de obiecte (de tip Object) cele 4 modele de
personaje. Ele sunt incarcate de la inceputul executiei, pentru a nu incetini executia
pe parcurs.
	Personajele se deplaseaza pe trambuline conform enuntului. In cazul personajului
principal activ, am abordat coliziunea cu parapetii rampei astfel: atunci cand schiurile
lui ating parapetul, el continua sa inainteze, dar unghiul cu care este rotit pe rampa
este readus catre 0, pana cand devine paralel cu parapetul sau este rotit in directia
opusa parapetului. In aer, personajele se deplaseaza pe o traiectorie sub forma de
parabola. In timpul deplasarii in aer, personajele si skiurile se rotest in jurul axei
Ox, pentru a da impresia de efect aerodinamic. Deplasarea pe quad se face in limitele
acestuia si cu evitarea suprapunerii personajelor.
	Pentru plasarea personajelor secundare pe quad am implementat un algoritm BFS
care gaseste calea minima pana la o pozitie libera si foloseste o matrice pentru a tine
cont de pozitiile ocupate de alte personaje sau obiecte.
	Efectul de cutremur si schimbarea culorii personajului principal activ sunt
realizate in vertex shader, prin transmiterea de variabile uniforme care permit sau nu
aplicarea transformarilor corespunzatoare asupra vertecsilor, in functie de obiectul care
urmeaza a fi desenat. In timpul unui cutremur, variabila x_quake este mai mare decat -1,
doar pentru personajul principal activ si pentru rampa pe care se afla el, asta permitand
modificarea pozitiei vertecsilor cu un offset dat de o miscare ondulatorie, generata
random pe baza de frecventa si amplitudine. Modificarea culorii se face doar asupra
personajului principal, conform enuntului si in functie de timpul decurs de la inceperea
deplasarii pe rampa pana la schimbarea starii.
	Camerele sunt implementate conform enuntului. Mentionez ca, camerele TPS
corespunzatoare fiecarui personaj activ raman in partea de dreapta sus a personajului
respectiv pe tot parcursul deplasarii lui. Retin camerele intr-un vector de pointeri in
main, pentru a comuta usor intre ele. Camerele FPS si TPS asociate fiecarui personaj
activ sunt actualizate in clasa ce reprezinta un personaj (Character).
	Iluminarea scenei o realizez cu modelul Phong, calculand normalele la fiecare
vertex in parte in functie de normalele la fetele adiacente vertexului respectiv.

******************************************************************************************

4. Testare
	Am testat diferite scenarii, mutand personajul principal, camerele, modificand
parametrii transmisi shaderelor. Nu am utilizat teste automate. Am utilizat debugger-ul
din Visual Studio.

******************************************************************************************

5. Probleme aparute
	Pentru a face posibila luminarea obiectelor din scena a trebuit sa calculez
"de mana" normalele la vertecsi. Nu stiam asta cand am inceput implementarea, dar sper ca
am abordat corect problema.
	Obiectul pe care il folosesc ca personaj secundar in modul special este mai mare
decat unitatea pentru celelalte obiecte, deci a trebui sa introduc o matrice de scalare
in vertex shader si sa il scalez la dimensiunile corespunzatoare.
	A fost problematica conversia de la radiani la grade si invers, deoarece facea
ca deplasarea in panta sa se realizeze cu erori de pozitie. Am rezolvat problema prin
resetarea matricii de modelare, urmata de translatia obiectului respectiv si rotirea lui
la unghiul dorit - o singura rotatie nu implica o eroare mare.

******************************************************************************************

6. Continutul Arhivei
	main.cpp - sursa principala
	Character.cpp
	Character.h
	CharacterType.h
	MyVertexFormat.h
	Object.cpp
	Object.h
	ObjectBuilder.cpp
	ObjectBuilder.h
	Quad.cpp
	Quad.h
	Slope.cpp
	Slope.h
	Util.h
	Framework-ul de la laborator.
	README - acest fisier.

******************************************************************************************

7. Functionalitati
	Functionalitati Standard
		Desenarea obiectelor scenei
		Deplasarea personajului principal activ in scena
		Deplasarea personajului secundar activ in scena
		Camere
		Efect de cutremur
		Schimbarea culorii personajului principal activ

	Functionalitati Bonus
		Personajele au skiuri
		Efect aerodinamic la sarituri
		Traiectorie sub forma de parabola in aer
		Rampa asociata personajului principal are parapet

	Functionalitati Suplimentare
		Scena luminata cu modelul Phong
		Personaje speciale
		BFS + gasirea caii minime pentru personajele secundare
		
******************************************************************************************

8. Mentiuni
	Majoritatea comentariilor din cod, in special cele care descriu metode si membri
ale claselor implementate se afla in fisierele de tip header.
	Am descarcat fisiere .obj folosite de pe site-ul http://tf3dm.com/