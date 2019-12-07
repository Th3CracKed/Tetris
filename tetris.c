#include <stdio.h>
#include <stdlib.h>
#include "tetris.h"
#include <time.h>
/*
struct sPiece
{
	int hauteur;
	int largeur;
	char* forme[HAUTEUR_MAX_DES_PIECES];

};

typedef struct sPiece Piece;
*/
//typedef char Grille[HAUTEUR][LARGEUR];

void initialiseGrille(Grille g){
	for(int i=0;i<HAUTEUR;i++){
		for(int j=0;j<LARGEUR;j++){
			g[i][j]=' ';
		}
	}
}


char  lireCase(Grille g,int nligne,int ncolonne){
	char s='0'; // s'il la condition est validÃ© le programme affiche 0 comme erreur
	if(ncolonne>=LARGEUR || nligne>=HAUTEUR ){

	}else{
		s=g[nligne][ncolonne];
	}
	
	return s;
}
void afficheGrille(Grille g){
	for(int i=HAUTEUR-1;i>=0;i--){
		printf("||");
		for(int j=0;j<LARGEUR;j++){
			char tmp=lireCase(g,i,j);
			printf("%c",tmp);
		}
		printf("||\n");
	}
	
	printf("||");
	for(int i=0;i<LARGEUR;i++){
		printf("|");
	}
	printf("||\n");

	printf("  ");
	for(int i=0;i<LARGEUR;i++){
		printf("%d",i%10);
	}
	printf("\n");
	
}


void genererPieces (Piece tabPiece[]){
	
	tabPiece[0].hauteur = 4;
    tabPiece[0].largeur = 1;
    tabPiece[0].forme[3] = "@";
    tabPiece[0].forme[2] = "@";
    tabPiece[0].forme[1] = "@";
    tabPiece[0].forme[0] = "@";
    
	tabPiece[1].hauteur = 1;
    tabPiece[1].largeur = 4;
	tabPiece[1].forme[0] = "@@@@"; 

	tabPiece[2].hauteur = 2;
    tabPiece[2].largeur = 2;
    tabPiece[2].forme[1] = "##";
	tabPiece[2].forme[0] = "##";
	
	tabPiece[3].hauteur = 3;
    tabPiece[3].largeur = 2;
    tabPiece[3].forme[2] = " -";
	tabPiece[3].forme[1] = "--";
	tabPiece[3].forme[0] = " -";

	tabPiece[4].hauteur = 2;
    tabPiece[4].largeur = 3;
	tabPiece[4].forme[1] = "---";
	tabPiece[4].forme[0] = " - ";
		
	tabPiece[5].hauteur = 3;
    tabPiece[5].largeur = 2;
    tabPiece[5].forme[2] = "- ";
	tabPiece[5].forme[1] = "--";
	tabPiece[5].forme[0] = "- ";

	tabPiece[6].hauteur = 2;
    tabPiece[6].largeur = 3;
    tabPiece[6].forme[1] = " - ";
	tabPiece[6].forme[0] = "---";


	tabPiece[7].hauteur = 3;
    tabPiece[7].largeur = 2;
	tabPiece[7].forme[2] = "ll";
	tabPiece[7].forme[1] = " l";
	tabPiece[7].forme[0] = " l";
	
	tabPiece[8].hauteur = 2;
    tabPiece[8].largeur = 3;
	tabPiece[8].forme[1] = "  l";
	tabPiece[8].forme[0] = "lll";

	tabPiece[9].hauteur = 3;
    tabPiece[9].largeur = 2;
    tabPiece[9].forme[2] = "l ";
	tabPiece[9].forme[1] = "l ";
	tabPiece[9].forme[0] = "ll";

	tabPiece[10].hauteur = 2;
    tabPiece[10].largeur = 3;
	tabPiece[10].forme[1] = "lll";
	tabPiece[10].forme[0] = "l  ";
}

