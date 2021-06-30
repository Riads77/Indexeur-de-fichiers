#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000

typedef struct celpos{
	int position;
	struct celpos *suivant;
}Celpos, *Listepos; /*Pour les positions des mots*/

typedef struct celmot{
	char *mot;
	Listepos positions;
}Celmot; /*Pour les mots et l'accès aux postions*/

typedef struct cellule{
	Celmot *valeur;
	struct cellule *suivant;
}Cellule, *Liste; /*Pour les listes chaînées de mots*/


unsigned int hache(char *p){
	/*Fonction de hachage donnée dans le sujet, renvoyant pour un mot donné l'indice de la liste où il sera*/
	unsigned int h = 0, g;
	for(;*p;p++){
		h = (h<<4) + *p;
		if ((g = h&0xf0000000)){ /*il y a des valeurs dans les 4 bits de poids fort*/
			h = h^(g>>24);/*elles vont influencer l'octet de poids faible*/
			h = h^g; /*on les supprime du haut de h*/
		}
	}
	return h;
}


void afficheListePos(Listepos lst){

	//Permet d'afficher la listepos.
	if(lst != NULL){
		printf("pos = %d, ", lst->position);
		afficheListePos(lst->suivant);
	}
	else
		printf("\n");
}

void afficheListe(Liste lst){

	//Permet d'afficher la liste en cas de pépin.
	if(lst != NULL){
		printf("mot = %s, ", lst->valeur->mot);
		afficheListePos(lst->valeur->positions);
		afficheListe(lst->suivant);
	}
	else
		printf("\n");
}


Listepos allouerCelPos(int val){
	/*Fonction allouant de la mémoire et une valeur val(entier) à une cellule de position Celpos*/
	Listepos tmp;
	tmp = (Celpos *)malloc(sizeof(Celpos));
	if(tmp != NULL){
		tmp->position = val;
		tmp->suivant = NULL;
	}
	return tmp;
}

Celmot* allouerCelmot(char *mot, Listepos *liste){
	/*Fonction allouant de la mémoire et des valeurs (mot, et liste de positions) à une cellule de mot Celmot*/
	Celmot *tmp = (Celmot*)malloc(sizeof(Celmot));
	tmp->mot = (char *)malloc(sizeof(strlen(mot) + 1));
	tmp->mot = (char *) mot;
	tmp->positions = (Listepos ) liste;
	return tmp;
}


Liste allouerCellule(Celmot *val){
	/*Fonction allouant de la mémoire et une valeur val (Celmot) à une cellule de liste Cellule*/
	Liste tmp;
	tmp = (Cellule *)malloc(sizeof(Cellule));
	if(tmp != NULL){
		tmp->valeur = (Celmot *) val;
		tmp->suivant = NULL;
	}
	return tmp;
}

int insererEnTetePos(Listepos* liste, int val){
	/*Fonction insérant dans une liste chaînée de position "liste" une cellule qu'on alloue avec la valeur entière val*/
	Listepos tmp;
	tmp = allouerCelPos(val);
	if(tmp != NULL){
		tmp->suivant = *liste;
		*liste = tmp;
		return 1;
	}
	return 0;
}

int insererEnTete(Liste* liste, Celmot *val){
	/*Fonction insérant dans une liste chaînée "liste" une cellule qu'on alloue avec la valeur Celmot val*/
	Liste tmp;
	tmp = allouerCellule(val);
	if(tmp != NULL){
		tmp->suivant = *liste;
		*liste = tmp;
		return 1;
	}
	return 0;
}

void libererListePos(Listepos* liste){
	/*Fonction libérant la mémoire d'une liste de position*/
	Celpos *suppr = (*liste);
	while(suppr != NULL){
		*liste = (*liste)->suivant;
		free(suppr);
		suppr = *liste;
	}
}

void libererListe(Liste* liste){
	/*Fonction libérant la mémoire d'une liste*/
	Cellule *suppr = (*liste);
	while(suppr != NULL){
		*liste = (*liste)->suivant;
		free(suppr);
		suppr = *liste;
	}
}

