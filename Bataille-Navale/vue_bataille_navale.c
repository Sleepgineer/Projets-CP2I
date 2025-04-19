# include <stdlib.h>
# include <stdio.h>		
# include "jeu.h"	
# include "joueur.h"
# include "client_serveur.h"
# include "bateau_type.h"
# include "placement.h" 
# include "flotte.h" 
# include "case_grille.h"
# include "grille.h"
# include <time.h> 
# include <gtk/gtk.h>
# include <string.h>
# include "vue_bataille_navale.h"


/* ____________________ Définition des fonctions de manipulation de l'interface graphique */

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de la connexion (les champs finissant par _co) sont initialisés à NULL
*/
void init_vue_co(vue_t * v)
{
    v->f_co = NULL ;
    v->bp_co = NULL ;
    v->b1_co = NULL ;
    v->b2_co = NULL ;
    for (int k = 0; k < 4; ++k)
        v->texte_co[k] = NULL ;
    for (int l = 0; l < 2; ++l)
    {
        v->mode_co[l] = NULL ;
    }
    for (int m = 0; m < 3; ++m)
    {
        v->saisie_co[m] = NULL ;
    }
    v->bouton_pseudo_co = NULL ;
    v->bouton_co = NULL ;
    v->invalide_co = NULL ;
}

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de jeu (les champs finissant par _jeu) sont initialisés à NULL
*/
void init_vue_jeu(vue_t * v)
{
    v->f_jeu = NULL ;
    v->bp_jeu = NULL ;
    v->b1_jeu = NULL ;
    v->b2_jeu = NULL ;
    v->b3_jeu = NULL ;
    for (int k = 0; k < 3; ++k)
        v->donnees_jeu[k] = NULL ;
    v->etat_tour_jeu = NULL ;
    for (int l = 0; l < 2; ++l)
    {
        v->camp_jeu[l] = NULL ;
        v->grille_jeu[l] = NULL ;
    }
    for (int i = 0; i < JOUEUR_GRILLE_L_DEFAULT; ++i)
    {
        for (int j = 0; j < JOUEUR_GRILLE_H_DEFAULT; ++j)
        {
            v->cases_adv_jeu[i][j] = NULL ;
            v->cases_j_jeu[i][j] = NULL ;
            v->im_cases_adv_jeu[i][j] = NULL ;
        }
    }
}

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs relatifs à la fenêtre de fin de partie (les champs finissant par _wl) sont initialisés à NULL
*/
void init_vue_wl(vue_t * v)
{
    v->f_wl = NULL ;
    v->bp_wl = NULL ;
    v->b1_wl = NULL ;
    v->im_wl = NULL ;
    v->rejouer_wl = NULL ;
    v->quitter_wl = NULL ;
}

/*	Pre-conditions 	: v adresse d'une vue
	Post-conditions : tous les champs de la vue sont initialisés à NULL
*/
void init_vue(vue_t * v)
{
    init_vue_co(v) ;
    init_vue_jeu(v) ;
    init_vue_wl(v) ;
}

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de connexion sont construits et imbriqués de sorte à construire la fenêtre de connexion.
	On affiche aussi tous les widgets la fenêtre de connexion car ce sera la première fenêtre à être visible au lancement du programme
