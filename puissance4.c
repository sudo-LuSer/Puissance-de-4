#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "ai_player.h"
#include "Puissance4.h"
char NomJoueur1[30];
char NomJoueur2[30];
char JoueurEnCours; 
char GrilleDeJeu[42]; 
char Joueur2Virtuel = '0';
float priority[42];
void init(){
	printf("\x1B[2J");
	for(int i = 0 ; i < 42 ;i++){
		GrilleDeJeu[i] = '0';
	}
	printf("entrer le nom du \x1B[93m joueur 1 : \x1B[0m ");
	scanf("%s" , &NomJoueur1);
	printf("entrer le nom du \x1B[91m joueur 2 : \x1B[0m ");
	int c;
    while ((c = getchar()) != '\n' && c != EOF);
    fgets(NomJoueur2, 30, stdin);
    if (strcmp("", NomJoueur2) == 0 || NomJoueur2[0] == '\n') {
        Joueur2Virtuel = '1';
    }
	if(Joueur2Virtuel=='0'){
		JoueurEnCours = '1' + (rand()%2);
	}
	else {
		JoueurEnCours = '1'; 
	}
}

void display(){
	printf("\x1B[93m joueur 1 : %s\n" , NomJoueur1);
	if(Joueur2Virtuel =='0'){
		printf("\x1B[91m joueur 2 : %s\n\n" ,NomJoueur2);
	}
	else{
		printf("\x1B[91m joueur 2 : COMPUTER\n\n");
	}
	printf("  \x1B[0m");
	for(int i = 0 ; i < 7 ;i++){
		printf("%d " , i+1);
	}
	for(int i = 0 ; i < 42 ;i++){
		if(i % 7 == 0){
			printf("\n");
			int r = floor(i/7) +1;
			printf("%d " , r);
		}
		if(GrilleDeJeu[i]=='0'){
			printf("%c " , '.');
		}
		else if(GrilleDeJeu[i]=='1'){
			printf("\x1B[93m%c " , 'X');
			printf("\x1B[0m");
		}
		else if(GrilleDeJeu[i]=='2'){
			printf("\x1B[91m%c " , 'O');
			printf("\x1B[0m");
		}
		else{
			printf("\x1B[1m%c " , '?');
			printf("\x1B[0m");
		}
	}
	printf("\n");
	printf("le tour du joueur %c :\n" , JoueurEnCours);
}

void colonne_de_joueur(){
	int r = 6 , c; 
	printf("placer le jeton sur le colonne : ");
	scanf("%d" , &c);
	if(c<1 || c>7){
		printf("\e[0;91mc'est dehors la grille de jeu \n");
		printf("\x1B[0m");
		colonne_de_joueur();
	}
	c--;
	int p = 0 ;
	while(r--){	
		if(GrilleDeJeu[r*7 +c]=='0'){
			GrilleDeJeu[r*7 + c] = JoueurEnCours;
			if(JoueurEnCours=='1' && Joueur2Virtuel=='0'){
				JoueurEnCours = '2'; 
			}
			else if(JoueurEnCours=='2'){
				JoueurEnCours = '1';
			}
			p = 1; 
			break; 
		}
	}
	if(!p){
		printf("\e[0;91m cette case est deja reservé essayer par autre colonne.\n");
		printf("\x1B[0m");
		colonne_de_joueur();
	}
}

int detecte_plein(){
	for(int i = 0 ; i < 42 ; i++){
		if(GrilleDeJeu[i]=='0'){
			return 0;
		}
	}
	JoueurEnCours = 10;
	return 1 ;  
}

char TestLigne(char *grille){
	int r = 6; 
	while(r--){
		int c = 0 ;
		while(c+3 < 7){
			if(grille[r*7 + c] == '0'){
				c++; 
				continue; 
			}
			else if((grille[r*7 + c]==grille[r*7 + (c+1)]) && (grille[r*7 + (c+1)]==grille[r*7 + (c+2)]) && (grille[r*7 + (c+2)]==grille[r*7 +(c+3)])){
				return grille[r*7 +c];
			}
			c++; 
		}
	}
	return '0';
}

char TestColonne(char *grille){
	int c = 7; 
	while(c--){
		int r = 0 ;
		while(r+3 < 6){
			if(grille[r*7 + c] == '0'){
				r++; 
				continue; 
			}
			else if((grille[r*7 + c]==grille[(r+1)*7 + c]) && (grille[(r+1)*7 + c]==grille[(r+2)*7 + c]) && (grille[(r+2)*7 + c]==grille[(r+3)*7 + c])){
				return grille[r*7 +c];
			}
			r++; 
		}
	}
	return '0';
}

char TestDiagNeg(char *grille){
	/* l'idée est de vérifier pour la sous matrice A[0->2][0->3] :) */
	for(int r = 0 ; r < 3 ; r++){
		for(int c = 0 ; c < 4 ; c++){
			if(grille[r*7 + c]=='0'){
				continue; 
			}
			if(grille[r*7 + c]==grille[(r+1)*7 + (c+1)] && grille[(r+1)*7 + (c+1)]==grille[(r+2)*7 + (c+2)] && grille[(r+2)*7 + (c+2)]==grille[(r+3)*7 + (c+3)]){
				return grille[r*7 + c]; 
			}
		}
	} 
	return '0';
}