void libererTableau(Liste **tableauHachage){
	/*Fonction libérant la mémoire d'un tableau de liste*/
	int i;
	Liste lst;
	for(i = 0; i < N; i++ ){
		lst = *tableauHachage[i];
		libererListePos(&lst->valeur->positions);
		libererListe(&lst);
	}
}

char* lireMot(FILE *file, int position, char **s){
	/*Fonction lisant un fichier file à partir d'une certaine position et renvoyant le premier mot à partir de celle-ci*/
	fseek(file, position, SEEK_SET);
	char c;
	char mot[51];
	int nbchar;
	while(fscanf(file, "%c", &c) != EOF){
		if(c == ' ' || c == '\n' || c == '\t' || c == '.' || c == ';' || c == '!' || c == '?'){
			nbchar = strlen (mot);
			*s = (char *) malloc( nbchar + 1 );
			if (*s != NULL)
				strcpy(*s, mot);
			else
				nbchar = -1;
			break;
		}
		else{
			strncat(mot, &c, 1);
		}

	}
	return *s;
}

char* lirePhrase(FILE *file, int position, char **s){
	/*Fonction lisant un fichier file à partir d'une certaine position et renvoyant la première phrase à partir de celle-ci*/
	fseek(file, position, SEEK_SET);
	char c;
	char phrase[1024];
	int nbchar;
	while(fscanf(file, "%c", &c) != EOF){
		if(c == '.' || c == '!' || c == '?'){
			nbchar = strlen (phrase);
				*s = (char *) malloc( nbchar + 1 );
				if (*s != NULL)
					strcpy(*s, phrase);
				else
					nbchar = -1;
				break;
		}
		else{
			strncat(phrase, &c, 1);
		}

	}
	return *s;
}

int position_apres_mot(FILE *file, int position){
	/*Fonction pouvant etre remplacee par strlen, mais pouvant etre utilisee sur un fichier : prend en parametre la position au debut
	d'un mot ainsi que le fichier a parcourir et renvoie celle a la fin*/
	fseek(file, position, SEEK_SET);
	char c;
	int compteur = position;
	while(fscanf(file, "%c", &c) != EOF){
		if(c == ' ' || c == '\n' || c == '\t' || c == '.' || c == ';' || c == '!' || c == '?'){
			return compteur;
		}
		else
			compteur ++;

	}
	return compteur;
}

int position_apres_phrase(FILE *file, int position){
	/*Fonction pouvant etre remplacee par strlen, mais pouvant etre utilisee sur un fichier : prend en parametre la position au debut
	d'une phrase ainsi que le fichier a parcourir et renvoie celle a la fin*/
	fseek(file, position, SEEK_SET);
	char c;
	int compteur = position;
	while(fscanf(file, "%c", &c) != EOF){
		if(c == '.' || c == '!' || c == '?'){
			return compteur;
		}
		else
			compteur ++;

	}
	return compteur;
}



int nombreCaracteresFichiers(FILE *file){
	/*Renvoie le nombre total de caractères d'un fichier*/
	rewind(file);
	int c = 0;
	while (fgetc(file) != EOF)
		c++;
	return c;

}

int indiceHachage(char *c){
	/*Effectue la fonction hachage sur le mot c et renvoie son indice modulo N afin de rentrer le mot à coup sur dans la liste */
	int h;
	h = hache(c);
	return h % N;
}

int RecherchePos(Listepos liste, int n){
	/*Recherche récursivement une position n dans une liste de position.
	Renvoie 1 si elle y est, 0 sinon*/
	if(liste == NULL)
		return 0;
	if(liste->position == n)
		return 1;
	return RecherchePos(liste->suivant, n);
}

int Recherche(Liste liste, char *c){
	/*Recherche récursivement une chaine de caractere c dans une liste.
	Renvoie 1 si elle y est, 0 sinon*/
	if(liste == NULL)
		return 0;
	if(!strcmp(liste->valeur->mot, c))
		return 1;
	return Recherche(liste->suivant, c);
}

int RechercheIndice(Liste liste, char *c){
	/*Fonction similaire a la precedente. 
	Renvoie l'indice de la première (et normalement unique) occurence du mot en paramètre.
	Renvoie l'indice de la première cellule vide sinon*/
	if(liste == NULL)
		return 0;
	if(!strcmp(liste->valeur->mot, c))
		return 0;
	return 1 + RechercheIndice(liste->suivant, c);
}

