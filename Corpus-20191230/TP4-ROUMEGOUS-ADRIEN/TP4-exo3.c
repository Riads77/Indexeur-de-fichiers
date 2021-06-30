#include <stdio.h>
#include <stdlib.h>


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


void Histogramme(int Note[], int Histo[]){
	int i,p;
	for(i=0;i<10;i++){
		p = Note[i];
		Histo[p] += 1;
	}
}


int main(void){
	int Histo[21] = {0}; /*Ou init_histogramme(histo,21) et remplir avec 0.*/
	int Note[10];
	initAlea(Note,10,20);

	Histogramme(Note,Histo);

	affichage_tableau(Note, 10);
	affichage_tableau(Histo, 21);

	return 0;
}