#include <libTableauNoir.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/************************************/
/* définitions des types structurés */
/************************************/

/* coordonnées dans le plan */
typedef struct {double x ; double y ;} coord_t ;

/* description du mouvement  */
typedef struct {coord_t position ; coord_t v ; coord_t a ; double r ; double w ;} mouvement_t ;

/* état de la course */
typedef struct {int tour ; int pass ;} course_t ;

/* tableau de stockage des statistiques des joueurs */
typedef struct {double vitesse ; double acceleration ; double maniabilite ; int touches[5] ;} stats_t ;

/* structure comprenant la plage de coordonnées de la ligne de départ/arrivée et des 5 points de passage. */
typedef struct {coord_t depart[2] ; coord_t P[5][2] ;} checkpoint_t ;

/* Contient l'objet en possession d'un joueur, l'objet en cours d'activation d'un joueur, l'effet d'un objet que subit un joueur et un compteur de temps  */
typedef struct {int objet ; int effet ; double temps ; double angle_initial;  double timer_objet ;} statut_t ;

/* caractéristiques d'une voiture */
typedef struct {mouvement_t d ; stats_t stats ; int id ; int phase ; int f ; course_t c ; statut_t statut ;} voiture_t ;

/* Positions des boites objets, il y aura toujours 8 boites objets peu importe le circuit */
typedef struct {coord_t Box[8] ; double timer[8] ;} items_boxes_t ;

/* Structure caractérisant une banane */
typedef struct {coord_t position ; double timer ;} banane_t ;

/* Structure caractérisant une bombe */
typedef struct {coord_t position ; double timer ; int direction ; int detone ; double tDetone ;} bombe_t ;

/* Structure caractérisant une carapace bleue */
typedef struct {coord_t position ; double timer ; int vitesse ; int lanceur ;} carapace_t ;

/* Positions des potentielles bananes sur la piste, et leur durée de vie */
typedef struct {banane_t bananes[134] ; int nb_banane ; bombe_t bombes[20] ; int nb_bombe ; carapace_t carapaces[15] ; int nb_carapace ;} objets_t ;

/* Caractéristiques propres au circuit sélectionné */
typedef struct {checkpoint_t check ; items_boxes_t items ;} circuit_t ;

/* tableau d'images */
typedef struct {Image Im[72] ; Image Pix[42] ;} gallerie_t ;

/* ensemble des données */
typedef struct {voiture_t J1 ; voiture_t J2 ;  int etat ; int id_circuit ; circuit_t circuit ; int gagnant ; double chrono ; objets_t objets ;} donnees_t ;

/******************************/
/* déclarations des fonctions */
/******************************/

/* Opérations sur les vecteurs */

double norme(coord_t) ;
coord_t mult(double, coord_t) ;
coord_t plus(coord_t, coord_t) ;
coord_t vect(coord_t, coord_t) ;
double dist(coord_t, coord_t) ;
double calculAngle(coord_t, coord_t) ;

/* Fonctions d'état du programme */

gallerie_t init_gallerie() ;
gallerie_t ajustement_gallerie(gallerie_t) ;
donnees_t init_donnees() ;
circuit_t init_circuit(donnees_t) ;
circuit_t init_Circuit_Yoshi() ;
circuit_t init_Circuit_Mario() ;
circuit_t init_Plaines_Donut() ;
circuit_t init_Circuit_Luigi() ;
donnees_t avant_course(donnees_t, EtatSourisClavier, gallerie_t, double dt) ;
donnees_t accueil(donnees_t, EtatSourisClavier, gallerie_t) ;
donnees_t selectionPersonnage(donnees_t, EtatSourisClavier, gallerie_t) ;
donnees_t selectionCircuit(donnees_t, EtatSourisClavier, gallerie_t, double dt) ;
void preparation_course(donnees_t *, stats_t *, stats_t *, gallerie_t) ;
donnees_t ecran_de_victoire(donnees_t, EtatSourisClavier, gallerie_t, int *) ;
donnees_t reset_stats(donnees_t, int *) ;
void compte_a_rebours(int *, double, donnees_t *, gallerie_t) ;
void libererGallerie(gallerie_t) ;


/* Fonctions liées à l'affichage */

void afficherCourse(donnees_t, gallerie_t) ;
void afficherCompteursTours(donnees_t, gallerie_t) ;
void afficherCasesPersos(donnees_t, gallerie_t) ;
void afficherJoueur(voiture_t, gallerie_t) ;
void afficherCadres(donnees_t, gallerie_t) ;
void afficherChoixPerso1(gallerie_t) ;
void afficherChoixPerso2(int, gallerie_t) ;
void tracerMenuChoixPersos(gallerie_t, int) ;
void afficherVictoire(donnees_t, gallerie_t) ;
void choisirCouleurCadre(voiture_t) ;
void afficherPlace(donnees_t, gallerie_t) ;
void afficherObjet(donnees_t, gallerie_t) ;
void affichierCircuit(donnees_t, gallerie_t) ;
void afficherBoites(donnees_t, gallerie_t) ;
void afficherBananes(objets_t, gallerie_t) ;
void afficherBombes(objets_t *, gallerie_t) ;
void afficherExplosion(objets_t *, int, gallerie_t) ;
void afficherEffets(voiture_t, gallerie_t) ;
void afficherCarapaces(objets_t, gallerie_t) ;

/* Autres fonctions d'état du programme */

void ajustement_stats(voiture_t *) ;
void repositionnement(donnees_t *) ;
int cap_vitesse(voiture_t) ;
voiture_t frein(voiture_t) ;
void maj_mouvement(voiture_t *, EtatSourisClavier, double dt, double) ;
void maj_position(donnees_t *, double dt) ;
void maj_vitesse(voiture_t *, double dt, double) ;
donnees_t verificationObjet(donnees_t, double dt) ;
int toucheObjetPiste(voiture_t, coord_t, int) ;
void collision(voiture_t *) ;
void inverserTouches(voiture_t *, stats_t) ;
void retablirTouches(voiture_t *, stats_t) ;
int franchit_Depart(voiture_t, checkpoint_t) ;
int franchit_Zone(voiture_t, checkpoint_t, int) ;
void nouveau_Tour(voiture_t *, checkpoint_t *) ;
void detectionPiste(voiture_t *, double, stats_t) ;
void animationTouche(voiture_t *, double dt, double) ;
void utiliserObjetJ1(donnees_t *, double dt, EtatSourisClavier, stats_t, stats_t) ;
void utiliserObjetJ2(donnees_t *, double dt, EtatSourisClavier, stats_t, stats_t) ;
void consommerObjetJ1(donnees_t *, EtatSourisClavier) ;
void consommerObjetJ2(donnees_t *, EtatSourisClavier) ;
void placerBanane(voiture_t, objets_t *) ;
void placerBombe(voiture_t, objets_t *, EtatSourisClavier) ;
void lancerCarapaceJ1(donnees_t *) ;
void lancerCarapaceJ2(donnees_t *) ;
void retirerBanane(objets_t *, int) ;
void retirerBombe(objets_t *, int) ;
void retirerCarapace(objets_t *, int) ;
void actualisation_bananes(donnees_t *, double dt) ;
void actualisation_bombes(donnees_t *, double dt) ;
void actualisation_carapaces(donnees_t *, double dt) ;
void boost_vitesse_carapace(donnees_t *, int) ;
donnees_t maj_donnees(donnees_t, EtatSourisClavier, double dt, stats_t, stats_t) ;

/***********************/
/* Fonction principale */
/***********************/
 
int main()
{
    creerTableau() ;
    fixerModeBufferisation(1) ;/* efface les objets mouvants */
    fixerTaille(1200, 800) ;
    donnees_t donnees = init_donnees() ; 
    gallerie_t gallerie = init_gallerie() ;
    stats_t stats1, stats2 ;
    int commencer ;
    srand(time(NULL)) ; /* Initialisation du générateur de nombres aléatoires avec comme graine l'heure actuelle */
    while ((donnees.etat != -1) && !(fermetureDemandee())) 
    {
      EtatSourisClavier esc = lireEtatSourisClavier() ;
      double dt = delta_temps() ; /* permet d'effacer les objets mouvants */     
      donnees = avant_course(donnees, esc, gallerie, dt) ;
      if (donnees.etat == 4)
        preparation_course(&donnees, &stats1, &stats2, gallerie) ;
      if (donnees.etat == 5)
      {  
	  effacerTableau() ;
	  if (commencer)
              donnees = maj_donnees(donnees, esc, dt, stats1, stats2) ;
	  afficherCourse(donnees, gallerie) ;
	  if (!commencer)
              compte_a_rebours(&(commencer), dt, &(donnees), gallerie) ;
	  tamponner() ;
      }
      if (donnees.etat == 6)
      {
	  effacerTableau() ; 
	  donnees = ecran_de_victoire(donnees, esc, gallerie, &commencer) ;
	  tamponner() ;
      }
    }
    libererGallerie(gallerie) ;
    fermerTableau() ;
    return 0 ;
}

/*********************************************/
/* Fonctions sur les opérations vectorielles */
/*********************************************/

double norme(coord_t u)
{
    return sqrt(u.x*u.x+u.y*u.y);
}

coord_t mult(double a, coord_t u)
{
    coord_t v = {a*u.x, a*u.y};
    return v;
}

coord_t plus(coord_t a, coord_t b)
{
    coord_t v = {a.x+b.x, a.y+b.y};
    return v;
}

coord_t vect(coord_t A, coord_t B)
{
    return plus(B, mult(-1,A));
}

double dist(coord_t A, coord_t B)
{
    return norme(vect(A,B));
}

double calculAngle(coord_t A, coord_t B)
{
  return atan2(B.y - A.y, B.x - A.x) ;
}

/******************************************/
/* Fonctions d'état du programme partie 1 */
/******************************************/

gallerie_t init_gallerie() 
{
  gallerie_t g ;
  
  /* Chargement des images (gallerie[0]) */
  for (int i = 0; i < 72; ++i)
  {
    char n[5] ; 
    sprintf(n, "%d", i) ; // conversion de l'entier i en chaine de caractères
    char chemin[50] = "../data/Image" ;
    strcat(chemin, n) ; // concaténation pour obtenir "../data/Image-i-"
    strcat(chemin, ".png") ; // concaténation pour obtenir "../data/Image-i-.png"
    g.Im[i] = chargerImage(chemin) ;
  }
  /* Chargement des pixelArts (gallerie[1]) */
  for (int j = 0; j < 38; ++j)
  {
    char m[5] ; 
    sprintf(m, "%d", j) ; // Même procédé mais pour les pixel arts
    char chemin[50] = "../data/PixelArt" ;
    strcat(chemin, m) ; 
    strcat(chemin, ".png") ;
    g.Pix[j] = chargerPixelArt(chemin) ;
  }
  g = ajustement_gallerie(g) ;
  return g ;
}