void affichePiece(Piece p){

	for(int i=p.hauteur-1;i>=0;i--){
				printf("%s\n",p.forme[i]);
	}
	printf("%s\n","|");
}

void ecrireCase(Grille g,int nligne,int ncolonne,char c){
	if(ncolonne>=LARGEUR || nligne>=HAUTEUR ){

			printf("erreur ligne ou colonne invalide\n");
		}else{
				g[nligne][ncolonne]=c;
	}
}

int hauteurMax (Grille g, int initial ,int final){
	int ok=-1;
	for (int i = 0; i <HAUTEUR; i++)
	{
		for (int j = initial; j <final ; j++)
		{
			if(g[i][j]!=' '){
				ok=i;
				break;
			}
		}
	}
	return ok;
}

int hauteurExacte( Grille g, int col, Piece piece ){
	int i=0;
	int t[piece.largeur];
	for(int l=col;l<col+piece.largeur;l++){
		int h=hauteurMax(g,col+i,col+1+i);
		int c=0;
		for(int j=0;j<piece.hauteur;j++){
			if(piece.forme[j][i]!=' '){
			break;
			}else{
			c++;
			}
		}
		t[i]=h+1-c;
		i++;
	}
	int max=0;
	for(int i=0;i<piece.largeur;i++)
	{
		if(max<t[i])
		max=t[i];
	}
	return max;
}

void ecrirePiece (Grille g, Piece p, int col,int haut){
	
	for (int i = 0; i < p.hauteur; i++)
	{
		for (int j = 0; j < p.largeur; j++)
	{
		if(p.forme[i][j]!=' ')
		ecrireCase(g,haut+i,col+j,p.forme[i][j]);
	}
	}
}
int pieceAleatoire (){
	int alea = (int)(((double)rand()/((double)RAND_MAX)) * (NB_PIECES));
		return alea;
}
void supprimerLigne (Grille g,int nligne){
	if(nligne<HAUTEUR-1){
	
	for(int i=nligne;i<HAUTEUR-1;i++){
		for(int j=0;j<LARGEUR;j++)
			g[i][j]=g[i+1][j];
	}
	for(int j=0;j<LARGEUR;j++)
		g[HAUTEUR-1][j]=' ';
	
	}else if(nligne==HAUTEUR-1){	
	for(int j=0;j<LARGEUR;j++)
		g[HAUTEUR-1][j]=' ';
	}else{
		printf("suppression impossible");
	}
}
void nettoyer(Grille g){
	for(int i=0;i<HAUTEUR;i++){
		int c=0;
		for(int j=0;j<LARGEUR;j++){
			if(g[i][j]==' '){
			c++;
			break;
			}
		}
		if(c==0){
		supprimerLigne(g,i);
		i--; //si je supprime,je dois decremente pour tester dans la prochaine itteration la ligne qui était en dessus avant la suppression
		}
	}
}

int main_console (int argc, char *argv[]){
	int col1;
	do
	{
	Piece p[NB_PIECES];
	Grille g;
	initialiseGrille(g);
	genererPieces(p);
	int conteur=0;
	do
	{
		srand(time(NULL));
		int alea=pieceAleatoire();
		Piece UnePiece=p[alea];
		affichePiece(UnePiece);
		
		printf("\n------------------------\n");
		
		afficheGrille(g);
		printf("Donner une numero colonne:\n");
		scanf("%d",&col1);
		//int haut=hauteurMax(g,col1,col1+UnePiece.largeur);
		int haut=hauteurExacte(g,col1,UnePiece);
		if(haut+UnePiece.hauteur>HAUTEUR){
			printf("Vous avez perdu,Vous arrivez a placer %d pieces\n",conteur);
			break;
		}else{
		ecrirePiece(g,UnePiece,col1,haut);
		conteur++;
		nettoyer(g);
		}
	}while(col1!=-1);
	}while(col1!=-1);
}