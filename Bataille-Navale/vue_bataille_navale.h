/* _____________________________________________________
	Interface graphique du jeu de la bataille navale : fichier d'entête

	Les pré-conditions d'une fonction listent les conditions que ses arguments se doivent de vérifier à tout appel de la fonction : c'est le programme appelant et non la fonction elle-même qui est en charge de vérifier que ces conditions sont vérifiées.

	Les post-conditions d'une fonction listent les conditions que ses arguments se doivent de vérifier à l'issue de l'appel à la fonction : c'est la fonction elle-même qui est en charge de s'assurer que ces conditions sont vérifiées.	
	La valeur retour éventuelle d'une fonction décrit ce que renvoie la fonction, à savoir le type et l'utilité de la variable renvoyée.
_____________________________________________________ */

#ifndef GRAPH_H

#define GRAPH_H

/* _______________________________
	Structure de données			*/
	
/* vue_t est caractérisée par 3 interfaces distinctes, à savoir:
- un ensemble de widgets (dont la fenetre) finissant par _jeu. Tous sont inclus dans l'interface du jeu
- un ensemble de widgets (dont la fenetre) finissant par _co. Tous sont inclus dans l'interface de connexion
- un ensemble de widgets (dont la fenetre) finissant par _wl. Tous sont inclus dans l'interface de fin de partie
*/

/* Type structuré définissant la vue graphique dans son ensemble */

typedef struct 
{
/* Ensemble des widgets de la fenetre de jeu */
GtkWindow* f_jeu; // fenetre de jeu
GtkBox* bp_jeu; // boite principale de la fenetre de jeu
GtkBox* b1_jeu; // boite n°1 de la fenetre de jeu
GtkBox* b2_jeu; // boite n°2 de la fenetre de jeu
GtkBox* b3_jeu; // boite n°3 de la fenetre de jeu
GtkLabel* donnees_jeu[3]; // tableau statique de labels relatifs aux données d'une partie. Indices --> 0 : pseudo adverse ; 1 : nombre de victoires ; 2 : nombre de défaites
GtkLabel* etat_tour_jeu ; // label indiquant le tour de jeu actuel
GtkLabel* camp_jeu[2] ; // tableau statique de labels précisant la formation du jeu. Indices --> 0 : texte formation du joueur ; 1 : texte formation de l'adversaire
GtkGrid* grille_jeu[2]; // tableau statique de grilles permettant de représenter l'état de la partie. Indices --> 0 : grille du joueur ; 1 : grille de l'adversaire
GtkButton* cases_adv_jeu[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT]; // grille de boutons destinés à figurer dans la grille de l'adversaire
GtkImage* cases_j_jeu[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT]; // grille d'images destinés à figurer dans la grille du joueur
GtkImage* im_cases_adv_jeu[JOUEUR_GRILLE_L_DEFAULT][JOUEUR_GRILLE_H_DEFAULT]; // grille d'images destinés à figurer dans les boutons présents dans la grille de l'adversaire

/* Ensemble des widgets de la fenetre de connexion */

GtkWindow* f_co; // fenetre de connexion
GtkBox* bp_co; // boite principale de la fenetre de connexion
GtkBox* b1_co; // boite n°1 de la fenetre de connexion
GtkBox* b2_co; // boite n°2 de la fenetre de connexion
GtkLabel* texte_co[4]; // tableau statique de labels contenus dans la fenetre de connexion. Indices --> 0 : choix pseudo ; 1 : choix mode connexion ; 2 : message rejoindre via nom ; 3 : message rejoindre via IP
GtkButton* mode_co[2]; // tableau statique de boutons permettant de choisir un mode de connexion. Indices --> 0 : Client ; 1 : Serveur 
GtkEntry* saisie_co[3]; // tableau statique de zones de saisie contenues dans la fenetre de connexion. Indices --> 0 : barre de saisie pseudo ; 1 : barre de saisie nom d'hôte ; 2 : barre de saisie IP hôte 
GtkButton* bouton_pseudo_co; // bouton de la fenetre de connexion qui permet de valider le pseudo saisi
GtkButton* bouton_co; // bouton de la fenetre de connexion qui permet d'établir la connexion
GtkDialog* invalide_co ; // boite de dialogue de la fenetre de connexion

/* Ensemble des widgets de la fenetre de victoire/defaite */
GtkWindow* f_wl; // fenetre de fin de partie
GtkBox* bp_wl ; // boite principale de la fenetre de fin de partie
GtkBox* b1_wl ; // boite n°1 de la fenetre de fin de partie
GtkImage* im_wl ; // image de la fenetre de fin de partie
GtkButton* rejouer_wl ; // bouton de la fenetre de fin de partie permettant de rejouer
GtkButton* quitter_wl ; // bouton de la fenetre de fin de partie permettant de quitter le programme
} 
vue_t ;


/* _______________________________
	Fonctions de manipulation de l'interface graphique	*/
	
/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de la connexion (les champs finissant par _co) sont initialisés à NULL
*/
void init_vue_co(vue_t * v) ;

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de jeu (les champs finissant par _jeu) sont initialisés à NULL
*/
void init_vue_jeu(vue_t * v) ;

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de fin de partie (les champs finissant par _wl) sont initialisés à NULL
*/
void init_vue_wl(vue_t * v) ;

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs de la vue sont initialisés à NULL
*/
void init_vue(vue_t * v) ;

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de connexion sont construits et imbriqués de sorte à construire la fenêtre de connexion.
	On affiche aussi tous les widgets la fenêtre de connexion car ce sera la première fenêtre à être visible au lancement du programme