gallerie_t ajustement_gallerie(gallerie_t g)
{
  g.Pix[0] = rotozoomImage(g.Pix[0], 0, 3, 3) ;
  g.Pix[1] = rotozoomImage(g.Pix[1], 0, 3, 3) ;
  g.Pix[14] = rotozoomImage(g.Pix[14], 0, 0.5, 0.5) ;
  g.Pix[27] = rotozoomImage(g.Pix[27], 0, 0.9, 0.9) ;
  for (int k = 31; k < 44; ++k)
    g.Im[k] = rotozoomImage(g.Im[k], 0, 0.8, 0.8) ; /* Dezoom des statistiques */
  for (int l = 18; l < 27; ++l)
    g.Pix[l] = rotozoomImage(g.Pix[l], 0, 0.5, 0.5) ; /* Dezoom des frames d'explosion */
  for (int m = 30; m < 38; ++m)
    g.Pix[m] = rotozoomImage(g.Pix[m], 0, 1.67, 1.67) ; /* Zoom des circuits */
  g.Pix[38] = chargerImage("../data/PixelArt30.png") ;
  g.Pix[39] = chargerImage("../data/PixelArt31.png") ;
  g.Pix[40] = chargerImage("../data/PixelArt32.png") ;
  g.Pix[41] = chargerImage("../data/PixelArt33.png") ;
  return g ;
}

donnees_t init_donnees() 
{
    mouvement_t m = {v: {x: 0, y: 0}, a: {x: 0, y: 0}, r:180, w:0} ;
    course_t k = {tour: 0, pass: 0} ;
    statut_t u = {objet: 0, effet: 0, temps: 0, timer_objet: 0} ;
    stats_t s1 = {vitesse: 90, acceleration: 80, maniabilite: 140, touches: {26, 4, 22, 7, 9}} ;
    stats_t s2 = {vitesse: 90, acceleration: 80, maniabilite: 140, touches: {82, 80, 81, 79, 228}} ;
    donnees_t donnees = {J1: {d: m, stats: s1, id: 0, phase: 0, f:0, c:k, statut: u}, J2: {d: m, stats: s2, id: 0, phase: 0, f:0, c:k, statut: u}, etat: 0, gagnant: 0, id_circuit: 1, chrono: 0} ;
    
    /* Significations des valeurs de donnees.etat :
   - -1 -> phase de sortie
   - 0 -> écran d'accueil
   - 1 -> accès commandes
   - 2 -> écran de sélection du personnage
   - 3 -> écran de sélection du circuit
   - 4 -> initialisation de la course + compte à rebours
   - 5 -> course
   - 6 -> écran de victoire (rejouabilité possible)
   */
    
    return donnees ;
}

circuit_t init_circuit(donnees_t donnees)
{
    circuit_t c ;
    if (donnees.id_circuit == 1)
      c = init_Circuit_Yoshi() ;
    if (donnees.id_circuit == 2)
      c = init_Circuit_Mario() ;
    if (donnees.id_circuit == 3)
      c = init_Plaines_Donut() ;
    if (donnees.id_circuit == 4)
      c = init_Circuit_Luigi() ;
    return c ;
}

circuit_t init_Circuit_Yoshi()
{
    checkpoint_t zones ; 
    /* Ligne de départ/arrivée */
    zones.depart[0] = (coord_t){x: 58, y: -318} ; 
    zones.depart[1] = (coord_t){x: 54, y: -378} ;
    /* 5 zones de passage nécessaires pour comptabiliser un tour */ 
    zones.P[0][0] = (coord_t){x: -80, y: -65} ; 
    zones.P[0][1] = (coord_t){x: -84, y: -118} ;
    zones.P[1][0] = (coord_t){x: -178, y: 133} ;
    zones.P[1][1] = (coord_t){x: -182, y: 82} ;
    zones.P[2][0] = (coord_t){x: -32, y: 397} ;
    zones.P[2][1] = (coord_t){x: -36, y: 362} ;
    zones.P[3][0] = (coord_t){x: 126, y: 14} ;
    zones.P[3][1] = (coord_t){x: 122, y: -7} ;
    zones.P[4][0] = (coord_t){x: 128, y: -318} ;
    zones.P[4][1] = (coord_t){x: 124, y: -378} ;
    /* Coordonnées des boites objets */
    items_boxes_t boites ;
    boites.Box[0] = (coord_t){x: -105, y: -300} ;
    boites.Box[1] = (coord_t){x: -80, y: -300} ;
    boites.Box[2] = (coord_t){x: -118, y: 102} ;
    boites.Box[3] = (coord_t){x: -118, y: 125} ;
    boites.Box[4] = (coord_t){x: -40, y: 367} ;
    boites.Box[5] = (coord_t){x: -40, y: 385} ;
    boites.Box[6] = (coord_t){x: 250, y: -6} ;
    boites.Box[7] = (coord_t){x: 260, y: -23} ;
    for (int i = 0; i < 8; ++i)
      boites.timer[i] = 0 ;
    circuit_t Yoshi = {check: zones, items: boites} ;
    return Yoshi ;
}

circuit_t init_Circuit_Mario()
{
    checkpoint_t zones ; 
    /* Ligne de départ/arrivée */
    zones.depart[0] = (coord_t){x: 420, y: -35} ; 
    zones.depart[1] = (coord_t){x: 282, y: -40} ;
    /* 5 zones de passage nécessaires pour comptabiliser un tour */ 
    zones.P[0][0] = (coord_t){x: 28, y: 98} ; 
    zones.P[0][1] = (coord_t){x: 1, y: -1} ;
    zones.P[1][0] = (coord_t){x: -299, y: 298} ;
    zones.P[1][1] = (coord_t){x: -420, y: 292} ;
    zones.P[2][0] = (coord_t){x: 28, y: -17} ;
    zones.P[2][1] = (coord_t){x: 1, y: -87} ;
    zones.P[3][0] = (coord_t){x: -272, y: -261} ;
    zones.P[3][1] = (coord_t){x: -419, y: -277} ;
    zones.P[4][0] = (coord_t){x: 420, y: -208} ;
    zones.P[4][1] = (coord_t){x: 282, y: -229} ;
    /* Coordonnées des boites objets */
    items_boxes_t boites ;
    boites.Box[0] = (coord_t){x: 342, y: 180} ;
    boites.Box[1] = (coord_t){x: 366, y: 180} ;
    boites.Box[2] = (coord_t){x: 30, y: 50} ;
    boites.Box[3] = (coord_t){x: 30, y: 20} ;
    boites.Box[4] = (coord_t){x: -366, y: 194} ;
    boites.Box[5] = (coord_t){x: -342, y: 194} ;
    boites.Box[6] = (coord_t){x: -277, y: -189} ;
    boites.Box[7] = (coord_t){x: -260, y: -211} ;
    for (int i = 0; i < 8; ++i)
      boites.timer[i] = 0 ;
    circuit_t Mario = {check: zones, items: boites} ;
    return Mario ;
}

circuit_t init_Plaines_Donut()
{
    checkpoint_t zones ; 
    /* Ligne de départ/arrivée */
    zones.depart[0] = (coord_t){x: -212, y: -8} ; 
    zones.depart[1] = (coord_t){x: -419, y: -13} ;
    /* 5 zones de passage nécessaires pour comptabiliser un tour */ 
    zones.P[0][0] = (coord_t){x: -197, y: 399} ; 
    zones.P[0][1] = (coord_t){x: -211, y: 206} ;
    zones.P[1][0] = (coord_t){x: 16, y: 193} ;
    zones.P[1][1] = (coord_t){x: 2, y: 51} ;
    zones.P[2][0] = (coord_t){x: 169, y: -14} ;
    zones.P[2][1] = (coord_t){x: 152, y: -196} ;
    zones.P[3][0] = (coord_t){x: -66, y: -102} ;
    zones.P[3][1] = (coord_t){x: -79, y: -242} ;
    zones.P[4][0] = (coord_t){x: -212, y: -123} ;
    zones.P[4][1] = (coord_t){x: -420, y: -141} ;
    /* Coordonnées des boites objets */
    items_boxes_t boites ;
    boites.Box[0] = (coord_t){x: -277, y: 319} ;
    boites.Box[1] = (coord_t){x: -248, y: 319} ;
    boites.Box[2] = (coord_t){x: 329, y: 201} ;
    boites.Box[3] = (coord_t){x: 360, y: 201} ;
    boites.Box[4] = (coord_t){x: -7, y: -25} ;
    boites.Box[5] = (coord_t){x: -7, y: -50} ;
    boites.Box[6] = (coord_t){x: 163, y: -274} ;
    boites.Box[7] = (coord_t){x: 163, y: -310} ;
    for (int i = 0; i < 8; ++i)
      boites.timer[i] = 0 ;
    circuit_t Donut = {check: zones, items: boites} ;
    return Donut ;
}

circuit_t init_Circuit_Luigi()
{
    checkpoint_t zones ; 
    /* Ligne de départ/arrivée */
    zones.depart[0] = (coord_t){x: -279, y: -2} ; 
    zones.depart[1] = (coord_t){x: -390, y: -12} ;
    /* 5 zones de passage nécessaires pour comptabiliser un tour */ 
    zones.P[0][0] = (coord_t){x: -90, y: 209} ; 
    zones.P[0][1] = (coord_t){x: -202, y: 195} ;
    zones.P[1][0] = (coord_t){x: 114, y: 264} ;
    zones.P[1][1] = (coord_t){x: 94, y: 99} ;
    zones.P[2][0] = (coord_t){x: 355, y: -271} ;
    zones.P[2][1] = (coord_t){x: 225, y: -280} ;
    zones.P[3][0] = (coord_t){x: -167, y: -88} ;
    zones.P[3][1] = (coord_t){x: -255, y: -100} ;
    zones.P[4][0] = (coord_t){x: -277, y: -68} ;
    zones.P[4][1] = (coord_t){x: -391, y: -88} ;
    /* Coordonnées des boites objets */
    items_boxes_t boites ;
    boites.Box[0] = (coord_t){x: -171, y: 161} ;
    boites.Box[1] = (coord_t){x: -150, y: 143} ;
    boites.Box[2] = (coord_t){x: -67, y: 150} ;
    boites.Box[3] = (coord_t){x: -50, y: 140} ;
    boites.Box[4] = (coord_t){x: 110, y: 20} ;
    boites.Box[5] = (coord_t){x: 110, y: -4} ;
    boites.Box[6] = (coord_t){x: -7, y: -145} ;
    boites.Box[7] = (coord_t){x: -10, y: -170} ;
    for (int i = 0; i < 8; ++i)
      boites.timer[i] = 0 ;
    circuit_t Luigi = {check: zones, items: boites} ;
    return Luigi ;
}

