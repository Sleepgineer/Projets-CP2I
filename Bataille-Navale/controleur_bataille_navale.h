/* _____________________________________________________
	Contrôleur du jeu de la bataille navale : fichier d'entête

	Les pré-conditions d'une fonction listent les conditions que ses arguments se doivent de vérifier à tout appel de la fonction : c'est le programme appelant et non la fonction elle-même qui est en charge de vérifier que ces conditions sont vérifiées.

	Les post-conditions d'une fonction listent les conditions que ses arguments se doivent de vérifier à l'issue de l'appel à la fonction : c'est la fonction elle-même qui est en charge de s'assurer que ces conditions sont vérifiées.	
	La valeur retour éventuelle d'une fonction décrit ce que renvoie la fonction, à savoir le type et l'utilité de la variable renvoyée.
_____________________________________________________ */

#ifndef CTRL_H

#define CTRL_H

/* _______________________________
	Structure de données			*/
	
/* ctrl_t est caractérisé par :
- un champ contenant le modèle de la bataille navale
- un champ contenant la vue graphique de la bataille navale
- un champ contenant un type structuré référençant des entiers non signés et l'adresse du contrôleur
- un champ contenant un entier non signé indiquant l'état du programme
- un champ contenant un type structuré sigaction, afin de transmettre des signaux convenablement
*/

/* Définition des types structurés */

/* Déclaration du type structuré du contrôleur */
typedef struct ctrl_s ctrl_t ;

/* Type structuré case_t permettant un autre accès au contrôleur ainsi qu'à d'autres données cruciales */
typedef struct 
{
ctrl_t * controleur; // adresse du contrôleur
unsigned int lig ; // entier non signé indiquant un numéro de ligne dans la grille de jeu
unsigned int col ; // entier non signé indiquant un numéro de colonne dans la grille de jeu
}
case_t ;

/* Définition du type structuré du contrôleur */
struct ctrl_s 
{
jeu * modele; // adresse d'un jeu, qui contient l'ensemble du modèle de la bataille navale
vue_t * vue; // adresse d'une vue graphique complète de la bataille navale
case_t id[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT]; // type structuré permettant d'accéder au contrôleur tout en retenant des données importantes
unsigned int etat_prog; // entier non signé désignant l'état du programme
struct sigaction* handler_interruption; // type structuré destiné aux signaux
} ;


/* _______________________________
	Fonctions de manipulation du controleur	*/

/*	Pre-conditions 	: - c adresse d'un controleur
                          - v adresse d'une vue
	Post-conditions : tous les champs du controleur sont initialisés, en particulier le jeu et la vue graphique qui 
	                  sont intégralement initialisés et construits
*/
void controleur_init_bataille(ctrl_t * c, vue_t * v) ;

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : le label de la fenetre de jeu indiquant le tour du joueur est mis à jour selon l'état de la partie
*/
void controleur_afficher_tour(ctrl_t * c) ; 

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : agit sur la fenetre de jeu en faisant les tâches suivantes :
	                  - met à jour les images affichées par les deux grilles de jeu
	                  - désactive tous les boutons de la grille adversaire si c'est le tour de l'adversaire
	                  - active tous les boutons sauf ceux qui ont déjà été cliqués auparavant si c'est le tour du joueur
	NB : Bug connu : les cases "touché" de la grille adverse ne se mettent pas toutes à jour lorsqu'un bateau coule. 
	Ce léger bug visuel provient soit de la fonction controleur_afficher_case_adversaire, soit de l'organisation de la structure 
	de la vue, plus précisément pour la fenetre de jeu
*/
void controleur_afficher_grilles(ctrl_t * c) ;

/*	Pre-conditions 	: - c adresse d'un controleur construit entièrement
                          - x entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : modifie sur la fenetre de jeu l'image de la case de la grille du joueur de coordonnées (x;y) selon l'état de la case et son contenu
*/
void controleur_afficher_case_joueur(ctrl_t * c, unsigned int x, unsigned int y) ;

/*	Pre-conditions 	: - c adresse d'un controleur construit entièrement
                          - x entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : modifie sur la fenetre de jeu l'image de la case de la grille de l'adversaire de coordonnées (x;y) selon l'état de la case et son contenu
*/
void controleur_afficher_case_adversaire(ctrl_t * c, unsigned int x, unsigned int y) ;

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : met à jour sur la fenetre de jeu le nombre de parties gagnées et perdues
*/
void controleur_afficher_nb_wl(ctrl_t * c) ;

/// Fonctions de rappel ///

