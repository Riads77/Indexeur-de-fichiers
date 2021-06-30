#include <stdio.h>
#include <stdlib.h>

int position(int t[],int taille, int tx){
		int i;
		for(i=0; i<taille;i++){
			if (tx == t[i]){
				return i;
			}
		}
		return -1;
}


int main(void){
	int taille=5,tx;
	int t[5] = {0,3,8,12,-4};
	printf("Entier recherché :\n");
	scanf("%d",&tx);
	printf("%d\n",position(t,taille,tx));
	return 0;
}