donnees_t avant_course(donnees_t donnees, EtatSourisClavier esc, gallerie_t gal, double dt)
{
  if (donnees.etat < 2)
    donnees = accueil(donnees, esc, gal) ;
  if (donnees.etat == 2)
    donnees = selectionPersonnage(donnees, esc, gal) ;
  if (donnees.etat == 3)
    donnees = selectionCircuit(donnees, esc, gal, dt) ;
  return donnees ;
}

donnees_t accueil(donnees_t donnees, EtatSourisClavier esc, gallerie_t gal)
{
    if (donnees.etat == 0)
    {
      afficherImage(gal.Im[0], - tn_largeur(gal.Im[0])/2, tn_hauteur(gal.Im[0])/2) ;
      choisirTypeStylo(1, 210, 255, 185) ;
      choisirCouleurPinceau(210, 255, 185) ;
      tracerRectangle(364, -186, 556, -372) ;
      afficherImage(gal.Im[1], 460 - tn_largeur(gal.Im[1])/2, -280 + tn_hauteur(gal.Im[1])/2) ;
      afficherImage(gal.Im[2], - tn_largeur(gal.Im[2])/2, -200 + tn_hauteur(gal.Im[2])/2) ;
      tamponner() ;
      if ((esc.sourisBoutonGauche) && (esc.sourisX < 556) && (esc.sourisX > 364) && (esc.sourisY < -186) && (esc.sourisY > -372))
      {
          donnees.etat = 1 ;
	  effacerTableau() ;
          afficherImage(gal.Im[3], - tn_largeur(gal.Im[3])/2, -100 + tn_hauteur(gal.Im[3])/2) ;
	  choisirTypeStylo(1, 255, 185, 185) ;
          choisirCouleurPinceau(255, 185, 185) ;
          tracerRectangle(-530, -206, -265, -396) ;
	  afficherImage(gal.Im[4], -400 - tn_largeur(gal.Im[4])/2, -300 + tn_hauteur(gal.Im[4])/2) ;
	  tamponner() ;
      }
      if (esc.touchesClavier[44]) 
      {
          donnees.etat = 2 ;
	  effacerTableau() ;
          tamponner() ;
      }
    }
    if ((donnees.etat == 1) && (esc.sourisBoutonGauche) && (esc.sourisX < -265) && (esc.sourisX > -530) && (esc.sourisY < -206) && (esc.sourisY > -396))
    {
        donnees.etat = 0 ;
	effacerTableau() ;
	tamponner() ;
    }
    return donnees ;
}

donnees_t selectionPersonnage(donnees_t donnees, EtatSourisClavier esc, gallerie_t gal)
{
    effacerTableau() ;
    int tabX[4][2] = {{-158, -293}, {-8, -143}, {142, 7}, {282, 157}} ;
    int tabY[3][2] = {{217, 83}, {67, -67}, {-83, -217}} ;
    if (donnees.J1.id == 0)
    {
      afficherChoixPerso1(gal) ;
      for (int y = 0; y < 3; ++y)
      {
	for (int x = 0; x < 4; ++x)
	  {
	    if ((esc.sourisX < tabX[x][0]) && (esc.sourisX > tabX[x][1]) && (esc.sourisY < tabY[y][0]) && (esc.sourisY > tabY[y][1]))
	    {
              afficherImage(gal.Im[x+4*y+31], - tn_largeur(gal.Im[x+4*y+31])/2, -310 + tn_hauteur(gal.Im[x+4*y+31])/2) ;
	      gal.Pix[x+4*y+2] = rotozoomImage(gal.Pix[x+4*y+2], 0, 3, 3) ;
	      afficherImage(gal.Pix[x+4*y+2], 472 - tn_largeur(gal.Pix[x+4*y+2])/2, -310 + tn_hauteur(gal.Pix[x+4*y+2])/2) ;
	      
	      if (esc.sourisBoutonGauche)
                donnees.J1.id = x + 4*y + 1 ;
	    }
	  }
      }
    }
    if ((donnees.J2.id == 0) && (donnees.J1.id != 0))
    {
      afficherChoixPerso2(donnees.J1.id, gal) ;
      for (int y = 0; y < 3; ++y)
      {
	for (int x = 0; x < 4; ++x)
	  {
	    if ((esc.sourisX < tabX[x][0]) && (esc.sourisX > tabX[x][1]) && (esc.sourisY < tabY[y][0]) && (esc.sourisY > tabY[y][1]))
	    {
              afficherImage(gal.Im[x+4*y+31], - tn_largeur(gal.Im[x+4*y+31])/2, -310 + tn_hauteur(gal.Im[x+4*y+31])/2) ;
	      gal.Pix[x+4*y+2] = rotozoomImage(gal.Pix[x+4*y+2], 0, 3, 3) ;
	      afficherImage(gal.Pix[x+4*y+2], 472 - tn_largeur(gal.Pix[x+4*y+2])/2, -310 + tn_hauteur(gal.Pix[x+4*y+2])/2) ;
	      if ((esc.sourisBoutonGauche) && ((x+4*y+1) != donnees.J1.id)) 
                donnees.J2.id = x + 4*y + 1 ;
	    }
	  }
      }
    }
    
    if ((donnees.J1.id != 0) && (donnees.J2.id != 0))
    {
      donnees.etat = 3 ;
      attendreNms(100) ;
    }
    tamponner() ;
    return donnees ;
}

donnees_t selectionCircuit(donnees_t donnees, EtatSourisClavier esc, gallerie_t gal, double dt)
{
    effacerTableau() ;
    donnees.chrono += dt ;
    if (donnees.chrono >= 0.5)
    {
      afficherImage(gal.Im[71], - tn_largeur(gal.Im[71])/2, + tn_hauteur(gal.Im[71])/2) ;
      int tabY[4][2] = {{187, 72}, {56, -58}, {-73, -188}, {-204, -318}} ;
      for (int i = 0; i < 4; ++i)
      {
        if ((esc.sourisX < -125) && (esc.sourisX > -558) && (esc.sourisY < tabY[i][0]) && (esc.sourisY > tabY[i][1]))
	{
          afficherImage(gal.Pix[38+i], 344 - tn_largeur(gal.Pix[38+i])/2, -144 + tn_hauteur(gal.Pix[38+i])/2) ;
	  if (esc.sourisBoutonGauche)
	  {
	    donnees.id_circuit = i + 1 ;
            donnees.etat = 4 ;
	    donnees.chrono = 0 ;
	    delta_temps() ;
	    break ;
	  }
	}
      }
    }
    tamponner() ;
    return donnees ;
}

void preparation_course(donnees_t * donnees, stats_t * stats1, stats_t * stats2, gallerie_t gal)
{
  donnees->circuit = init_circuit(*donnees) ;
  ajustement_stats(&(donnees->J1)) ;
  ajustement_stats(&(donnees->J2)) ;
  *stats1 = (stats_t){vitesse: donnees->J1.stats.vitesse, acceleration: donnees->J1.stats.acceleration, maniabilite: donnees->J1.stats.maniabilite, {26, 4, 22, 7, 9}} ;
  *stats2 = (stats_t){vitesse: donnees->J2.stats.vitesse, acceleration: donnees->J2.stats.acceleration, maniabilite: donnees->J2.stats.maniabilite, {82, 80, 81, 79, 228}} ;
  repositionnement(donnees) ;
  afficherImage(gal.Pix[33+donnees->id_circuit], - tn_largeur(gal.Pix[33+donnees->id_circuit])/2, tn_hauteur(gal.Pix[33+donnees->id_circuit])/2) ;
  tamponner() ;
  chargerPixelsEnMemoire() ;
  donnees->etat = 5 ;
}

donnees_t ecran_de_victoire(donnees_t donnees, EtatSourisClavier esc, gallerie_t gal, int * commencer)
{
    afficherVictoire(donnees, gal) ;
    if ((esc.sourisBoutonGauche) && (esc.sourisX < -344) && (esc.sourisX > -556) && (esc.sourisY < -169) && (esc.sourisY > -381))
    {
        /* On revient aux statistiques génériques avant la sélection du personnage pour les deux joueurs */
      donnees = reset_stats(donnees, commencer) ;
    }
    if ((esc.sourisBoutonGauche) && (esc.sourisX < 556) && (esc.sourisX > 344) && (esc.sourisY < -169) && (esc.sourisY > -381))
      donnees.etat = -1 ;
    return donnees ;
}

donnees_t reset_stats(donnees_t donnees, int * commencer)
{
    donnees.J1.id = 0 ;
    donnees.J1.stats.vitesse = 90 ; 
    donnees.J1.stats.acceleration = 80 ;
    donnees.J1.stats.maniabilite = 140 ;
    donnees.J1.d.v = mult(0, donnees.J1.d.v) ;
    donnees.J1.d.a = mult(0, donnees.J1.d.a) ;
    donnees.J1.d.w = 0 ;
    donnees.J1.c = (course_t){tour: 0, pass: 0} ;
    donnees.J1.statut = (statut_t){objet: 0, effet: 0, temps: 0, timer_objet: 0} ;
    donnees.J2.id = 0 ;
    donnees.J2.stats.vitesse = 90 ; 
    donnees.J2.stats.acceleration = 80 ;
    donnees.J2.stats.maniabilite = 140 ;
    donnees.J2.d.v = mult(0, donnees.J2.d.v) ;
    donnees.J2.d.a = mult(0, donnees.J2.d.a) ;
    donnees.J2.d.w = 0 ;
    donnees.J2.c = (course_t){tour: 0, pass: 0} ;
    donnees.J2.statut = (statut_t){objet: 0, effet: 0, temps: 0, timer_objet: 0} ;
    donnees.etat = 2 ;
    donnees.gagnant = 0 ;
    for (int i = 0; i < donnees.objets.nb_banane; ++i)
      donnees.objets.bananes[i].timer = 100 ;
    donnees.objets.nb_banane = 0 ;
    for (int j = 0; j < donnees.objets.nb_bombe; ++j)
      donnees.objets.bombes[j].detone = -1 ;
    donnees.objets.nb_bombe = 0 ;
    for (int i = 0; i < donnees.objets.nb_carapace; ++i)
      donnees.objets.carapaces[i].timer = 100 ;
    donnees.objets.nb_carapace = 0 ;
    *commencer = 0 ; // Mettre cette variable déclarée dans le main à 0 permet de rétablir le "3 2 1 GO" après chaque course
    return donnees ;
}