/*	Pre-conditions 	: - b adresse d'un widget
                          - c adresse d'un controleur construit entièrement
	Post-conditions : récupère le pseudo du joueur rentré dans la zone de saisie dédiée et le renseigne dans les données du jeu lorsque la fonction est appelée.
	En conséquence, la fenetre de connexion est mise à jour pour afficher le pseudo rentré en modifiant un label. Si rien n'est renseigné dans la zone de saisie,
	le pseudo du joueur sera STR_JOUEUR_PSEUDO_DEFAULT
*/
void controleur_definir_pseudo(GtkWidget* b, ctrl_t * c) ;

/*	Pre-conditions 	: - b adresse d'un widget
                          - event est de type GdkEventButton* est vaut explicitement "button-press-event"
                          - casee adresse d'une variable de type case_t dont le champ controleur pointe vers un controleur entièrement construit
	Post-conditions : permet d'effectuer un tour pour le joueur, en envoyant les données du coup joué à l'adversaire. Si suite à cela la partie est gagnée ou
	perdue, on bascule vers la fenetre de fin de partie et on affiche la bonne image selon l'issue de la partie.
	Quoi qu'il advienne, l'interface graphique est mise à jour lorsque l'on quitte cette fonction de rappel.
	Valeur retour : booleen FALSE
*/
gboolean controleur_attaquer(GtkWidget* b, GdkEventButton* event, case_t * casee) ; 

/*	Pre-conditions 	: - b adresse d'un widget
                          - event est de type GdkEventButton* est vaut explicitement "button-release-event"
                          - casee adresse d'une variable de type case_t dont le champ controleur pointe vers un controleur entièrement construit
	Post-conditions : permet d'effectuer un tour pour l'adversaire, en recevant les données envoyées par le joueur, à savoir le coup joué. Si suite à cela la partie est gagnée ou
	perdue, on bascule vers la fenetre de fin de partie et on affiche la bonne image selon l'issue de la partie.
	Quoi qu'il advienne, l'interface graphique est mise à jour lorsque l'on quitte cette fonction de rappel.
	Valeur retour : booleen FALSE
*/
gboolean controleur_recevoir(GtkWidget* b, GdkEventButton* event, case_t * casee) ; 

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le mode client est initialisé et la fenetre de connexion est mise à jour afin d'y faire figurer les instructions de connexion
*/
void controleur_partie_client(GtkWidget* b, ctrl_t * controleur) ;

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le mode serveur est initialisé et la fenetre de connexion est mise à jour afin d'y faire figurer les instructions de connexion
*/
void controleur_partie_serveur(GtkWidget* b, ctrl_t * controleur) ;

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : établit la connexion. Lorsque les deux joueurs sont connectés, la partie est initialisée et on bascule sur la fenetre de jeu.
	                  Si la connexion a échoué, la boite de dialogue de la fenetre de connexion s'ouvre pour préciser que la connexion a échoué.
*/
void controleur_connexion(GtkWidget* b, ctrl_t * controleur) ;

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : relance une partie en basculant sur la fenetre de jeu, en mettant à jour le nombre de victoires/défaites et l'état du programme.
*/
void controleur_rejouer(GtkWidget* b, ctrl_t * controleur) ;

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le joueur se déconnecte et le programme se ferme.
*/
void controleur_quitter(GtkWidget* b, ctrl_t * controleur) ;

/// Fonction appelée à chaque itération de boucle gtk via g_idle_add ///

/*	Pre-conditions 	: - data pointe vers les données d'un controleur
	Post-conditions : - si c'est le tour de l'adversaire, on assure la réception des données et le bon déroulement de la partie
	                  - si la partie est gagnée/perdue, on assure le passage à la fenetre de fin de partie et on met à jour l'état du programme. 
*/
gboolean controleur_assurage_reception(gpointer data) ;

/* _______________________________
	Accesseurs en lecture	*/

/*	Pre-conditions 	: ctrl est l'adresse d'un controleur construit entièrement
	Post-conditions : aucune
	Valeur retour : adresse d'un jeu
*/
jeu* controleur_get_jeu(ctrl_t ctrl) ;

/*	Pre-conditions 	: ctrl est l'adresse d'un controleur construit entièrement
	Post-conditions : aucune
	Valeur retour : adresse d'une vue graphique
*/
vue_t * controleur_get_vue(ctrl_t ctrl) ;

/*	Pre-conditions 	: - ctrl est l'adresse d'un controleur construit entièrement
                          - x est un entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y est un entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : aucune
	Valeur retour : adresse d'un type structuré de type case_t appartenant au controleur (dépend de (x;y)) 
	N.B : inutilisé car provoque une erreur
*/
case_t controleur_get_case(ctrl_t ctrl, unsigned int x, unsigned int y) ;

#endif
	
	
