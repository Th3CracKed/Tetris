#include <stdlib.h>
#include <math.h>
#include <gtk/gtk.h>
#include "tetris.h"
#include <time.h>
#include <string.h>
#define MAX_DELAY 16
typedef struct {
  Grille g;
  Piece tab[ NB_PIECES ];
  int piece; // la piece "en cours" que l'on cherche à placer.
  int col; // la colonne actuelle pour la pièce que l'on cherche à caser.
  int score; // le score
  int delay; // le delay
  int pause;
  GtkWidget* drawing_area;
  GtkWidget* label_delay;
  GtkWidget* label_score;
} Jeu;
GtkWidget *create_arrow_button( GtkArrowType  arrow_type );
gboolean realize_evt_reaction( GtkWidget *widget, gpointer data );
void dessineCarre( cairo_t* cr, int ligne, int colonne, char c );
void dessineGrille( cairo_t* cr, Grille g );
void nouvellePiece(Jeu* Jeu);
void dessinePiece( cairo_t* cr, Jeu* pJeu );
gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, gpointer data );
gboolean Gauche( GtkWidget *widget, gpointer data );
gboolean Droite( GtkWidget *widget, gpointer data );
gboolean Bas( GtkWidget *widget, gpointer data );
gint tic( gpointer data );
gboolean New( GtkWidget *widget, gpointer data );
int string_equals(char* s1, char* s2);
gboolean touche_clavier( GtkWidget *widget, GdkEventKey *event, gpointer data );
gboolean RotateADroite( GtkWidget *widget, gpointer data );
void creerIHM( Jeu* ptrJeu);