void compte_a_rebours(int * condition, double dt, donnees_t * donnees, gallerie_t gal)
{
      donnees->chrono += dt ;
      if (donnees->chrono >= 1) 
      {
        if (donnees->chrono < 2)
	  afficherImage(gal.Im[54], - tn_largeur(gal.Im[54])/2, tn_hauteur(gal.Im[54])/2) ;
	if ((donnees->chrono >= 2) && (donnees->chrono < 3))
	  afficherImage(gal.Im[55], - tn_largeur(gal.Im[55])/2, tn_hauteur(gal.Im[55])/2) ;
	if ((donnees->chrono >= 3) && (donnees->chrono < 4))
	 afficherImage(gal.Im[56], - tn_largeur(gal.Im[56])/2, tn_hauteur(gal.Im[56])/2) ;
	if ((donnees->chrono >= 4) && (donnees->chrono < 5))
	  afficherImage(gal.Im[57], - tn_largeur(gal.Im[57])/2, tn_hauteur(gal.Im[57])/2) ;
	if ((donnees->chrono >= 5) && (donnees->chrono < 6))
	{
	  *condition = 1 ;
	  donnees->chrono = 0 ;
	}
      }
}

void libererGallerie(gallerie_t gal)
{
    for (int j = 0; j < 72; ++j)
      libererImage(gal.Im[j]) ;
    for (int k = 0; k < 38; ++k)
      libererImage(gal.Pix[k]) ;
}
/*********************************/
/* Fonctions liées à l'affichage */
/*********************************/


void afficherJoueur(voiture_t J, gallerie_t gal)
{
    /* Affiche le kart d'un joueur */
    double angle = (M_PI*J.d.r)/180 ;
    gal.Pix[J.id + 1] = rotozoomImage(gal.Pix[J.id + 1], angle, 1, 1) ;
    afficherImage(gal.Pix[J.id + 1], J.d.position.x - tn_largeur(gal.Pix[J.id + 1])/2 , J.d.position.y + tn_hauteur(gal.Pix[J.id + 1])/2) ;
}

void afficherCompteursTours(donnees_t donnees, gallerie_t gal)
{
    /* Affiche le nombre de tours effectués pour les deux joueurs */
    if (donnees.J1.c.tour == 0)
    {
      afficherImage(gal.Im[46], -510 - tn_largeur(gal.Im[46])/2, 0 + tn_hauteur(gal.Im[46])/2) ;
    }
    if (donnees.J1.c.tour == 1)
    {
      afficherImage(gal.Im[47], -510 - tn_largeur(gal.Im[47])/2, 0 + tn_hauteur(gal.Im[47])/2) ;
    }
    if (donnees.J1.c.tour == 2)
    {
      afficherImage(gal.Im[48], -510 - tn_largeur(gal.Im[48])/2, 0 + tn_hauteur(gal.Im[48])/2) ;
    }
    if (donnees.J2.c.tour == 0)
    {
      afficherImage(gal.Im[46], 510 - tn_largeur(gal.Im[46])/2, 0 + tn_hauteur(gal.Im[46])/2) ;
    }
    if (donnees.J2.c.tour == 1)
    {
      afficherImage(gal.Im[47], 510 - tn_largeur(gal.Im[47])/2, 0 + tn_hauteur(gal.Im[47])/2) ;
    }
    if (donnees.J2.c.tour == 2)
    {
      afficherImage(gal.Im[48], 510 - tn_largeur(gal.Im[47])/2, 0 + tn_hauteur(gal.Im[47])/2) ;
    }
}

void afficherCadres(donnees_t donnees, gallerie_t gal)
{
    /* Affiche tout ce qui se trouve à gauche et à droite de l'écran, qui n'appartient pas au circuit */
    choisirTypeStylo(4, 255, 255, 255) ;
    choisirCouleurCadre(donnees.J1) ;
    tracerRectangle(-430, 398, -600, -400) ;
    choisirCouleurCadre(donnees.J2) ;
    tracerRectangle(430, 398, 600, -400) ;
    choisirTypeStylo(4, 0, 0, 0) ;
    choisirCouleurPinceau(255, 255, 255) ;
    tracerRectangle(-454, 382, -584, 252) ; 
    tracerRectangle(454, 382, 584, 252) ; 
    tracerRectangle(454, -218, 584, -348) ; /* Cadre objet actif joueur 1 */
    tracerRectangle(-454, -218, -584, -348) ; /* Cadre objet actif joueur 2 */
    afficherImage(gal.Im[44], -516 - tn_largeur(gal.Im[44])/2, 232 + tn_hauteur(gal.Im[44])/2) ; /* Texte joueur 1 */
    afficherImage(gal.Im[45], 516 - tn_largeur(gal.Im[45])/2, 232 + tn_hauteur(gal.Im[45])/2) ; /* Texte joueur 2 */
    afficherCompteursTours(donnees, gal) ;
    afficherImage(gal.Im[donnees.J1.id + 6], -519 - tn_largeur(gal.Im[donnees.J1.id + 6])/2, 317 + tn_hauteur(gal.Im[donnees.J1.id + 6])/2) ;
    afficherImage(gal.Im[donnees.J2.id + 6], 519 - tn_largeur(gal.Im[donnees.J2.id + 6])/2, 317 + tn_hauteur(gal.Im[donnees.J2.id + 6])/2) ;
    afficherImage(gal.Im[49], -520 - tn_largeur(gal.Im[49])/2, -368 + tn_hauteur(gal.Im[49])/2) ;
    afficherImage(gal.Im[49], 518 - tn_largeur(gal.Im[49])/2, -368 + tn_hauteur(gal.Im[49])/2) ;
    afficherObjet(donnees, gal) ;
}

void afficherObjet(donnees_t donnees, gallerie_t gal)
{
  /* Affiche l'objet que possèdent les deux joueurs */
  for (int i = 0; i < 7; ++i)
  {
    if (donnees.J1.statut.objet == i)
    {
      if (i == 0)
	afficherImage(gal.Im[58], -519 - tn_largeur(gal.Im[58])/2, -283 + tn_hauteur(gal.Im[58])/2) ;
      else
      {
        if (donnees.J1.statut.timer_objet > 1)
          afficherImage(gal.Im[58+i], -519 - tn_largeur(gal.Im[58+i])/2, -283 + tn_hauteur(gal.Im[58+i])/2) ;
        else
	  afficherImage(gal.Im[64+i], -519 - tn_largeur(gal.Im[64+i])/2, -283 + tn_hauteur(gal.Im[64+i])/2) ;
      }
    }
  }
  for (int j = 0; j < 7; ++j)
  {
    if (donnees.J2.statut.objet == j)
    {
      if (j == 0)
	afficherImage(gal.Im[58], 519 - tn_largeur(gal.Im[58])/2, -283 + tn_hauteur(gal.Im[58])/2) ;
      else
      {
        if (donnees.J2.statut.timer_objet > 1)
          afficherImage(gal.Im[58+j], 519 - tn_largeur(gal.Im[58+j])/2, -283 + tn_hauteur(gal.Im[58+j])/2) ;
        else
	  afficherImage(gal.Im[64+j], 519 - tn_largeur(gal.Im[64+j])/2, -283 + tn_hauteur(gal.Im[64+j])/2) ;
      }
    }
  }
}

void tracerMenuChoixPersos(gallerie_t gal, int num)
{
    /* Trace l'interface de sélection de personnage en fonction du joueur, représenté ici par num */
    choisirTypeStylo(4, 182, 255, 126) ;
    choisirCouleurPinceau(182, 255, 126) ;
    tracerRectangle(-600, 400, 600, -400) ;  
    choisirTypeStylo(4, 255, 255, 255) ;
    tracerRectangle(-293, 217, -158, 83) ;
    tracerRectangle(-143, 217, -8, 83) ;
    tracerRectangle(7, 217, 142, 83) ;
    tracerRectangle(157, 217, 292, 83) ;
    tracerRectangle(-293, 67, -158, -67) ;
    tracerRectangle(-143, 67, -8, -67) ;
    tracerRectangle(7, 67, 142, -67) ;
    tracerRectangle(157, 67, 292, -67) ;
    tracerRectangle(-293, -83, -158, -217) ;
    tracerRectangle(-143, -83, -8, -217) ;
    tracerRectangle(7, -83, 142, -217) ;
    tracerRectangle(157, -83, 292, -217) ;
    afficherImage(gal.Im[4+num], - tn_largeur(gal.Im[4+num])/2, 320 + tn_hauteur(gal.Im[4+num])/2) ;    
}

void afficherChoixPerso1(gallerie_t gal)
{
    /* Affiche l'écran de sélection de personnage pour le joueur 1 */
    tracerMenuChoixPersos(gal, 1) ;
    int tabX[4] = {-225, -75, 75, 225} ;
    int tabY[3] = {150, 0, -150} ;
    for (int y = 0; y < 3; ++y) // parcours du tableau des ordonnées
    {
      for (int x = 0; x < 4; ++x) // parcours du tableau des abscisses
      {
          afficherImage(gal.Im[x+4*y+7], tabX[x] - tn_largeur(gal.Im[x+4*y+7])/2, tabY[y] + tn_hauteur(gal.Im[x+4*y+7])/2) ;
      }
    }
    afficherImage(gal.Im[43], - tn_largeur(gal.Im[43])/2, -310 + tn_hauteur(gal.Im[43])/2) ;
}

void afficherChoixPerso2(int id1, gallerie_t gal)
{
    /* Affiche l'écran de sélection de personnage pour le joueur 2 
    en prenant en compte le choix du joueur 1 */
    tracerMenuChoixPersos(gal, 2) ;
    int tabX[4] = {-225, -75, 75, 225} ;
    int tabY[3] = {150, 0, -150} ;
    for (int y = 0; y < 3; ++y) // parcours du tableau des ordonnées
    {
      for (int x = 0; x < 4; ++x) // parcours du tableau des abscisses
      {
        if (id1 != x+4*y+1) // si le personnage du joueur 1 n'a pas déjà été choisi
          afficherImage(gal.Im[x+4*y+7], tabX[x] - tn_largeur(gal.Im[x+4*y+7])/2, tabY[y] + tn_hauteur(gal.Im[x+4*y+7])/2) ;
	else
	  afficherImage(gal.Im[x+4*y+19], tabX[x] - tn_largeur(gal.Im[x+4*y+19])/2, tabY[y] + tn_hauteur(gal.Im[x+4*y+19])/2) ;
      }
    }
    afficherImage(gal.Im[43], - tn_largeur(gal.Im[43])/2, -310 + tn_hauteur(gal.Im[43])/2) ;
}