char TestDiagPos(char *grille){
	/* l'idée est de vérifier pour la sous matrice A[3->5][0->3] :) */
	for(int r = 3 ; r < 6 ; r++){
		for(int c = 0 ; c < 4 ; c++){
			if(grille[r*7 + c]=='0'){
				continue; 
			}
			if(grille[r*7 + c]==grille[(r-1)*7 + (c+1)] && grille[(r-1)*7 + (c+1)]==grille[(r-2)*7 + (c+2)] && grille[(r-2)*7 + (c+2)]==grille[(r-3)*7 + (c+3)]){
				return grille[r*7 + c]; 
			}
		}
	} 
	return '0';
}

int TestVainqueur(char *grille){
	if(TestLigne(grille)!='0'){
		printf("\x1B[2J");
		display();
		if(TestLigne(grille)=='1'){
			printf("\x1B[32m le joueur \x1B[93m %c \x1B[32m a gagné \n" , TestLigne(grille));
			printf("\x1B[0m");
			JoueurEnCours = 11; 
			return 1; 
		}
		else if(Joueur2Virtuel!='0'){
			printf("\x1B[32m l'ordi a gagné \n");
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		else{
			printf("\x1B[32m le joueur \x1B[91m %c \x1B[32m a gagné \n" , TestLigne(grille));
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		return 2; 
	}
	if(TestDiagNeg(grille)!='0'){
		printf("\x1B[2J");
		display();
		if(TestDiagNeg(grille)=='1'){
			printf("\x1B[32m le joueur \x1B[93m %c \x1B[32m a gagné \n" , TestDiagNeg(grille));
			printf("\x1B[0m");
			JoueurEnCours = 11; 
			return 1; 
		}
		else if(Joueur2Virtuel!='0'){
			printf("\x1B[32m l'ordi a gagné \n");
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		else{
			printf("\x1B[32m le joueur \x1B[91m %c \x1B[32m a gagné \n" , TestDiagNeg(grille));
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		return 2; 
	}
	if(TestDiagPos(grille)!='0'){
		printf("\x1B[2J");
		display();
		if(TestDiagPos(grille)=='1'){
			printf("\x1B[32m le joueur \x1B[93m %c \x1B[32m a gagné \n" , TestDiagPos(grille));
			printf("\x1B[0m");
			JoueurEnCours = 11;
			return 1;  
		}
		else if(Joueur2Virtuel!='0'){
			printf("\x1B[32m l'ordi a gagné \n");
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		else{
			printf("\x1B[32m le joueur \x1B[91m %c \x1B[32m a gagné \n" , TestDiagPos(grille));
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		return 2; 
	}
	if(TestColonne(grille)!='0'){
		printf("\x1B[2J");
		display();
		if(TestColonne(grille)=='1'){
			printf("\x1B[32m le joueur \x1B[93m %c \x1B[32m a gagné \n" , TestColonne(grille));
			printf("\x1B[0m");
			JoueurEnCours = 11;
			return 1;  
		}
		else if(Joueur2Virtuel!='0'){
			printf("\x1B[32m l'ordi a gagné \n");
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		else{
			printf("\x1B[32m le joueur \x1B[91m %c \x1B[32m a gagné \n" , TestColonne(grille));
			printf("\x1B[0m");
			JoueurEnCours = 12; 
		}
		return 2; 
	}
	return 0;
}

void CalculeColonneAleatoire(){
	int r = 6 , c = rand()%7; 
	int p = 0 ;
	while(r--){	
		if(GrilleDeJeu[r*7 +c]=='0'){
			GrilleDeJeu[r*7 + c] = '2';
			p = 1; 
			break; 
		}
	}
	if(!p){
		colonne_de_joueur();
	}
}

int main(){

	FILE *myFile;
    myFile = fopen("priority.txt", "r");

    if(myFile == NULL){
        printf("Error Reading File\n");
        exit (0);
    }

    for (int i = 0; i < 42; i++){
        fscanf(myFile, "%f,", &priority[i] );
    }
    fclose(myFile);  


	srand(time(NULL)); 
	clock_t start = clock();
	init();
	
	while(JoueurEnCours=='1' || JoueurEnCours=='2'){
		printf("\x1B[2J");
		display();
		colonne_de_joueur();	
		if(Joueur2Virtuel == '1'){
			display();
			sleep(1);
			CalculeColonneAleatoire();	
		}
		detecte_plein();
		TestVainqueur(GrilleDeJeu);
	}

	if(JoueurEnCours==10){
		printf("\x1B[1m Égalité :/");
	}

	myFile = fopen("priority.txt", "w+");
	
	for (int i = 0; i < 42; i++){
    	fprintf(myFile, "%f ", priority[i]);
	}

	clock_t stop = clock();
    double total_t = ((double)stop - start) / CLOCKS_PER_SEC;
    printf("Total time taken by the game: %f seconds \n", total_t);
	return 0; 
}