int remplirListe(Liste *liste, int position, char *mot){
	/*Fonction utilisant les fonctions de recherche afin d'ajouter le mot si absent de la liste en paramètre ainsi que 
	la position attribuée. Sinon, la position est ajoutée dans la liste des positions de la cellule du mot, sauf si celle-ci est
	déjà existante*/
	Cellule *actuel = *liste;
	Celpos *actuelPos;
	Celmot *nouveau;
	int i, n = RechercheIndice(actuel, mot), p;

	for(i = 0; i < n; i++){
		actuel = actuel->suivant;
	}
	actuelPos = actuel->valeur->positions;
	p = RecherchePos(actuelPos, position);
	if (!p){
		while(actuelPos != NULL){
			actuelPos = actuelPos->suivant;
		}
		if(!insererEnTetePos(&actuelPos, position))
			return 0;
		if(actuel == NULL){
			nouveau = allouerCelmot(mot, &actuel->valeur->positions);
			if(!insererEnTete(&actuel, nouveau))
				return 0;
		}
	}
	return 1;
}

int remplirTableauHachage(FILE *file, Liste **tableauHachage){
	/*Fonction permettant de remplir la table de hachage à partir d'un fichier, en utilisant les fonctions précédentes.
	Le fichier sera parcouru par les fonctions de lecture de mot et de phrases (permettant d'avoir la position actuelle de chaque mot 
	ainsi que la position de début de phrase attribuée. Puis on utilise la fonction ajouteliste afin d'ajouter les données voulues dans la table.
	En cas de soucis de mémoire, la fonction renvoie 0, sinon 1.*/
	rewind(file);
	char *mot;
	int i, position = 0, pos_phrase = 0, pos_phrase_suivant = 0;
	int pos_max = nombreCaracteresFichiers(file);

	while(position < pos_max){
		mot = lireMot(file, position, &mot);
		if(strlen(mot) != 0){
			i = indiceHachage(mot);
			if(!remplirListe(tableauHachage[i], pos_phrase, mot))
				return 0;
		}

		position = position_apres_mot(file, position);
		if(pos_phrase == pos_phrase_suivant)
			pos_phrase_suivant = position_apres_phrase(file, pos_phrase);
		if(position == pos_phrase_suivant)
			pos_phrase = pos_phrase_suivant;
	}
	return 1;
}

int appartenanceMotTexte(Liste *tableauHachage, char *mot){
	/*Fonction utlisant simplement la Recherche, en retrouvant au préalable par la fonction indiceHachage
	dans quelle liste de la table pourrait se trouver le mot. Renvoie 1 si le mot appartient au texte, 0 sinon.*/
	int i = indiceHachage(mot);
	return Recherche(tableauHachage[i], mot);


}

void affichePosMot(Liste *tableauHachage, char *mot){
	/*fonction affichant la liste de positions d'un mot recherché. Renvoie une phrase
	signalant l'absence du mot dans le texte si tel est le cas*/
	int i = indiceHachage(mot);
	Cellule *actuel = tableauHachage[i];
	if(!appartenanceMotTexte(tableauHachage, mot)){
		printf("Mot n'appartenant pas au texte.\n");
		return;
	}
	while(strcmp(actuel->valeur->mot, mot))
		actuel = actuel->suivant;
	afficheListePos(actuel->valeur->positions);
}

void affichePhraseMotDonne(FILE *file, char *mot, Liste *tableauHachage){
	/*Affiche l'ensemble des phrases contenant le mot recherché. Renvoie le meme message que 
	la fonction precedente en cas d'absence du mot.*/
	int i = indiceHachage(mot);
	Cellule *actuel = tableauHachage[i];
	Celpos *actuelPos = NULL;
	char *phrase; 

	if(!appartenanceMotTexte(tableauHachage, mot))
		printf("Mot n'appartenant pas au texte.\n");

	while(strcmp(actuel->valeur->mot, mot))
		actuel = actuel->suivant;
	actuelPos = actuel->valeur->positions;
	while(actuelPos != NULL){
		phrase = lirePhrase(file, actuelPos->position, &phrase);
		printf("%s\n", phrase);
		actuelPos = actuelPos->suivant;
	}
}