void afficherVictoire(donnees_t donnees, gallerie_t gal)
{
    /* Affiche l'écran de victoire en fonction du gagnant de la course */
    afficherImage(gal.Im[49+donnees.gagnant], - tn_largeur(gal.Im[49+donnees.gagnant])/2, tn_hauteur(gal.Im[49+donnees.gagnant])/2) ;
    if (donnees.gagnant == 1)
    {
        gal.Im[6+donnees.J1.id] = rotozoomImage(gal.Im[6+donnees.J1.id], 0, 2, 2) ;
        afficherImage(gal.Im[6+donnees.J1.id], - tn_largeur(gal.Im[6+donnees.J1.id])/2, tn_hauteur(gal.Im[6+donnees.J1.id])/2) ;
    }
    else
    {
	gal.Im[6+donnees.J2.id] = rotozoomImage(gal.Im[6+donnees.J2.id], 0, 2, 2) ;
        afficherImage(gal.Im[6+donnees.J2.id], - tn_largeur(gal.Im[6+donnees.J2.id])/2, tn_hauteur(gal.Im[6+donnees.J2.id])/2) ;
    }
    afficherImage(gal.Im[52], -450 - tn_largeur(gal.Im[52])/2, -275 + tn_hauteur(gal.Im[52])/2) ;
    afficherImage(gal.Im[53], 450 - tn_largeur(gal.Im[53])/2, -275 + tn_hauteur(gal.Im[53])/2) ; 
}

void choisirCouleurCadre(voiture_t J)
{
    /* Change la couleur du cadre du joueur en fonction du personnage choisi */
    if (J.id == 1)
      choisirCouleurPinceau(234, 27, 24) ;
    if (J.id == 2)
      choisirCouleurPinceau(71, 183, 74) ;
    if (J.id == 3)
      choisirCouleurPinceau(255, 177, 224) ;
    if (J.id == 4)
      choisirCouleurPinceau(254, 220, 49) ;
    if (J.id == 5)
      choisirCouleurPinceau(65, 101, 31) ;
    if (J.id == 6)
      choisirCouleurPinceau(88, 62, 127) ;
    if (J.id == 7)
      choisirCouleurPinceau(160, 241, 232) ;
    if (J.id == 8)
      choisirCouleurPinceau(135, 72, 50) ;
    if (J.id == 9)
      choisirCouleurPinceau(255, 226, 181) ;
    if (J.id == 10)
      choisirCouleurPinceau(111, 210, 83) ;
    if (J.id == 11)
      choisirCouleurPinceau(255, 252, 62) ;
    if (J.id == 12)
      choisirCouleurPinceau(159, 62, 222) ;
}

void afficherPlace(donnees_t donnees, gallerie_t gal)
{
  /* Affiche la place correspondante pour les deux joueurs */
  if (donnees.J1.c.tour > donnees.J2.c.tour)
  {
    afficherImage(gal.Pix[0], -519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
    afficherImage(gal.Pix[1], 519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
  }
  if (donnees.J1.c.tour < donnees.J2.c.tour)
  {
    afficherImage(gal.Pix[1], -519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
    afficherImage(gal.Pix[0], 519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
  }
  if (donnees.J1.c.tour == donnees.J2.c.tour)
  {
    if (donnees.J1.c.pass > donnees.J2.c.pass)
    {
      afficherImage(gal.Pix[0], -519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
      afficherImage(gal.Pix[1], 519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
    }
    if (donnees.J1.c.pass < donnees.J2.c.pass)
    {
      afficherImage(gal.Pix[1], -519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
      afficherImage(gal.Pix[0], 519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
    }
    if (donnees.J1.c.pass == donnees.J2.c.pass)
    {
      int i = donnees.J1.c.pass ;
      coord_t p ;
      if (i == 5)
	p = (coord_t){x: donnees.circuit.check.depart[0].x, y: (donnees.circuit.check.depart[0].y + donnees.circuit.check.depart[1].y)/2} ;
      else
        p = (coord_t){x: donnees.circuit.check.P[i][0].x, y: (donnees.circuit.check.P[i][0].y + donnees.circuit.check.P[i][1].y)/2} ;
      if (dist(donnees.J1.d.position, p) < dist(donnees.J2.d.position, p))
      {
        afficherImage(gal.Pix[0], -519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
        afficherImage(gal.Pix[1], 519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
      }
      else
      {
        afficherImage(gal.Pix[1], -519 - tn_largeur(gal.Pix[1])/2, 142 + tn_hauteur(gal.Pix[1])/2) ;
        afficherImage(gal.Pix[0], 519 - tn_largeur(gal.Pix[0])/2, 142 + tn_hauteur(gal.Pix[0])/2) ;
      }
    }		       
  } 
}

void afficherCircuit(donnees_t donnees, gallerie_t gal)
{
    /* Affiche le circuit sélectionné et les boites objets aux bonnes coordonnées */
    afficherImage(gal.Pix[29+donnees.id_circuit], - tn_largeur(gal.Pix[29+donnees.id_circuit])/2, tn_hauteur(gal.Pix[29+donnees.id_circuit])/2) ;
    afficherBoites(donnees, gal) ;
}

void afficherBoites(donnees_t donnees, gallerie_t gal)
{
  /* Affiche les boites objets en fonction du circuit choisi */
  for (int i = 0; i < 8; ++ i)
  {
    if (!(toucheObjetPiste(donnees.J1, donnees.circuit.items.Box[i], 12)) && !(toucheObjetPiste(donnees.J2, donnees.circuit.items.Box[i], 12)) && (donnees.circuit.items.timer[i] == 0))
    {
      int x = donnees.circuit.items.Box[i].x ;
      int y = donnees.circuit.items.Box[i].y ;
      afficherImage(gal.Pix[14], x - tn_largeur(gal.Pix[14])/2, y + tn_hauteur(gal.Pix[14])/2) ;
    }
  }
}

void afficherBananes(objets_t obj, gallerie_t gal)
{
  /* Affiche les bananes sur le circuit */
  for (int i = 0; i < obj.nb_banane; ++i)
    afficherImage(gal.Pix[15], obj.bananes[i].position.x - tn_largeur(gal.Pix[15])/2, obj.bananes[i].position.y + tn_hauteur(gal.Pix[15])/2) ;
}

void afficherBombes(objets_t * obj, gallerie_t gal)
{
  /* Affiche les bombes sur le circuit. L'image dépend du temps de présence de la bombe */
  for (int i = 0; i < obj->nb_bombe; ++i)
  {
    if ((obj->bombes[i].timer < 2.25) && (obj->bombes[i].detone == 0))
      afficherImage(gal.Pix[16], obj->bombes[i].position.x - tn_largeur(gal.Pix[16])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[16])/2) ;    
    if ((obj->bombes[i].timer >= 2.25) && (obj->bombes[i].timer < 3) && (obj->bombes[i].detone == 0))
    {
      afficherImage(gal.Pix[17], obj->bombes[i].position.x - tn_largeur(gal.Pix[17])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[17])/2) ;
    }
    if (obj->bombes[i].detone == 1)
      afficherExplosion(obj, i, gal) ;
  }
}

void afficherExplosion(objets_t * obj, int i, gallerie_t gal)
{
  /* Affiche une certaine frame en fonction de l'écart entre le temps actuel et le temps à l'explosion */
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.05) && (obj->bombes[i].timer > obj->bombes[i].tDetone)) 
    afficherImage(gal.Pix[18], obj->bombes[i].position.x - tn_largeur(gal.Pix[18])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[18])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.1) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.05)) 
    afficherImage(gal.Pix[19], obj->bombes[i].position.x - tn_largeur(gal.Pix[19])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[19])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.15) && (obj->bombes[i].timer >obj->bombes[i].tDetone + 0.10)) 
    afficherImage(gal.Pix[20], obj->bombes[i].position.x - tn_largeur(gal.Pix[20])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[20])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.20) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.15)) 
    afficherImage(gal.Pix[21], obj->bombes[i].position.x - tn_largeur(gal.Pix[21])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[21])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.25) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.20)) 
    afficherImage(gal.Pix[22], obj->bombes[i].position.x - tn_largeur(gal.Pix[22])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[22])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.30) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.25)) 
    afficherImage(gal.Pix[23], obj->bombes[i].position.x - tn_largeur(gal.Pix[23])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[23])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.35) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.30)) 
    afficherImage(gal.Pix[24], obj->bombes[i].position.x - tn_largeur(gal.Pix[24])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[24])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.4) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.35)) 
    afficherImage(gal.Pix[25], obj->bombes[i].position.x - tn_largeur(gal.Pix[25])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[25])/2) ;
  if ((obj->bombes[i].timer <= obj->bombes[i].tDetone + 0.45) && (obj->bombes[i].timer > obj->bombes[i].tDetone + 0.40))
    afficherImage(gal.Pix[26], obj->bombes[i].position.x - tn_largeur(gal.Pix[26])/2, obj->bombes[i].position.y + tn_hauteur(gal.Pix[26])/2) ; 
}

void afficherCarapaces(objets_t obj, gallerie_t gal)
{
  /* Affiche les carapaces bleues sur le circuit */
  for (int i = 0; i < obj.nb_carapace; ++i)
    afficherImage(gal.Pix[27], obj.carapaces[i].position.x - tn_largeur(gal.Pix[27])/2, obj.carapaces[i].position.y + tn_hauteur(gal.Pix[27])/2) ;
}

void afficherEffets(voiture_t J, gallerie_t gal)
{
  /* Affiche le viseur de la bombe et la confusion pour un joueur si les conditions sont réunies */
  double angle = (M_PI*J.d.r)/180 ;
  if ((J.statut.objet == 3) && (J.statut.effet != -1))
    afficherImage(gal.Pix[28], J.d.position.x + 120*cos(angle) - tn_largeur(gal.Pix[28])/2, J.d.position.y + 120*sin(angle) + tn_hauteur(gal.Pix[28])/2) ;
  if (J.statut.effet == 5)
    afficherImage(gal.Pix[29], J.d.position.x + - tn_largeur(gal.Pix[29])/2, J.d.position.y + tn_hauteur(gal.Pix[29])/2) ;
}

void afficherCourse(donnees_t donnees, gallerie_t gal)
{
    /* Affiche tous les éléments relatifs à la phase 5, entre autre le déroulement entier d'une course */
    afficherCircuit(donnees, gal) ;
    afficherBananes(donnees.objets, gal) ;
    afficherBombes(&(donnees.objets), gal) ;
    afficherCarapaces(donnees.objets, gal) ;
    afficherJoueur(donnees.J1, gal) ;
    afficherJoueur(donnees.J2, gal) ;
    afficherEffets(donnees.J1, gal) ;
    afficherEffets(donnees.J2, gal) ;
    afficherCadres(donnees, gal) ;
    afficherPlace(donnees, gal) ;
}