*/
void construire_vue_co(vue_t * c) ;

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de jeu sont construits et imbriqués de sorte à construire la fenêtre de jeu
*/
void construire_vue_jeu(vue_t * c ) ;

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de fin de partie sont construits et imbriqués de sorte à construire la fenêtre de fin de partie
*/
void construire_vue_wl(vue_t * c) ;

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : l'ensemble de la vue graphique est construite, à savoir les trois fenêtres qui la composent ainsi que l'ensemble des widgets associés
*/
void construire_vue(vue_t * vue) ;

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de connexion est masquée, et on affiche la fenêtre de jeu
*/
void chng_vue_co_to_jeu(vue_t * vue) ;

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de jeu est masquée, et on affiche la fenêtre de fin de partie
*/
void chng_vue_jeu_to_wl(vue_t * vue) ;

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de fin de partie est masquée, et on affiche la fenêtre de jeu
*/
void chng_vue_wl_to_jeu(vue_t * vue) ;

/*	Pre-conditions 	: - bouton est un GtkButton* quelconque de la vue graphique
                          - val est un entier (0 ou 1)
	Post-conditions : - si val vaut 0, le bouton passé en paramètre devient inactif
	                  - sinon,le bouton passé en paramètre devient actif
*/
void chng_etat_bouton(GtkButton* bouton, int val) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
	Post-conditions : le label associé au champ donnees_jeu[0] de vue est changé et affiche le texte passé
	                  en argument, c'est-à-dire que l'on affiche sur la fenêtre de jeu quel adversaire on affronte
*/
void set_texte_vs(vue_t * vue, char * texte) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image de la case [ligne][colonne] de la grille de l'adversaire est modifiée
*/
void set_case_adv(vue_t * vue, int ligne, int colonne, char * image) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image de la case [ligne][colonne] de la grille du joueur est modifiée
*/
void set_case_j(vue_t * vue, int ligne, int colonne, char * image) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : le label associé au champ etat_tour_jeu de vue est changé et affiche le texte passé
	                  en argument, c'est-à-dire que l'on affiche si c'est au tour du joueur, ou au tour de l'adversaire
*/
void set_tour(vue_t * vue, char * texte) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
                          - n est un entier compris entre 0 et 3
	Post-conditions : le label associé au champ texte_co[n] de vue est changé et affiche le texte passé
	                  en argument. Ainsi, on peut modifier chaque label de la fenêtre de connexion de sorte à afficher toutes
	                  les instructions relatives à la connexion
*/
void set_texte_co(vue_t * vue, char * texte, int n) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
                          - n est un entier qui vaut soit 1 soit 2
	Post-conditions : le label associé au champ donnees_jeu[n] de vue est changé et affiche le texte passé
	                  en argument. Ainsi, pour n = 1, on affiche le nombre de victoires du joueur et pour n = 2 le nombre de défaites
*/
void set_nb_victoires_defaites(vue_t * vue, char * texte, int n) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image associée au champ im_wl de vue est modifié. Ainsi, on peut afficher dans la fenetre de fin de partie
	                  une image de victoire ou de défaite selon l'issue de la partie
*/
void set_image_wl(vue_t * vue, char * image) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - msg est une chaine de caractères
	Post-conditions : on crée une boite de dialogue contenant le texte passé en argument qui disparait lorsque l'on clique sur
                          le bouton "Valider" qui figure sur cette dernière. La boite de dialogue est présente uniquement dans la 
                          fenêtre de connexion
*/
void dialogue_invalide(vue_t * vue, char * msg) ;

/* _______________________________
	Accesseurs en lecture	*/

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de connexion
*/
GtkWindow* vue_get_fenetre_co(vue_t * vue) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de jeu
*/
GtkWindow* vue_get_fenetre_jeu(vue_t * vue) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de fin de partie (écran de victoire/défaite)
*/
GtkWindow* vue_get_fenetre_wl(vue_t * vue) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier compris entre 0 et 3
	Post-conditions : aucune
	Valeur retour : label relatif à la fenêtre de connexion (dépend de n)
*/
GtkLabel* vue_get_texte_co(vue_t * vue, int n) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier qui vaut soit 0 soit 1
	Post-conditions : aucune
	Valeur retour : - si n vaut 0, on renvoie le bouton avec pour label "Client"
	                - sinon, on renvoie le bouton avec pour label "Serveur"
*/
GtkButton* vue_get_bouton_client_serveur(vue_t * vue, int n) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à confirmer le choix du pseudo
*/
GtkButton* vue_get_bouton_pseudo(vue_t * vue) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à se connecter
*/
GtkButton* vue_get_bouton_connexion(vue_t * vue) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : aucune
	Valeur retour : un des boutons de la grille de jeu (référencé par i et j)
*/
GtkButton* vue_get_bouton_jeu(vue_t * vue, int i, int j) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de fin de partie destiné à relancer une partie
*/
GtkButton* vue_get_bouton_rejouer(vue_t * vue) ;

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à quitter le jeu
*/
GtkButton* vue_get_bouton_quitter(vue_t * vue) ;

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier compris entre 0 et 2
	Post-conditions : aucune
	Valeur retour : zone de saisie de la fenêtre de connexion (dépend de n)
*/
GtkEntry* vue_get_barre_de_saisie(vue_t * vue, int n) ;

#endif
