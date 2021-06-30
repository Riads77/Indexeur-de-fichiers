#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void initAlea(int t[], int taille, int max){
	int i;
	for(i=0;i<taille;i++){
		t[i] = random()%max;
	}
}

void affichage_tableau(int t[], int taille){
	int i;
	for (i=0; i<taille;i++){
		printf("%d ",t[i]);
	}
	printf("\n");
}

int main(void){
	int i,aleatoire,tableau[20],taille,max;

	printf("Exercice a\n");

	for(i=0;i<20;i++){
		aleatoire = random();
		printf("Le nombre aleatoire %d est : %d\n",i,aleatoire);
	}
	printf("Exercice b\n");

	printf("Choisir taille\n");
	scanf(" %d",&taille);

	printf("Choisir rand_max\n");
	scanf(" %d",&max);

	initAlea(tableau,taille,max);
	affichage_tableau(tableau, taille);


	

	return 0;
}