/******************************************/
/* Fonctions d'état du programme partie 2 */
/******************************************/

void ajustement_stats(voiture_t * J)
{
  /* Modifie les statistiques du joueur en fonction du personnage sélectionné */
  int modifs[12][3] = {{0, 0, 0}, {0, -8, 10}, {-4, 8, 0}, {0, 8, -10}, {4, 0, -10}, {4, -8, 0}, {-4, 0, 10}, {2, -4, 0}, {-2, 0, 5}, {-2, 4, 0}, {2, 0, -5}, {0, 4, -5}} ;
  for (int i = 0; i < 12; ++i)
  {
    if (J->id == i + 1)
      {
        J->stats.vitesse += modifs[i][0] ;
	J->stats.acceleration += modifs[i][1] ;
	J->stats.maniabilite += modifs[i][2] ;
	break ;
      }
  }		  
}

void repositionnement(donnees_t * data)
{
    /* Positionne les voitures en fonction du circuit sélectionné */
    if (data->id_circuit == 1)
    {
      data->J1.d.position = (coord_t){x: 56, y: -340} ;
      data->J2.d.position = (coord_t){x: 56, y: -365} ;
      data->J1.d.r = 180 ;
      data->J2.d.r = 180 ;
    }
    else
    {
      if (data->id_circuit == 2)
      {
        data->J1.d.position = (coord_t){x: 338, y: -40} ;
        data->J2.d.position = (coord_t){x: 370, y: -40} ;
      }
      if (data->id_circuit == 3)
      {
        data->J1.d.position = (coord_t){x: -333, y: -11} ;
        data->J2.d.position = (coord_t){x: -284, y: -11} ;
      }
      if (data->id_circuit == 4)
      {
	data->J1.d.position = (coord_t){x: -334, y: -12} ;
        data->J2.d.position = (coord_t){x: -316, y: -12} ;
      }
      data->J1.d.r = 90 ;
      data->J2.d.r = 90 ;
    }
}

int cap_vitesse(voiture_t J)
{
    /* Empêche un joueur de dépasser un seuil de vitesse */
    if (norme(J.d.v) > J.stats.vitesse)
        return 1 ;
    return 0 ;
}

voiture_t frein(voiture_t kart)
{
    /* Fixe la vitesse d'un joueur à 0 lorsque celle-ci est très petite */
    if (((norme(kart.d.v)) < kart.stats.vitesse/6) && (kart.phase != 0))
      {
          kart.phase = 0 ;
 	  kart.d.v = mult(0, kart.d.v) ;
	  kart.d.a = mult(0, kart.d.a) ;
      }
    return kart ;
}

void maj_mouvement(voiture_t * J, EtatSourisClavier esc, double dt, double angle)
{
    /* Décris le mouvement d'un kart en fonction des touches appuyées */
    int ordre[2] ;
    ordre[0] = 0 ;
    ordre[1] = 0 ;
  
    if ((esc.touchesClavier[J->stats.touches[0]]) && !(esc.touchesClavier[J->stats.touches[2]]))
    {
        ordre[0] = 1 ;
	ordre[1] = 1 ;
	J->f = 1 ;
    }

    if ((esc.touchesClavier[J->stats.touches[2]]) && !(esc.touchesClavier[J->stats.touches[0]]))
    {
        ordre[0] = -1 ;
	ordre[1] = -1 ;
	J->f = -1 ;
    }

    if ((esc.touchesClavier[J->stats.touches[2]]) && (esc.touchesClavier[J->stats.touches[0]]))
    {
        ordre[0] = J->f ;
        ordre[1] = -J->f ;
    }

    J->phase = ordre[1] ; /* la "2ème" touche pressée détermine la phase */

    if (J->phase == 1)
    {  
        J->d.a.x = J->stats.acceleration*cos(angle) ;
        J->d.a.y = J->stats.acceleration*sin(angle) ;
	if (J->d.w > 0)
	  J->d.w += dt*J->stats.acceleration ;
	else
	  J->d.w += dt*J->stats.acceleration ;
    }
    if (J->phase == -1)
    {
	J->d.a.x = -J->stats.acceleration*cos(angle) ;
        J->d.a.y = -J->stats.acceleration*sin(angle) ;
	if (J->d.w > 0)
	  J->d.w -= dt*J->stats.acceleration ;
	else
	  J->d.w -= dt*J->stats.acceleration ;
    }	

    if ((J->d.w > 0) && (J->d.w > J->stats.vitesse))
        J->d.w = J->stats.vitesse ;
    if ((J->d.w < 0) && (J->d.w < -J->stats.vitesse))
        J->d.w = -J->stats.vitesse ;
				 
    if (!(esc.touchesClavier[J->stats.touches[0]]) && !(esc.touchesClavier[J->stats.touches[2]]) && (norme(J->d.v) != 0))
    {
        J->f = 0 ;
        J->phase = 2 ;
    }  

    if (!(esc.touchesClavier[J->stats.touches[0]]) && !(esc.touchesClavier[J->stats.touches[2]]) && (J->phase != 0))
    {
      J->d.a = mult((-J->stats.acceleration*2)/norme(J->d.v), J->d.v) ;
      J->d.w *= pow(0.05, dt) ;
      *J = frein(*J) ;
    }
    if (esc.touchesClavier[J->stats.touches[1]])
    {
        J->d.r += J->stats.maniabilite*dt ;
    }

    if (esc.touchesClavier[J->stats.touches[3]])
    {
        J->d.r -= J->stats.maniabilite*dt ;
    }
}

void maj_position(donnees_t * data, double dt)
{
        data->J1.d.position = plus(data->J1.d.position,
  				 plus(mult(dt, data->J1.d.v),
				      mult(dt*dt/2, data->J1.d.a))) ;

        data->J2.d.position = plus(data->J2.d.position,
				 plus(mult(dt, data->J2.d.v),
				      mult(dt*dt/2, data->J2.d.a))) ;
}

void maj_vitesse(voiture_t * J, double dt, double angle)
{
    /* Mets à jour la vitesse d'un joueur */
    J->d.v = plus(J->d.v, mult(dt, J->d.a)) ;
    J->d.v.x = J->d.w*cos(angle) ; /* annulation du drift */
    J->d.v.y = J->d.w*sin(angle) ; /* annulation du drift */
    if (cap_vitesse(*J))      
        J->d.v = mult((J->stats.vitesse/norme(J->d.v)), J->d.v) ;
}  

void collision(voiture_t * J)
{
    /* Provoque une collision chez un joueur, l'empêchant de traverser les barrières */
    double angleJ = (M_PI*J->d.r)/180 ;
    if (J->d.w > 0)   
    {
      J->d.position.x += 6*cos(angleJ) ; 
      J->d.position.y += 6*sin(angleJ) ;
      J->d.position.x -= 8*cos(angleJ) ; 
      J->d.position.y -= 8*sin(angleJ) ;
    }
    if (J->d.w < 0)
    {
      J->d.position.x += -6*cos(angleJ) ; 
      J->d.position.y += -6*sin(angleJ) ;
      J->d.position.x -= -8*cos(angleJ) ; 
      J->d.position.y -= -8*sin(angleJ) ;
    }
    J->d.w = 0 ;
    J->d.v = mult(0, J->d.v) ;
}

void inverserTouches(voiture_t * J, stats_t stats)
{ 
    J->stats.touches[1] = stats.touches[3] ;
    J->stats.touches[3] = stats.touches[1] ;
}

void retablirTouches(voiture_t * J, stats_t stats)
{
    J->stats.touches[1] = stats.touches[1] ;
    J->stats.touches[3] = stats.touches[3] ;
}

donnees_t verificationObjet(donnees_t donnees, double dt)
{
    /* Joueur 1 */
    for (int i = 0; i < 8; ++i)
    {
      if ((toucheObjetPiste(donnees.J1, donnees.circuit.items.Box[i], 12)) && (donnees.circuit.items.timer[i] == 0))
      {
	donnees.circuit.items.timer[i] += 0.00001 ;
	if ((donnees.J1.statut.objet == 0) && (donnees.J1.statut.effet != 6))
            donnees.J1.statut.objet = rand() % 6 + 1 ;	  
      }      
      if (donnees.circuit.items.timer[i] != 0)
      {
        donnees.circuit.items.timer[i] += dt ;
        if (donnees.circuit.items.timer[i] > 2)
	  donnees.circuit.items.timer[i] = 0 ;
      }
    }
    if (donnees.J1.statut.objet != 0)
	  donnees.J1.statut.timer_objet += dt ;
    /* Joueur 2 */
    for (int i = 0; i < 8; ++i)
    {
      if ((toucheObjetPiste(donnees.J2, donnees.circuit.items.Box[i], 12)) && (donnees.circuit.items.timer[i] == 0))
      {
	donnees.circuit.items.timer[i] += 0.00001 ;
	if ((donnees.J2.statut.objet == 0) && (donnees.J1.statut.effet != 6))
            donnees.J2.statut.objet = rand() % 6 + 1 ;
      }
      if (donnees.circuit.items.timer[i] != 0 )
      {
        donnees.circuit.items.timer[i] += dt ;
        if (donnees.circuit.items.timer[i] > 1.5)
	  donnees.circuit.items.timer[i] = 0 ;
      }
    }
    if (donnees.J2.statut.objet != 0)
	  donnees.J2.statut.timer_objet += dt ;
    return donnees ;
}

int franchit_Depart(voiture_t kart, checkpoint_t check)
{
  /* Vérifie si un joueur se trouve dans la zone définissant la ligne de départ */
  if ((kart.d.position.x < check.depart[0].x) && (kart.d.position.x > check.depart[1].x) && (kart.d.position.y < check.depart[0].y) && (kart.d.position.y > check.depart[1].y))
     return 1 ;
  
  return 0 ;
}

int franchit_Zone(voiture_t kart, checkpoint_t check, int i)
{
  /* Vérifie si un joueur se trouve dans la zone définissant un point de passage */
  if ((kart.d.position.x < check.P[i][0].x) && (kart.d.position.x > check.P[i][1].x) && (kart.d.position.y < check.P[i][0].y) && (kart.d.position.y > check.P[i][1].y))
     return 1 ;
  return 0 ;
}

int toucheObjetPiste(voiture_t J, coord_t b, int distance)
{
  /* Vérifie si un joueur est en contact avec un objet du circuit */
  if (dist(J.d.position, b) <= distance)
    return 1;
  return 0 ;
}