int main(int argc, char *argv[]){
	/*Programme principal*/

	//Declaration des variables
	int i;
	FILE *file;
	Liste *tabH = (Cellule **)malloc(N*sizeof(Cellule*));
	if (tabH == NULL){
		printf("table non initialisée\n");
		return 0;
	}

	Cellule *actuel;

	//Initialisation des listes du tableau
	for(i = 0; i < N; i++){
		tabH[i] = (Cellule *)malloc(N*sizeof(Cellule));
		actuel = tabH[i];
		if (tabH[i] == NULL){
			printf("liste non initialisée\n");
			return 0;
		}
		actuel->valeur = allouerCelmot(NULL, NULL);
	}
	//Menu si ./Index
	if(argc == 1){
		printf("./Index : menu des fonctionnalités\n");
		printf("./Index -question [mot] Fichier\n");
		printf("Type de questions 'question' :\n");
		printf("-a mot : appartenance du mot au texte\n");
		printf("-p mot : Suite des positions du mot dans le texte\n");
		printf("-P mot : Affiche les phrases du texte contenant mot.\n");
		printf("-l : Affiche la liste triée des mots du texte.\n");
		printf("-d mot : Affiche ensemble des mots du texte avec mot pour préfixe\n");
		printf("-D : Sauvegarde de l'index\n");
	}

	else if(argc == 3){
		//Remplissage de la table de hachage
		file = fopen(argv[2], "r");
		if (file == NULL)
			printf("fichier inexistant.\n");
		else if(!remplirTableauHachage(file, &tabH))
			printf("Table de hachage non initialisée\n");

		else{
			//Renvoie l'ensemble des mots distincts du texte dans l'ordre alphabétique
			if(!strcmp(argv[1], "-l")){
				printf("inachevé\n");
			}
			//Sauvegarde l'index et la liste triée.
			else if(!strcmp(argv[1], "-D")){
				printf("inachevé\n");
			}
			else{
				//Message signalant un problème d'arguments.
				printf("Veuillez respecter cette écriture : ./Index -question [mot] Fichier (pas besoin de mot pour -l ou -D)\n");
			}
		}
	}

	else if(argc == 4){
		//Remplissage de la table de hachage
		file = fopen(argv[3], "r");
		if (file == NULL)
			printf("fichier inexistant.\n");
		else if(!remplirTableauHachage(file, &tabH))
			printf("Table de hachage non initialisée\n");

		else{
			//Vérifie l'appartenance du mot recherhcé dans le texte du fichier.
			if(!strcmp(argv[1], "-a")){
				if(appartenanceMotTexte(tabH, argv[2]))
					printf("Le mot %s appartient bien au texte\n", argv[1]);
				else
					printf("Le mot %s n'appartient pas au texte\n", argv[1]);
			}
			//Renvoie la liste des positions des phrases où se trouvent le mot recherché
			else if(!strcmp(argv[1], "-p")){
				affichePosMot(tabH, argv[2]);
			}
			//Renvoie les phrases où se trouvent le mot recherché
			else if(!strcmp(argv[1], "-P")){
				affichePhraseMotDonne(file, argv[2], tabH);
			}
			//Renvoie l'ensemble des mots du fichier (provenant de la liste triée) commencant par le préfixe donné.
			else if(!strcmp(argv[1], "-d")){
				printf("inachevé\n");
			}
			else{
				//Message signalant un problème d'arguments.
				printf("Veuillez respecter cette écriture : ./Index -question [mot] Fichier (pas besoin de mot pour -l ou -D)\n");
			}
		}
	}


	else
		//Message signalant un problème d'arguments.
		printf("Veuillez respecter cette écriture : ./Index -question [mot] Fichier (pas besoin de mot pour -l ou -D)\n");
	
	//Libération de la mémoire, fermeture du fichier.
	libererTableau(&tabH);
	fclose(file);

	return 0;	
}