*/
void construire_vue_co(vue_t * c)
{
    /* Construction des widgets principaux de la fenetre de connexion */
    c->f_co = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL) ;
    c->texte_co[0] = (GtkLabel*) gtk_label_new("Choisissez votre pseudo") ;
    c->texte_co[1] = (GtkLabel*) gtk_label_new("Choisissez un mode de connexion") ;
    c->texte_co[2] = (GtkLabel*) gtk_label_new("") ;
    c->texte_co[3] = (GtkLabel*) gtk_label_new("") ;
    c->mode_co[0] = (GtkButton*) gtk_button_new_with_label("Client") ;
    c->mode_co[1] = (GtkButton*) gtk_button_new_with_label("Serveur") ;
    for (int p = 0; p < 3; ++p) 
        c->saisie_co[p] = (GtkEntry*) gtk_entry_new() ;
    gtk_entry_set_placeholder_text(c->saisie_co[0], "Saisir le pseudo") ;
    gtk_entry_set_placeholder_text(c->saisie_co[1], "Saisir le nom hôte de votre adversaire") ;
    gtk_entry_set_placeholder_text(c->saisie_co[2], "Saisir l'adresse IP de votre adversaire") ;
    c->bouton_pseudo_co = (GtkButton*) gtk_button_new_with_label("Valider") ;
    c->bouton_co = (GtkButton*) gtk_button_new_with_label("Se connecter") ;
    
    /* Emboitement */
    
    c->b1_co = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 45);
    gtk_box_pack_start(GTK_BOX(c->b1_co), GTK_WIDGET(c->saisie_co[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b1_co), GTK_WIDGET(c->bouton_pseudo_co), TRUE, TRUE, TRUE) ;
    c->b2_co = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 40);
    gtk_box_pack_start(GTK_BOX(c->b2_co), GTK_WIDGET(c->mode_co[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b2_co), GTK_WIDGET(c->mode_co[1]), TRUE, TRUE, TRUE) ;
    c->bp_co = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 15);
    gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->texte_co[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->b1_co), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->texte_co[1]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->b2_co), TRUE, TRUE, TRUE) ;
    for (int k = 2; k < 4; ++k)
    {
        gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->texte_co[k]), TRUE, TRUE, TRUE) ;
        gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->saisie_co[k-1]), TRUE, TRUE, TRUE) ;
    }
    gtk_box_pack_start(GTK_BOX(c->bp_co), GTK_WIDGET(c->bouton_co), TRUE, TRUE, TRUE) ;
    
    /* Finalisation de la fenetre de connexion */
    
    gtk_window_set_title(c->f_co, "Bataille navale - Accueil") ;
    gtk_container_add(GTK_CONTAINER(c->f_co), GTK_WIDGET(c->bp_co)) ;
    gtk_window_resize(c->f_co, 800, 600) ;
    gtk_widget_show_all(GTK_WIDGET(c->f_co)) ; 
}

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de jeu sont construits et imbriqués de sorte à construire la fenêtre de jeu
*/
void construire_vue_jeu(vue_t * c)
{
    /* Construction des widgets principaux de la fenetre de jeu */
    c->f_jeu = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    c->donnees_jeu[0] = (GtkLabel*) gtk_label_new("Jeu contre l'adversaire") ;
    c->donnees_jeu[1] = (GtkLabel*) gtk_label_new("Nombre de victoires : 0") ;
    c->donnees_jeu[2] = (GtkLabel*) gtk_label_new("Nombre de défaites : 0") ;
    c->etat_tour_jeu = (GtkLabel*) gtk_label_new("") ;
    c->camp_jeu[0] = (GtkLabel*) gtk_label_new("Votre formation") ;
    c->camp_jeu[1] = (GtkLabel*) gtk_label_new("Camp adverse") ;
    c->grille_jeu[0] = (GtkGrid*) gtk_grid_new() ;
    c->grille_jeu[1] = (GtkGrid*) gtk_grid_new() ;
    for (int i = 0; i < JOUEUR_GRILLE_L_DEFAULT; ++i)
    {
        for (int j = 0; j < JOUEUR_GRILLE_H_DEFAULT; ++j)
        {
            c->cases_j_jeu[i][j] = (GtkImage*) gtk_image_new_from_file("Images/Inconnu.png");
            gtk_grid_attach(c->grille_jeu[0], (GtkWidget*) c->cases_j_jeu[i][j], j, i, 1, 1) ;
            c->cases_adv_jeu[i][j] = (GtkButton*) gtk_button_new() ;
            gtk_grid_attach(c->grille_jeu[1], (GtkWidget*) c->cases_adv_jeu[i][j], j, i, 1, 1) ;
            c->im_cases_adv_jeu[i][j] = (GtkImage*) gtk_image_new_from_file("Images/Inconnu.png");
            gtk_button_set_image(c->cases_adv_jeu[i][j],(GtkWidget*) c->im_cases_adv_jeu[i][j]) ;
        }
    }
    
    /* Emboîtement */
    
    c->b1_jeu = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_start(GTK_BOX(c->b1_jeu), GTK_WIDGET(c->donnees_jeu[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b1_jeu), GTK_WIDGET(c->donnees_jeu[1]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b1_jeu), GTK_WIDGET(c->donnees_jeu[2]), TRUE, TRUE, TRUE) ;
    c->b2_jeu = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_box_pack_start(GTK_BOX(c->b2_jeu), GTK_WIDGET(c->camp_jeu[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b2_jeu), GTK_WIDGET(c->camp_jeu[1]), TRUE, TRUE, TRUE) ;
    c->b3_jeu = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 50);
    gtk_box_pack_start(GTK_BOX(c->b3_jeu), GTK_WIDGET(c->grille_jeu[0]), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b3_jeu), GTK_WIDGET(c->grille_jeu[1]), TRUE, TRUE, TRUE) ;
    c->bp_jeu = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 25);
    gtk_box_pack_start(GTK_BOX(c->bp_jeu), GTK_WIDGET(c->b1_jeu), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_jeu), GTK_WIDGET(c->etat_tour_jeu), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_jeu), GTK_WIDGET(c->b2_jeu), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_jeu), GTK_WIDGET(c->b3_jeu), TRUE, TRUE, TRUE) ;
    
    /* Finalisation de la fenetre de jeu */
    
    gtk_window_set_title(c->f_jeu, "Bataille navale - Jeu") ;
    gtk_container_add(GTK_CONTAINER(c->f_jeu), GTK_WIDGET(c->bp_jeu));
    
}