void nouveau_Tour(voiture_t * J, checkpoint_t * zones)
{
  /* Mets à jour le nombre de points de passages franchit par un joueur et
     augmente le tour de 1 si tous les points de passage ont été validés */
  for (int i = 0; i < 5; ++i)
  {
    if ((J->c.pass == i) && (franchit_Zone(*J, *zones, i)))
      ++ J->c.pass ;
  }
  if ((J->c.pass == 5) && (franchit_Depart(*J, *zones)))
  {
    ++ J->c.tour ;
    J->c.pass = 0 ;
  }
}

void detectionPiste(voiture_t * J, double angle, stats_t statsInit)
{	
        int r, v, b ; /* variables contenant les composantes de la couleur au centre de la position du joueur */
      
	lirePixel((int)(J->d.position.x), (int)(J->d.position.y), &r, &v, &b) ;
	if ((r == 152) && (v = 152) && (b == 160)) /* Détection du semi hors-piste */
	{
	    J->stats.vitesse = statsInit.vitesse*0.7 ;
	    J->stats.acceleration = statsInit.acceleration*0.7 ;
	    J->stats.maniabilite = statsInit.maniabilite*0.7 ;
	}
	else
	{
	  if ((r == 8) && (v = 120) && (b == 24)) /* Détection du hors-piste */
	  {
	     J->stats.vitesse = statsInit.vitesse*0.4 ;
	     J->stats.acceleration = statsInit.acceleration*0.4 ;
	     J->stats.maniabilite = statsInit.maniabilite*0.8 ;
	  }
	  else
	  {
            J->stats.vitesse = statsInit.vitesse ;
	    J->stats.acceleration = statsInit.acceleration ;
	    J->stats.maniabilite = statsInit.maniabilite ;
	  }
	}
	
	if (J->d.w > 0)
	{
	  for (int k = 6; k >= 0; k = k - 1) /* On boucle pour détecter les collisions sur plusieurs pixels */
          {
	    lirePixel((int)(J->d.position.x+k*cos(angle)), (int)(J->d.position.y+k*sin(angle)), &r, &v, &b) ;
	    if (((r == 0) && (v == 0) && (b == 2)) || ((r == 16) && (v == 112) && (b == 240)))
	    {
	        J->stats.acceleration = statsInit.acceleration*0.25 ;
	        collision(J) ; 
 	    }
          }
	}
	if (J->d.w < 0)
	{
	  for (int k = -6; k <= 0; k = k + 1)
          {
	    lirePixel((int)(J->d.position.x+k*cos(angle)), (int)(J->d.position.y+k*sin(angle)), &r, &v, &b) ;
	    if (((r == 0) && (v == 0) && (b == 2)) || ((r == 16) && (v == 112) && (b == 240)))
	    {
	        J->stats.acceleration = statsInit.acceleration*0.25 ;
	        collision(J) ; 
 	    }
          }
	}
}

void utiliserObjetJ1(donnees_t * donnees, double dt, EtatSourisClavier esc, stats_t stats1, stats_t stats2)
{
  consommerObjetJ1(donnees, esc) ;
  if (donnees->J1.statut.objet == 0)
    donnees->J1.statut.timer_objet = 0 ;
  if (donnees->J1.statut.effet == 1)
  {
    donnees->J1.stats.vitesse = stats1.vitesse*1.5 ;
    donnees->J1.stats.acceleration = stats1.acceleration*10 ;
    donnees->J1.statut.temps += dt ;
    if (donnees->J1.statut.temps >= 1.5)
    {
      donnees->J1.statut.effet = 0 ;
      donnees->J1.statut.temps = 0 ;
    }
  }
  if (donnees->J1.statut.effet == 5)
  {
    if (donnees->J1.statut.temps == 0)
    {
      inverserTouches(&(donnees->J1), stats1) ;
    }
    donnees->J1.statut.temps += dt ;
    if (donnees->J1.statut.temps >= 3)
    {
      donnees->J1.statut.effet = 0 ;
      retablirTouches(&(donnees->J1), stats1) ;
      donnees->J1.statut.temps = 0 ;
    }
  }
  if (donnees->J1.statut.effet == 6)
  {
    donnees->J1.statut.temps += dt ;
    if (donnees->J1.statut.temps >= 0.5)
    {
      donnees->J1.statut.effet = 0 ;
      donnees->J1.statut.temps = 0 ;
      donnees->J1.statut.objet = rand() % 5 + 1 ;
    }
  }
  if (donnees->J1.statut.effet == -1)
  {
    retablirTouches(&(donnees->J1), stats1) ;
    animationTouche(&(donnees->J1), dt, donnees->J1.statut.angle_initial) ;
  }
  else
    donnees->J1.statut.angle_initial = donnees->J1.d.r ;
}

void utiliserObjetJ2(donnees_t * donnees, double dt, EtatSourisClavier esc, stats_t stats1, stats_t stats2)
{
  consommerObjetJ2(donnees, esc) ;
  if (donnees->J2.statut.objet == 0)
    donnees->J2.statut.timer_objet = 0 ;
  if (donnees->J2.statut.effet == 1)
  {
    donnees->J2.stats.vitesse = stats1.vitesse*1.5 ;
    donnees->J2.stats.acceleration = stats1.acceleration*10 ;
    donnees->J2.statut.temps += dt ;
    if (donnees->J2.statut.temps >= 1.5)
    {
      donnees->J2.statut.effet = 0 ;
      donnees->J2.statut.temps = 0 ;
    }
  }
  if (donnees->J2.statut.effet == 5)
  {
    if (donnees->J2.statut.temps == 0)
    {
      inverserTouches(&(donnees->J2), stats2) ;
    }
    donnees->J2.statut.temps += dt ;
    if (donnees->J2.statut.temps >= 3)
    {
      donnees->J2.statut.effet = 0 ;
      retablirTouches(&(donnees->J2), stats2) ;
      donnees->J2.statut.temps = 0 ;
    }
  }
  if (donnees->J2.statut.effet == 6)
  {
    donnees->J2.statut.temps += dt ;
    if (donnees->J2.statut.temps >= 0.5)
    {
      donnees->J2.statut.effet = 0 ;
      donnees->J2.statut.temps = 0 ;
      donnees->J2.statut.objet = rand() % 5 + 1 ;
    }
  }
  if (donnees->J2.statut.effet == -1)
  {
    retablirTouches(&(donnees->J2), stats2) ;
    animationTouche(&(donnees->J2), dt, donnees->J2.statut.angle_initial) ;
  }
  else
    donnees->J2.statut.angle_initial = donnees->J2.d.r ;
}

void consommerObjetJ1(donnees_t * donnees, EtatSourisClavier esc)
{
  if ((donnees->J1.statut.objet == 1) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.timer_objet > 1))
  {
    donnees->J1.statut.effet = 1 ;
    donnees->J1.statut.objet = 0 ;
  }
  if ((donnees->J1.statut.objet == 2) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.timer_objet > 1))
  {
    placerBanane(donnees->J1, &(donnees->objets)) ;
    donnees->J1.statut.objet = 0 ;
  }
  if ((donnees->J1.statut.objet == 3) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.timer_objet > 1))
  {
    placerBombe(donnees->J1, &(donnees->objets), esc) ;
    donnees->J1.statut.objet = 0 ;
  }
  if ((donnees->J1.statut.objet == 4) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.timer_objet > 1))
  {
    lancerCarapaceJ1(donnees) ;
    donnees->J1.statut.objet = 0 ;
  }
  if ((donnees->J1.statut.objet == 5) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J2.statut.effet != 5) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.effet != 1) && (donnees->J2.statut.effet != 6) && (donnees->J1.statut.timer_objet > 1))
  {
    donnees->J1.statut.objet = 0 ;
    donnees->J2.statut.effet = 5 ;
  }
  if ((donnees->J1.statut.objet == 6) && (esc.touchesClavier[donnees->J1.stats.touches[4]]) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.timer_objet > 1))
  {
    donnees->J1.statut.objet = 0 ;
    donnees->J1.statut.effet = 6 ;
  }
}

void consommerObjetJ2(donnees_t * donnees, EtatSourisClavier esc)
{
  if ((donnees->J2.statut.objet == 1) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.effet != 5) && (donnees->J2.statut.timer_objet > 1))
  {
    donnees->J2.statut.effet = 1 ;
    donnees->J2.statut.objet = 0 ;
  }
  if ((donnees->J2.statut.objet == 2) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.timer_objet > 1))
  {
    placerBanane(donnees->J2, &(donnees->objets)) ;
    donnees->J2.statut.objet = 0 ;
  }
  if ((donnees->J2.statut.objet == 3) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.timer_objet > 1))
  {
    placerBombe(donnees->J2, &(donnees->objets), esc) ;
    donnees->J2.statut.objet = 0 ;
  }
  if ((donnees->J2.statut.objet == 4) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.timer_objet > 1))
  {
    lancerCarapaceJ2(donnees) ;
    donnees->J2.statut.objet = 0 ;
  }
  if ((donnees->J2.statut.objet == 5) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J1.statut.effet != 5) && (donnees->J1.statut.effet != -1) && (donnees->J1.statut.effet != 1) && (donnees->J1.statut.effet != 6) && (donnees->J2.statut.timer_objet > 1))
  {
    donnees->J2.statut.objet = 0 ;
    donnees->J1.statut.effet = 5 ;
  }
  if ((donnees->J2.statut.objet == 6) && (esc.touchesClavier[donnees->J2.stats.touches[4]]) && (donnees->J2.statut.effet != -1) && (donnees->J2.statut.timer_objet > 1))
  {
    donnees->J2.statut.objet = 0 ;
    donnees->J2.statut.effet = 6 ;
  }
}

void placerBanane(voiture_t J, objets_t * obj)
{
    double angle = (M_PI*J.d.r)/180 ;
    coord_t point = {x: J.d.position.x - 16*cos(angle), y: J.d.position.y - 16*sin(angle)} ;
    obj->bananes[obj->nb_banane].position = point ;
    obj->bananes[obj->nb_banane].timer = 0 ;
    obj->nb_banane += 1 ;
}

void placerBombe(voiture_t J, objets_t * obj, EtatSourisClavier esc)
{
    double angle = (M_PI*J.d.r)/180 ;
    coord_t point = {x: J.d.position.x - 20*cos(angle), y: J.d.position.y - 20*sin(angle)} ;
    if (esc.touchesClavier[J.stats.touches[2]])
    {
      obj->bombes[obj->nb_bombe].position = point ;
      obj->bombes[obj->nb_bombe].direction = -1 ;
    }
    else
    {
      obj->bombes[obj->nb_bombe].position = (coord_t){x: point.x + 120*cos(angle), y: point.y + 120*sin(angle)} ;
      obj->bombes[obj->nb_bombe].direction = 1 ;
    }
    obj->bombes[obj->nb_bombe].timer = 0 ;
    obj->bombes[obj->nb_bombe].detone = 0 ;
    obj->nb_bombe += 1 ;
}