int main( int   argc,char *argv[] )
{
  Jeu jeu;
  initialiseGrille( jeu.g );
  genererPieces(jeu.tab);
  srand(time(NULL));
  nouvellePiece(&jeu);  
  gtk_init (&argc, &argv);
  creerIHM( &jeu );
  gtk_main ();
  return 0;
}
////////////////////////////////////////////////////////////////
GtkWidget *create_arrow_button( GtkArrowType  arrow_type )
  { // Les boutons sont en fait des conteneurs à un seul élément.
    GtkWidget* button = gtk_button_new ();
    GtkWidget* arrow = gtk_arrow_new (arrow_type, GTK_SHADOW_OUT);
    gtk_container_add (GTK_CONTAINER (button), arrow);
    gtk_widget_show_all( button );
    return button;
  }
  /////////////////////////////////////////////////////
   gboolean realize_evt_reaction( GtkWidget *widget, gpointer data )
 { // force un événement "expose" juste derrière.
   gtk_widget_queue_draw( widget ); 
   return TRUE;
 }

 /////////////////////////////////////////////////////////////////
 void dessineCarre( cairo_t* cr, int ligne, int colonne, char c ){
    int col=(colonne+2)*TAILLE_CARRE;
    int ligneg=(ligne+4)*TAILLE_CARRE;
  switch(c){
  case '@':
  cairo_set_source_rgb (cr, 0.97, 0, 0.07);
  cairo_rectangle (cr, col, ligneg, TAILLE_CARRE,TAILLE_CARRE); // x, y, largeur, hauteur
  cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb (cr, 0.87, 0.00002, 0.07);
  cairo_stroke( cr ); // trace la forme actuelle (le même rectangle)
  break;
  case '#':
  cairo_set_source_rgb (cr, 0.0047, 1, 0);
  cairo_rectangle (cr,col ,ligneg , TAILLE_CARRE,TAILLE_CARRE); // x, y, largeur, hauteur
  cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb (cr, 0.4, 0, 0.15);
  cairo_stroke( cr ); // trace la forme actuelle (le même rectangle)
  break;
  case 'l':
  cairo_set_source_rgb (cr, 0.08, 0.96, 0.88); 
  cairo_rectangle (cr, col, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
  cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
  // => "_preserve" garde la forme (le rectangle) pour la suite
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb (cr, 0.11, 0.60, 0.56); 
  cairo_stroke( cr );// trace la forme actuelle (le même rectangle)
  break;
  case'-':
  cairo_set_source_rgb (cr, 0.99, 0.94, 0.2); 
  cairo_rectangle (cr, col, ligneg, TAILLE_CARRE, TAILLE_CARRE ); // x, y, largeur, hauteur
  cairo_fill_preserve( cr ); // remplit la forme actuelle (un rectangle)
  // => "_preserve" garde la forme (le rectangle) pour la suite
  cairo_set_line_width(cr, 1);
  cairo_set_source_rgb (cr, 0.8, 0.8, 0.18);
  cairo_stroke( cr );// trace la forme actuelle (le même rectangle)
  break;
  }
 }
 //////////////////////////////////////////////////////
 void dessineGrille( cairo_t* cr, Grille g )
 {
   cairo_set_source_rgb (cr, 0, 0.4, 1); // choisit le blanc.
   cairo_paint( cr ); // remplit tout dans la couleur choisie.
   cairo_set_source_rgb (cr, 1, 1, 1);
   cairo_rectangle (cr, 2*TAILLE_CARRE, 0, TAILLE_CARRE*LARGEUR, TAILLE_CARRE*(HAUTEUR+5)); // x, y, largeur, hauteur
   cairo_fill( cr ); // remplit la forme actuelle (un rectangle) 
   for(int i=0;i<HAUTEUR;i++){
      for(int j=0;j<LARGEUR;j++){
        if(lireCase(g,i,j)!=' '){
          dessineCarre(cr,HAUTEUR-i,j,lireCase(g,i,j));
        }
      }
    }
    //Dessin rectangle 
   cairo_set_source_rgb (cr, 0, 0, 0); 
   cairo_rectangle(cr, 2*TAILLE_CARRE, 5*TAILLE_CARRE, TAILLE_CARRE*(LARGEUR+4)-4*TAILLE_CARRE, 1);
   cairo_fill(cr);
 }
 ///////////////////////////////////////////////////////
 void nouvellePiece(Jeu* Jeu){
  Jeu->piece=pieceAleatoire();
  Jeu->col=(LARGEUR-(Jeu->tab[Jeu->piece].largeur))/2;
 }
 //////////////////////////////////////////////////////
 void dessinePiece( cairo_t* cr, Jeu* pJeu ){
  for(int i=0;i<pJeu->tab[pJeu->piece].hauteur;i++){
    for(int j=0;j<pJeu->tab[pJeu->piece].largeur;j++)
    {
     dessineCarre(cr,-i-1,pJeu->col+j,pJeu->tab[pJeu->piece].forme[i][j]);
    }
  }
 }
 //////////////////////////////////////////////////////
  // c'est la réaction principale qui va redessiner tout.
 gboolean expose_evt_reaction( GtkWidget *widget, GdkEventExpose *event, gpointer data )
 {
   Jeu* pJeu = (Jeu*) data;
   // via Cairo 
   cairo_t* cr = gdk_cairo_create (widget->window);
   dessineGrille(cr,pJeu->g);
   nettoyer(pJeu->g);
   dessineGrille(cr,pJeu->g);
   dessinePiece(cr,pJeu);
   cairo_destroy (cr);
   return TRUE;
 }
 ////////////////////////////////////////////////
gboolean Gauche( GtkWidget *widget, gpointer data )
{
  Jeu* pJeu = (Jeu*) data; // récupère le pointeur vers le jeu
  if(pJeu->pause == 0){
    if (pJeu->col > 0){
        pJeu->col -= 1;
        gtk_widget_queue_draw( pJeu->drawing_area);
    }
  }
    return TRUE; // Tout s'est bien passé
}
gboolean Droite( GtkWidget *widget, gpointer data )
{
  Jeu* pJeu = (Jeu*) data; // récupère le pointeur vers le jeu
  if(pJeu->pause == 0){
    if ((pJeu->col)+(pJeu->tab[pJeu->piece].largeur) < LARGEUR){
        pJeu->col += 1;
        gtk_widget_queue_draw( pJeu->drawing_area);
    }
  }
  return TRUE; // Tout s'est bien passé
}
gboolean Bas( GtkWidget *widget, gpointer data )
{
  Jeu* pJeu = (Jeu*) data; // récupère le pointeur vers le jeu
  if(pJeu->pause == 0){
  int haut=hauteurExacte(pJeu->g,pJeu->col,pJeu->tab[pJeu->piece]);
  if(haut+pJeu->tab[pJeu->piece].hauteur>HAUTEUR){
    pJeu->pause=1;
    printf("Partie Terminé!\n");
    }else{
    ecrirePiece(pJeu->g,pJeu->tab[pJeu->piece],pJeu->col,haut);
    nouvellePiece(pJeu);  
    pJeu->delay = MAX_DELAY; //on reinitialise le delay
    //on met a jour le score
      pJeu->score = pJeu->score + 1;
      char str_score[2];
      sprintf(str_score,"%d", pJeu->score);
      char var_score[] = "Score:    "; 
      strcat(var_score, str_score);
      gtk_label_set_text( GTK_LABEL( pJeu->label_score ), var_score);
      gtk_widget_show (pJeu->label_score);
    gtk_widget_queue_draw( pJeu->drawing_area);
    }
    }
  return TRUE; // Tout s'est bien passé
}
////////////////////////////////////////////////////
gint tic( gpointer data ){
  //Recuperation JEU
    Jeu* pJeu = (Jeu*) data;// récupère le pointeur vers le jeu
    char str[3];
    if(pJeu->pause == 0){
      if (pJeu->delay>0){
          sprintf(str,"%d", pJeu->delay);
          char var[] = "Delay:    "; 
          strcat(var, str);
          gtk_label_set_text( GTK_LABEL( pJeu->label_delay ), var);
          gtk_widget_show (pJeu->label_delay);
          pJeu->delay = pJeu->delay-1;
      }else{
          pJeu->delay = MAX_DELAY; //on reinitialise le delay
          Bas(pJeu->drawing_area, pJeu);//on bouge a la place du joueur
      }
    }
    g_timeout_add (1000, tic, (gpointer) pJeu ); // réenclenche le timer.
    return 0;
}
////////////////////////////////////////////////////
//EVENEMENT NEW
gboolean New( GtkWidget *widget, gpointer data ){
    Jeu* pJeu = (Jeu*) data; // récupère le pointeur vers le jeu
    initialiseGrille(pJeu->g); // on reinitialise le jeu pour une nouvelle partie
    nouvellePiece(pJeu);
    pJeu->pause=0;
    //on met a jour le delay a MAX_DELAY
    pJeu->delay = MAX_DELAY;//on reinitialise le delay
    char str[3];
    sprintf(str,"%d", pJeu->delay);
    char var[] = "Delay:    "; 
    strcat(var, str);
    gtk_label_set_text( GTK_LABEL( pJeu->label_delay ), var);
    gtk_widget_show (pJeu->label_delay);
    pJeu->delay = pJeu->delay-1;
    //on met a jour le score a 0
    pJeu->score = 0;//on reinitialise le score
    char str_score[2];
    sprintf(str_score,"%d", pJeu->score);
    char var_score[] = "Score:    "; 
    strcat(var_score, str_score);
    gtk_label_set_text( GTK_LABEL( pJeu->label_score ), var_score);
    gtk_widget_show (pJeu->label_score);
    gtk_widget_queue_draw( pJeu->drawing_area ); //redessine la zone de dessin
    return TRUE; // Tout s'est bien passé
}
int string_equals(char* s1, char* s2){
    if (strlen(s1) != strlen(s2)) return 0; //si la taille de s1 et de s2 sont différentes on return 0
    else if (strcmp(s1, s2)==0) return 1; //si s1==s2 on return 1
    else return 0; //Autrement on retourn 0 s'ils ne sont pas égaux
}
gboolean touche_clavier( GtkWidget *widget, GdkEventKey *event, gpointer data ){
    if (string_equals(gdk_keyval_name(event->keyval),"Right")==1)
        Droite(widget, data);
    else if (string_equals(gdk_keyval_name(event->keyval),"Left")==1)
        Gauche(widget, data);
    else if (string_equals(gdk_keyval_name(event->keyval),"Down")==1)
        Bas(widget, data);
    else if (string_equals(gdk_keyval_name(event->keyval),"space")==1) //Si boutton echap cliquer on ferme le jeu
        RotateADroite(widget, data);  
    else if (string_equals(gdk_keyval_name(event->keyval),"Escape")==1)
        gtk_exit(EXIT_SUCCESS);
    return TRUE;
}
gboolean RotateADroite( GtkWidget *widget, gpointer data ){
    Jeu* pJeu = (Jeu*) data; // récupère le pointeur vers le jeu
    int tab_arobase[] = {0,1};
    int tab_tire[] = {3,4,5,6};
    int tab_diese[] = {7,8,9,10};
    if(pJeu->pause == 0){
    switch (pJeu->piece){
        case 0:case 1:
            pJeu->piece = tab_arobase[(pJeu->piece+1)%2];
                if (pJeu->col > LARGEUR-pJeu->tab[pJeu->piece].largeur)
                  pJeu->piece = tab_arobase[(pJeu->piece-1)%2];
                break;
        case 3:case 4:case 5:case 6:
            pJeu->piece = tab_tire[(pJeu->piece-2)%4];
            if (pJeu->col > LARGEUR-pJeu->tab[pJeu->piece].largeur)
                pJeu->piece = tab_tire[(pJeu->piece-4)%4];
            break;
        case 7:case 8:case 9:case 10:
            pJeu->piece = tab_diese[(pJeu->piece-6)%4];
            if (pJeu->col > LARGEUR-pJeu->tab[pJeu->piece].largeur)
                pJeu->piece = tab_diese[(pJeu->piece-8)%4];
            break;
        default:
            break;
    }
  }
    gtk_widget_queue_draw( pJeu->drawing_area ); //redessine la zone de dessin
    return TRUE;
}
/////////////////////////////////////////////////////
void creerIHM( Jeu* ptrJeu) {
  ///////////////////////////////////////////////////
  GtkWidget *window;
/* Crée une fenêtre. */
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
       /* La rend visible. */
    gtk_widget_show  (window);
    GtkWidget* vbox1 = gtk_vbox_new (FALSE, 10);
    GtkWidget* hbox2 = gtk_hbox_new (FALSE, 10);
    GtkWidget* vbox3 = gtk_vbox_new (TRUE, 10);
    GtkWidget* vbox4 = gtk_vbox_new (TRUE, 10);
    GtkWidget* hbox5 = gtk_hbox_new (TRUE, 10);
    GtkWidget* hbox4 = gtk_hbox_new (TRUE, 10);
    // --LEs labels
                //LAbel score
    GtkWidget* label_score = gtk_label_new( "Score  0" );
    ptrJeu->label_score = label_score; //on passe le label de score au structure jeu
    ptrJeu->score = 0; //on initialise le score
                
                
    //Label Delay
    GtkWidget* label_delay = gtk_label_new("delay   0");
    ptrJeu->label_delay = label_delay;
    ptrJeu->delay = MAX_DELAY;

    g_timeout_add (200, tic, (gpointer) ptrJeu );
    ptrJeu->pause=0;
                
    
    GtkWidget*  button_quit = gtk_button_new_with_label ( "Quit" );
    GtkWidget*  button_new  = gtk_button_new_with_label ( "New" );
     // ...
    GtkWidget* left = create_arrow_button( GTK_ARROW_LEFT );
    // ...
    GtkWidget* right = create_arrow_button( GTK_ARROW_RIGHT );

    GtkWidget* down = create_arrow_button( GTK_ARROW_DOWN );
    GtkWidget* rotate = gtk_button_new_with_label ( "Rotate" );
                
                
                
    GtkWidget* drawing_area = gtk_drawing_area_new ();
    // largeur=150 pixels, hauteur = 100 pixels.
    gtk_widget_set_size_request (drawing_area, TAILLE_CARRE*(LARGEUR+4), TAILLE_CARRE*(HAUTEUR+7));
    ptrJeu->drawing_area=drawing_area;
  /////////////////////////////////////////////////////////
  gtk_container_add ( GTK_CONTAINER (window), vbox1 );
  gtk_container_add ( GTK_CONTAINER (vbox1), hbox2 );
  gtk_container_add ( GTK_CONTAINER (vbox1), vbox3 );
  gtk_container_add ( GTK_CONTAINER (hbox2), drawing_area );
  gtk_container_add ( GTK_CONTAINER (hbox2), vbox4 );
  gtk_container_add ( GTK_CONTAINER (vbox4), hbox5 );
  gtk_container_add ( GTK_CONTAINER (hbox5), button_new );
  gtk_container_add ( GTK_CONTAINER (hbox5), button_quit );
  gtk_container_add ( GTK_CONTAINER (vbox3), hbox4 );  
  gtk_container_add ( GTK_CONTAINER (vbox4), label_score );
  gtk_container_add ( GTK_CONTAINER (vbox4), label_delay );
  gtk_container_add ( GTK_CONTAINER (hbox4), left );
  gtk_container_add ( GTK_CONTAINER (hbox4), down );
  gtk_container_add ( GTK_CONTAINER (hbox4), right );
  gtk_container_add( GTK_CONTAINER( hbox4 ), rotate );
    // Rend visible le bouton.
    gtk_widget_show (vbox1);
    gtk_widget_show (hbox2);
    gtk_widget_show (vbox3);
    gtk_widget_show (vbox4);
    gtk_widget_show (drawing_area);
    gtk_widget_show (hbox5);
    gtk_widget_show (button_quit);
    gtk_widget_show (button_new);
    gtk_widget_show (hbox4);
    gtk_widget_show (left);
    gtk_widget_show (right);
    gtk_widget_show (down);
    gtk_widget_show (label_score);
    gtk_widget_show (label_delay);
    gtk_widget_show (rotate );
    
    /////////////////////////////////////////////
  g_signal_connect( left, "clicked",
                G_CALLBACK( Gauche ), ptrJeu );
  g_signal_connect( right, "clicked",
                G_CALLBACK( Droite ), ptrJeu );
  g_signal_connect( down, "clicked",
                G_CALLBACK( Bas ), ptrJeu );
  g_signal_connect( button_new, "clicked",
                            G_CALLBACK( New ),
                            ptrJeu);
  g_signal_connect( button_quit, "clicked",
                    G_CALLBACK( gtk_main_quit ),NULL);
  g_signal_connect( rotate, "clicked",
                G_CALLBACK( RotateADroite ), ptrJeu );
  g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (touche_clavier), ptrJeu);
  ////////////////////////////////////////////////
    // ... votre zone de dessin s'appelle ici "drawing_area"
  g_signal_connect( G_OBJECT(drawing_area), "realize", 
            G_CALLBACK(realize_evt_reaction), ptrJeu );
  g_signal_connect( G_OBJECT (drawing_area), "expose_event",
                    G_CALLBACK (expose_evt_reaction), ptrJeu );
  ///////////////////////////////////////////////////
}