/*	Pre-conditions 	: c adresse d'une vue préalablement initialisée
	Post-conditions : tous les widgets de la fenêtre de fin de partie sont construits et imbriqués de sorte à construire la fenêtre de fin de partie
*/
void construire_vue_wl(vue_t * c)
{
    /* Construction des widgets principaux de la fenetre de jeu */
    c->f_wl = (GtkWindow*) gtk_window_new(GTK_WINDOW_TOPLEVEL);
    c->im_wl = (GtkImage*) gtk_image_new_from_file("Images/Victoire.png");
    c->rejouer_wl = (GtkButton*) gtk_button_new_with_label("Rejouer") ;
    c->quitter_wl = (GtkButton*) gtk_button_new_with_label("Quitter") ;
    
    /* Emboîtement */
    
    c->b1_wl = (GtkBox*) gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 20);
    gtk_box_pack_start(GTK_BOX(c->b1_wl), GTK_WIDGET(c->rejouer_wl), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->b1_wl), GTK_WIDGET(c->quitter_wl), TRUE, TRUE, TRUE) ;
    c->bp_wl = (GtkBox*) gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
    gtk_box_pack_start(GTK_BOX(c->bp_wl), GTK_WIDGET(c->im_wl), TRUE, TRUE, TRUE) ;
    gtk_box_pack_start(GTK_BOX(c->bp_wl), GTK_WIDGET(c->b1_wl), TRUE, TRUE, TRUE) ;
    
    /* Finalisation de la fenetre de jeu */
    
    gtk_window_set_title(c->f_wl, "Bataille navale - Fin de partie") ;
    gtk_container_add(GTK_CONTAINER(c->f_wl), GTK_WIDGET(c->bp_wl));
}

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : l'ensemble de la vue graphique est construite, à savoir les trois fenêtres qui la composent ainsi que l'ensemble des widgets associés
*/
void construire_vue(vue_t * vue)
{
    construire_vue_co(vue) ;
    construire_vue_jeu(vue) ;
    construire_vue_wl(vue) ;
}

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de connexion est masquée, et on affiche la fenêtre de jeu
*/
void chng_vue_co_to_jeu(vue_t * vue)
{
    gtk_widget_hide(GTK_WIDGET(vue->f_co)) ; 
    gtk_widget_show_all(GTK_WIDGET(vue->f_jeu)) ; 
}

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de jeu est masquée, et on affiche la fenêtre de fin de partie
*/
void chng_vue_jeu_to_wl(vue_t * vue)
{
    gtk_widget_hide(GTK_WIDGET(vue->f_jeu)) ; 
    gtk_widget_show_all(GTK_WIDGET(vue->f_wl)) ; 
}

