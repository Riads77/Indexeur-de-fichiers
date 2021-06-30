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

void sommeTab(int A[],int B[], int Resultat[],int taille){
	int i;
	for(i=0; i<taille;i++){
		Resultat[i] = A[i] + B[i];
	}
}

int main(void){
	int A[5],B[5],Resultat[5];

	initAlea(A,5,50);
	initAlea(B,5,50);

	sommeTab(A, B, Resultat, 5);

	printf(" A : ");
	affichage_tableau(A, 5);
	printf("+B : ");
	affichage_tableau(B, 5);
	printf("=Resultat : ");
	affichage_tableau(Resultat, 5);


	return 0;
}