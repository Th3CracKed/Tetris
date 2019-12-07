#ifndef _TETRIS_H_
#define _TETRIS_H_
/* Constantes. */
#define HAUTEUR 10
#define LARGEUR 15
#define NB_PIECES  11
#define HAUTEUR_MAX_DES_PIECES 4
#define TAILLE_CARRE 16
/* ... */
/* Types. */
typedef char Grille[ HAUTEUR ][ LARGEUR ];
typedef struct sPiece {
	int hauteur;
	int largeur;
	char* forme[HAUTEUR_MAX_DES_PIECES];
} Piece;
/* ... */
/* Fonctions. */
extern void genererPieces( Piece tabPiece[ NB_PIECES ] );
extern void affichePiece( Piece p );
extern char lireCase( Grille G, int ligne, int colonne );
extern void initialiseGrille(Grille g);
extern void afficheGrille(Grille g);
extern void ecrireCase(Grille g,int nligne,int ncolonne,char c);
extern int hauteurMax (Grille g, int initial ,int final);
extern int hauteurExacte( Grille g, int col, Piece piece );
extern void ecrirePiece (Grille g, Piece p, int col,int haut);
extern int pieceAleatoire ();
extern void supprimerLigne (Grille g,int nligne);
extern void nettoyer(Grille g);
/* ... */
#endif /* ifndef _TETRIS_H_ */