/*	Pre-conditions 	: vue adresse d'une vue préalablement initialisée
	Post-conditions : la fenêtre de fin de partie est masquée, et on affiche la fenêtre de jeu
*/
void chng_vue_wl_to_jeu(vue_t * vue)
{
    gtk_widget_hide(GTK_WIDGET(vue->f_wl)) ; 
    gtk_widget_show_all(GTK_WIDGET(vue->f_jeu)) ; 
}

/*	Pre-conditions 	: - bouton est un GtkButton* quelconque de la vue graphique
                          - val est un entier (0 ou 1)
	Post-conditions : - si val vaut 0, le bouton passé en paramètre devient inactif
	                  - sinon,le bouton passé en paramètre devient actif
*/
void chng_etat_bouton(GtkButton* bouton, int val)
{
    gtk_widget_set_sensitive((GtkWidget*) bouton, val);
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
	Post-conditions : le label associé au champ donnees_jeu[0] de vue est changé et affiche le texte passé
	                  en argument, c'est-à-dire que l'on affiche sur la fenêtre de jeu quel adversaire on affronte
*/
void set_texte_vs(vue_t * vue, char * texte)
{
    gtk_label_set_label(vue->donnees_jeu[0], texte) ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image de la case [ligne][colonne] de la grille de l'adversaire est modifiée
*/
void set_case_adv(vue_t * vue, int ligne, int colonne, char * image)
{
    gtk_image_set_from_file(vue->im_cases_adv_jeu[ligne][colonne], image);
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image de la case [ligne][colonne] de la grille du joueur est modifiée
*/
void set_case_j(vue_t * vue, int ligne, int colonne, char * image)
{
    gtk_image_set_from_file(vue->cases_j_jeu[ligne][colonne], image);
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : le label associé au champ etat_tour_jeu de vue est changé et affiche le texte passé
	                  en argument, c'est-à-dire que l'on affiche si c'est au tour du joueur, ou au tour de l'adversaire
*/
void set_tour(vue_t * vue, char * texte)
{
    gtk_label_set_label(vue->etat_tour_jeu, texte) ; 
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
                          - n est un entier compris entre 0 et 3
	Post-conditions : le label associé au champ texte_co[n] de vue est changé et affiche le texte passé
	                  en argument. Ainsi, on peut modifier chaque label de la fenêtre de connexion de sorte à afficher toutes
	                  les instructions relatives à la connexion
*/
void set_texte_co(vue_t * vue, char * texte, int n)
{
    gtk_label_set_label(vue->texte_co[n], texte) ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - texte est une chaine de caractères
                          - n est un entier qui vaut soit 1 soit 2
	Post-conditions : le label associé au champ donnees_jeu[n] de vue est changé et affiche le texte passé
	                  en argument. Ainsi, pour n = 1, on affiche le nombre de victoires du joueur et pour n = 2 le nombre de défaites
*/
void set_nb_victoires_defaites(vue_t * vue, char  * texte, int n)
{
    char message[50] = ""; 
    char nb_victoires[25] = "Nombre de victoires : "; 
    char nb_defaites[25] = "Nombre de défaites : " ;
    if (n == 1)
        strcpy(message, nb_victoires) ;
    else if (n == 2)
        strcpy(message, nb_defaites) ;
    strcat(message, texte) ;
    gtk_label_set_label(vue->donnees_jeu[n], message) ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - image est une chaine de caractères désignant un fichier image en format .png
	Post-conditions : l'image associée au champ im_wl de vue est modifié. Ainsi, on peut afficher dans la fenetre de fin de partie
	                  une image de victoire ou de défaite selon l'issue de la partie
*/
void set_image_wl(vue_t * vue, char * image)
{
    gtk_image_set_from_file(vue->im_wl, image);
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - msg est une chaine de caractères
	Post-conditions : on crée une boite de dialogue contenant le texte passé en argument qui disparait lorsque l'on clique sur
                          le bouton "Valider" qui figure sur cette dernière. La boite de dialogue est présente uniquement dans la 
                          fenêtre de connexion
*/
void dialogue_invalide(vue_t * vue, char * msg)
{
    vue->invalide_co = (GtkDialog *) gtk_message_dialog_new(vue->f_co, GTK_DIALOG_DESTROY_WITH_PARENT, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, msg) ;
    gtk_dialog_run(vue->invalide_co) ;
    gtk_widget_destroy(GTK_WIDGET(vue->invalide_co)) ;
}

/* _______________________________
	Accesseurs en lecture	*/
	
/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de connexion
*/
GtkWindow * vue_get_fenetre_co(vue_t * vue)
{
    return vue->f_co ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de jeu
*/
GtkWindow * vue_get_fenetre_jeu(vue_t * vue)
{
    return vue->f_jeu ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : fenetre de fin de partie (écran de victoire/défaite)
*/
GtkWindow * vue_get_fenetre_wl(vue_t * vue)
{
    return vue->f_wl ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier compris entre 0 et 3
	Post-conditions : aucune
	Valeur retour : label relatif à la fenêtre de connexion (dépend de n)
*/
GtkLabel * vue_get_texte_co(vue_t * vue, int n)
{
    return vue->texte_co[n] ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier qui vaut soit 0 soit 1
	Post-conditions : aucune
	Valeur retour : - si n vaut 0, on renvoie le bouton avec pour label "Client"
	                - sinon, on renvoie le bouton avec pour label "Serveur"
*/
GtkButton * vue_get_bouton_client_serveur(vue_t * vue, int n)
{
    return vue->mode_co[n] ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à confirmer le choix du pseudo
*/
GtkButton * vue_get_bouton_pseudo(vue_t * vue)
{
    return vue->bouton_pseudo_co ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à se connecter
*/
GtkButton * vue_get_bouton_connexion(vue_t * vue)
{
    return vue->bouton_co ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - ligne est un entier compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - colonne est un entier compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : aucune
	Valeur retour : un des boutons de la grille de jeu (référencé par i et j)
*/
GtkButton * vue_get_bouton_jeu(vue_t * vue, int i, int j)
{
    return vue->cases_adv_jeu[i][j] ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de fin de partie destiné à relancer une partie
*/
GtkButton * vue_get_bouton_rejouer(vue_t * vue)
{
    return vue->rejouer_wl ;
}

/*	Pre-conditions 	: vue est l'adresse d'une vue construite entièrement
	Post-conditions : aucune
	Valeur retour : bouton de la fenetre de connexion destiné à quitter le jeu
*/
GtkButton * vue_get_bouton_quitter(vue_t * vue)
{
    return vue->quitter_wl ;
}

/*	Pre-conditions 	: - vue est l'adresse d'une vue construite entièrement
                          - n entier compris entre 0 et 2
	Post-conditions : aucune
	Valeur retour : zone de saisie de la fenêtre de connexion (dépend de n)
*/
GtkEntry * vue_get_barre_de_saisie(vue_t * vue, int n)
{
    return vue->saisie_co[n] ;
}

// :)