void lancerCarapaceJ1(donnees_t * donnees)
{
    donnees->objets.carapaces[donnees->objets.nb_carapace].position = donnees->J1.d.position ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].lanceur = 1 ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].vitesse = 80 ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].timer = 0 ;
    donnees->objets.nb_carapace += 1 ;
}

void lancerCarapaceJ2(donnees_t * donnees)
{
    donnees->objets.carapaces[donnees->objets.nb_carapace].position = donnees->J2.d.position ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].lanceur = 2 ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].vitesse = 80 ;
    donnees->objets.carapaces[donnees->objets.nb_carapace].timer = 0 ;
    donnees->objets.nb_carapace += 1 ;
}

void retirerBanane(objets_t * objets, int indice)
{
    for (int i = indice; i < objets->nb_banane - 1; ++i)
    {
        objets->bananes[i] = objets->bananes[i+1] ;
    }
    objets->nb_banane -= 1 ;
}

void retirerCarapace(objets_t * objets, int indice)
{
    for (int i = indice; i < objets->nb_carapace - 1; ++i)
    {
        objets->carapaces[i] = objets->carapaces[i+1] ;
    }
    objets->nb_carapace -= 1 ;
}

void retirerBombe(objets_t * objets, int indice)
{
    for (int i = indice; i < objets->nb_bombe - 1; ++i)
    {
        objets->bombes[i] = objets->bombes[i+1] ;
    }
    objets->nb_bombe -= 1 ;
}

void actualisation_bananes(donnees_t * donnees, double dt)
{
  for (int i = 0; i < donnees->objets.nb_banane ; ++i)
  {
    donnees->objets.bananes[i].timer += dt ;
    if (donnees->objets.bananes[i].timer >= 60)   
        retirerBanane(&(donnees->objets), i) ;
    if ((toucheObjetPiste(donnees->J1, donnees->objets.bananes[i].position, 12)) && (donnees->J1.statut.effet != -1))
    {
        donnees->J1.statut.effet = -1 ;
        retirerBanane(&(donnees->objets), i) ;
    }
    if ((toucheObjetPiste(donnees->J2, donnees->objets.bananes[i].position, 12)) && (donnees->J1.statut.effet != -1))
    {
        donnees->J2.statut.effet = -1 ;
        retirerBanane(&(donnees->objets), i) ;
    }
  }
}

void actualisation_bombes(donnees_t * donnees, double dt)
{
  for (int i = 0; i < donnees->objets.nb_bombe ; ++i)
  {
      donnees->objets.bombes[i].timer += dt ;
      if ((donnees->objets.bombes[i].timer >= 3) && (donnees->objets.bombes[i].detone == 0))
      {
	  donnees->objets.bombes[i].detone = 1 ;
	  donnees->objets.bombes[i].tDetone  = donnees->objets.bombes[i].timer ;
      }
      if ((donnees->objets.bombes[i].timer <= donnees->objets.bombes[i].tDetone + 0.45) && (donnees->objets.bombes[i].timer > donnees->objets.bombes[i].tDetone + 0.40) && (donnees->objets.bombes[i].detone == 1))
      {
	  donnees->objets.bombes[i].detone = -1 ;
      }
      if (donnees->objets.bombes[i].direction == 1)
      {
        int r, v, b ;
	lirePixel(donnees->objets.bombes[i].position.x, donnees->objets.bombes[i].position.y, &r, &v, &b) ;
	if ((((r == 0) && (v == 0) && (b == 2)) || ((r == 16) && (v == 112) && (b == 240))) && ((donnees->objets.bombes[i].detone == 0))) 
	{
	    donnees->objets.bombes[i].detone = 1 ;
	    donnees->objets.bombes[i].tDetone  = donnees->objets.bombes[i].timer ;
	}
	if ((donnees->objets.bombes[i].position.x > 600) || (donnees->objets.bombes[i].position.x < -600) || (donnees->objets.bombes[i].position.y > 400) || (donnees->objets.bombes[i].position.y < -400))
	{
            donnees->objets.bombes[i].detone = -1 ;
	}
      }
      if ((toucheObjetPiste(donnees->J1, donnees->objets.bombes[i].position, 12)) && (donnees->objets.bombes[i].detone == 0) && (donnees->J1.statut.effet != -1))
      {
        donnees->J1.statut.effet = -1 ;
	donnees->objets.bombes[i].detone = 1 ;
	donnees->objets.bombes[i].tDetone  = donnees->objets.bombes[i].timer ;
      }
      if ((toucheObjetPiste(donnees->J2, donnees->objets.bombes[i].position, 12)) && (donnees->objets.bombes[i].detone == 0) && (donnees->J2.statut.effet != -1))
      {
        donnees->J2.statut.effet = -1 ;
        donnees->objets.bombes[i].detone = 1 ;
	donnees->objets.bombes[i].tDetone  = donnees->objets.bombes[i].timer ;
      }
      if ((toucheObjetPiste(donnees->J1, donnees->objets.bombes[i].position, 80)) && (donnees->objets.bombes[i].detone == 1) && (donnees->J1.statut.effet != -1))
      {
        donnees->J1.statut.effet = -1 ;
      }
      if ((toucheObjetPiste(donnees->J2, donnees->objets.bombes[i].position, 80)) && (donnees->objets.bombes[i].detone == 1) && (donnees->J2.statut.effet != -1))
      {
        donnees->J2.statut.effet = -1 ;
      }
      if (donnees->objets.bombes[i].detone == -1)
	retirerBombe(&(donnees->objets), i) ;
  }
}

void actualisation_carapaces(donnees_t * donnees, double dt)
{
    for (int i = 0; i < donnees->objets.nb_carapace ; ++i)
    {
      donnees->objets.carapaces[i].timer += dt ;
      boost_vitesse_carapace(donnees, i) ;
      coord_t cible ;
      if (donnees->objets.carapaces[i].timer >= 60)   
          retirerCarapace(&(donnees->objets), i) ;
      if (donnees->objets.carapaces[i].lanceur == 1)
      {
	cible = donnees->J2.d.position ;
	double angle = calculAngle(donnees->objets.carapaces[i].position, cible) ;
	donnees->objets.carapaces[i].position.x += donnees->objets.carapaces[i].vitesse*cos(angle)*dt ;
	donnees->objets.carapaces[i].position.y += donnees->objets.carapaces[i].vitesse*sin(angle)*dt ;
	if ((toucheObjetPiste(donnees->J2, donnees->objets.carapaces[i].position, 12)))
        {
          if (donnees->J2.statut.effet != -1)
	      donnees->J2.statut.effet = -1 ;
          retirerCarapace(&(donnees->objets), i) ;
        }
      }
      if (donnees->objets.carapaces[i].lanceur == 2)
      {
	cible = donnees->J1.d.position ;
	double angle = calculAngle(donnees->objets.carapaces[i].position, cible) ;
	donnees->objets.carapaces[i].position.x += donnees->objets.carapaces[i].vitesse*cos(angle)*dt ;
	donnees->objets.carapaces[i].position.y += donnees->objets.carapaces[i].vitesse*sin(angle)*dt ;
	if ((toucheObjetPiste(donnees->J1, donnees->objets.carapaces[i].position, 12)))
        {
          if (donnees->J1.statut.effet != -1)
	      donnees->J1.statut.effet = -1 ;
          retirerCarapace(&(donnees->objets), i) ;
        }
      }
    }
}

void boost_vitesse_carapace(donnees_t * donnees, int indice)
{
  if ((donnees->objets.carapaces[indice].timer <= 10) && (donnees->objets.carapaces[indice].timer > 5))
      donnees->objets.carapaces[indice].vitesse = 100 ;
  if ((donnees->objets.carapaces[indice].timer <= 15) && (donnees->objets.carapaces[indice].timer > 10))
      donnees->objets.carapaces[indice].vitesse = 120 ;
  if (donnees->objets.carapaces[indice].timer >= 20)
      donnees->objets.carapaces[indice].vitesse = 140 ;
}

void animationTouche(voiture_t * J, double dt, double angle_stock)
{
    J->stats.maniabilite = 0 ;
    J->statut.temps += dt ;
    J->d.v = mult(0, J->d.v) ;
    J->d.a = mult(0, J->d.a) ;
    J->d.w = 0 ;
    J->d.r += 720*dt ;
    if (J->statut.temps >= 2)
    {
      J->statut.effet = 0 ;
      J->statut.temps = 0 ;
      J->d.r = angle_stock ;
    }
}

 donnees_t maj_donnees(donnees_t donnees, EtatSourisClavier esc, double dt, stats_t stats1, stats_t stats2) 
{
    double angleJ1 = (M_PI*donnees.J1.d.r)/180 ;
    double angleJ2 = (M_PI*donnees.J2.d.r)/180 ;	
    maj_mouvement(&(donnees.J1), esc, dt, angleJ1) ;
    maj_mouvement(&(donnees.J2), esc, dt, angleJ2) ;
    maj_position(&(donnees), dt) ; // Mise à jour de la position des deux joueurs
    maj_vitesse(&(donnees.J1), dt, angleJ1) ;
    maj_vitesse(&(donnees.J2), dt, angleJ2) ;

    /* Mise à jour de variables caractérstiques des joueurs en fonction de leur position sur la piste */

    detectionPiste(&(donnees.J1), angleJ1, stats1) ;
    detectionPiste(&(donnees.J2), angleJ2, stats2) ;

    /* Vérification de l'obtention de de l'utilisation d'objets pour les deux joueurs */
    
    donnees = verificationObjet(donnees, dt) ;
    utiliserObjetJ1(&(donnees), dt, esc, stats1, stats2) ;
    utiliserObjetJ2(&(donnees), dt, esc, stats1, stats2) ;

    /* Mise à jour de l'état des bananes et des bombes sur le circuit */

    actualisation_bananes(&(donnees), dt) ;
    actualisation_bombes(&(donnees), dt) ;
    actualisation_carapaces(&(donnees), dt) ;
    
    /* Mise à jour de l'état de la course pour les deux joueurs */
	
    nouveau_Tour(&(donnees.J1), &(donnees.circuit.check)) ;
    nouveau_Tour(&(donnees.J2), &(donnees.circuit.check)) ;
	
    if ((donnees.J1.c.tour == 3) || (donnees.J2.c.tour == 3))
    {
       donnees.etat = 6 ;
       if (donnees.J1.c.tour == 3)
	 donnees.gagnant = 1 ;
       else
	  donnees.gagnant = 2 ;
    }
    
    return donnees ;
}
