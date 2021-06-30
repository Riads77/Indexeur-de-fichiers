/* supprime les caracteres accentue et remplace majuscules par minusculesc*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main(int argc,char *argv[])
{int c;
  FILE*in,*out;
  char *s;
  if (2!=argc){
      fprintf(stderr,"usage %s fichier\n",argv[0]);
      return 1;
  }
  if((in=fopen(argv[1],"r"))==NULL){
    fprintf(stderr,"Ouverture de %s en lecture impossible\n",argv[1]);
    return 1;
  }
  if ((s=(char *)malloc(strlen(argv[1])+6))==NULL){
    fprintf(stderr,"probleme memoire\n");
    return 1;
  }
  strcpy(s,argv[1]);
  strcat(s,".ascii");
  if((out=fopen(s,"w"))==NULL){
    fprintf(stderr,"Ouverture de %s en ecriture impossible\n",argv[1]);
    return 1;
  }
  while((c=fgetc(in))!=EOF){
    switch(c){
    case 192: 
    case 193:
    case 194:
    case 195:
    case  196:
    case 197:c='A';break;
    case 199:c='C';break;/*C cedille*/
    case 200:
    case 201:
    case 202:
    case 203:c='E';break;
    case  204 : 
    case  205 : 
    case  206 : 
    case  207 : c='I';break;
    case 208:c='D';break;
    case 209:c='N';break;
    case  210 : 
    case  211 : 
    case  212 : 
    case  213 : 
    case  214 : c='O';break;
    case  217 : 
    case  218 : 
    case  219 : 
    case  220 : c='U';break;
    case  221 : c='Y';break;
    case  224 : 
    case  225 : 
    case  226 : 
    case  227 : 
    case  228 : 
    case  229 : 
    case  230 : c='a';break;
    case 231:c='c'; break;
    case 232:
    case 233 :
    case 234:
    case 235:c='e'; break;
    case  236: 
    case  237 : 
    case  238 : 
    case  239 : c='i'; break;

    case  241 :c='n'; break; 
    case  242 : 
    case  244 : 
    case  245 : 
    case  246 : 
    case  243 : c='o'; break;

    case  249 : 
    case  250 : 
    case  251 : 
    case  252 :  c='u'; break;
    case  253 : 
    case  255 : c='y'; break;
    }
    fputc(c,out);
  }
  fclose(in);
  fclose(out);
  return 0;
}
