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
# include <gtk/gtk.h>
# include <string.h>
# include "vue_bataille_navale.h"
# include "controleur_bataille_navale.h" 

/* ____________________ Declaration fonctions internes */

/* Fonction faisant appel aux destructeurs client et serveur sur c et s avant de sortir */	
static void nettoyer_socket(void);

/* sockets a supprimer eventuellement */
client* c = NULL;
serveur* s = NULL;

/* ____________________ Definition fonctions internes */

/* Fonction faisant appel aux destructeurs client et serveur sur c et s avant de sortir */
static void nettoyer_socket(void) {
	printf("%s IN (c == %p, s == %p)\n", __func__, (void*)c, (void*)s);

	if (c != NULL)
		client_delete(c);

	if (s != NULL)
		serveur_delete(s);

	printf("%s OUT (c == %p, s == %p)\n", __func__, (void*)c, (void*)s);

	/* on sort du progamme */
	exit(EXIT_FAILURE);
}

/* ____________________ Définition des fonctions de manipulation */

/*	Pre-conditions 	: - c adresse d'un controleur
                          - v adresse d'une vue
	Post-conditions : tous les champs du controleur sont initialisés, en particulier le jeu et la vue graphique qui 
	                  sont intégralement initialisés et construits
*/
void controleur_init_bataille(ctrl_t * c, vue_t * v)
{
    c->modele = jeu_construire() ;
    init_vue(v) ;
    c->vue = v ;
    construire_vue(c->vue) ;
    for (int i = 0; i < JOUEUR_GRILLE_L_DEFAULT; ++i)
    {
        for (int j = 0; j < JOUEUR_GRILLE_H_DEFAULT; ++j)
        {
            c->id[i][j].controleur = c ;
            c->id[i][j].lig = i ;
            c->id[i][j].col = j ;
        }
    }
    c->etat_prog = 0 ;
    struct sigaction handler_interruption ;
    c->handler_interruption = &handler_interruption ;
}

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : le label de la fenetre de jeu indiquant le tour du joueur est mis à jour selon l'état de la partie
*/
void controleur_afficher_tour(ctrl_t * c)
{
    int etat = jeu_get_etat_partie(c->modele) ;
    if (etat == JEU_ETAT_TOUR_JOUEUR)
        set_tour(c->vue, "À vous de jouer !") ;
    else if (etat == JEU_ETAT_TOUR_ADVERSAIRE)
        set_tour(c->vue, "Au tour de l'adversaire !") ;
} 

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : agit sur la fenetre de jeu en faisant les tâches suivantes :
	                  - met à jour les images affichées par les deux grilles de jeu
	                  - désactive tous les boutons de la grille adversaire si c'est le tour de l'adversaire
	                  - active tous les boutons sauf ceux qui ont déjà été cliqués auparavant si c'est le tour du joueur
	NB : Bug connu : les cases "touché" de la grille adverse ne se mettent pas toutes à jour lorsqu'un bateau coule. 
	Ce léger bug visuel provient soit de la fonction controleur_afficher_case_adversaire, soit de l'organisation de la structure 
	de la vue, plus précisément pour la fenetre de jeu
*/
void controleur_afficher_grilles(ctrl_t * c) 
{
    unsigned int x, y, nbL, nbC;
    nbL = joueur_get_grille_joueur_nbLignes(jeu_get_joueur(c->modele));
    nbC = joueur_get_grille_joueur_nbColonnes(jeu_get_joueur(c->modele));
    int etat ; 
    for (x = 0 ; x < nbL ; x ++) {
        for (y = 0 ; y < nbC ; y ++) {
	    controleur_afficher_case_joueur(c, x, y) ;
	    controleur_afficher_case_adversaire(c, x, y) ;
	    etat = joueur_get_grille_adversaire_case_etat(jeu_get_joueur(c->modele), x, y);
	    if (jeu_get_etat_partie(c->modele) == JEU_ETAT_TOUR_JOUEUR)
	      chng_etat_bouton(vue_get_bouton_jeu(c->vue, x, y), 1) ;
	    if (jeu_get_etat_partie(c->modele) == JEU_ETAT_TOUR_ADVERSAIRE)
	      chng_etat_bouton(vue_get_bouton_jeu(c->vue, x, y), 0) ;
	    if (etat != CASE_GRILLE_ETAT_NONE)
	      chng_etat_bouton(vue_get_bouton_jeu(c->vue, x, y), 0) ;
	}
    }
}

/*	Pre-conditions 	: - c adresse d'un controleur construit entièrement
                          - x entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : modifie sur la fenetre de jeu l'image de la case de la grille du joueur de coordonnées (x;y) selon l'état de la case et son contenu
*/
void controleur_afficher_case_joueur(ctrl_t * c, unsigned int x, unsigned int y)
{
    case_grille_etat etat ;
    bateau_type_cle type;
    
    etat = joueur_get_grille_joueur_case_etat(jeu_get_joueur(c->modele), x, y);
    type = joueur_get_grille_joueur_case_type_bateau(jeu_get_joueur(c->modele), x, y);
    
    if (etat == CASE_GRILLE_ETAT_NONE && type == BATEAU_TYPE_CLE_NONE)
        set_case_j(c->vue, x, y, "Images/Inconnu.png");

	/* pas jouee, un bateau */
	else if (etat == CASE_GRILLE_ETAT_NONE && type > BATEAU_TYPE_CLE_NONE && type < BATEAU_TYPE_CLE_NB) {
	    if (type == BATEAU_TYPE_CLE_PORTE_AVIONS)
	        set_case_j(c->vue, x, y, "Images/Porte-avions.png");
	    else if (type == BATEAU_TYPE_CLE_CROISEUR)
	        set_case_j(c->vue, x, y, "Images/Croiseur.png");
	    else if (type == BATEAU_TYPE_CLE_CONTRE_TORPILLEUR)
	        set_case_j(c->vue, x, y, "Images/Contre-torpilleur.png");
	    else if (type == BATEAU_TYPE_CLE_SOUS_MARIN)
	        set_case_j(c->vue, x, y, "Images/Sous-marin.png");
	    else if (type == BATEAU_TYPE_CLE_TORPILLEUR)
	        set_case_j(c->vue, x, y, "Images/Torpilleur.png");
	}
	
	/* jouee, pas de bateau */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_EAU && type == BATEAU_TYPE_CLE_NONE) {
	    set_case_j(c->vue, x, y, "Images/Raté.png");
	}
	
	/* jouee, un bateau touche */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_TOUCHE && type > BATEAU_TYPE_CLE_NONE && type < BATEAU_TYPE_CLE_NB) {
	    set_case_j(c->vue, x, y, "Images/Touché.png");
	}
	
	/* jouee, un bateau coule */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_COULE && type > BATEAU_TYPE_CLE_NONE && type < BATEAU_TYPE_CLE_NB) {
	      set_case_j(c->vue, x, y, "Images/Coulé.png");
        }
}

/*	Pre-conditions 	: - c adresse d'un controleur construit entièrement
                          - x entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : modifie sur la fenetre de jeu l'image de la case de la grille de l'adversaire de coordonnées (x;y) selon l'état de la case et son contenu
*/
void controleur_afficher_case_adversaire(ctrl_t * c, unsigned int x, unsigned int y)
{
    int etat, type;
    
    etat = joueur_get_grille_adversaire_case_etat(jeu_get_joueur(c->modele), x, y);
    type = joueur_get_grille_adversaire_case_type_bateau(jeu_get_joueur(c->modele), x, y);
    
    if (etat == CASE_GRILLE_ETAT_NONE && type == BATEAU_TYPE_CLE_NONE)
        set_case_adv(c->vue, x, y, "Images/Inconnu.png");
	
	/* jouee, pas de bateau */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_EAU && type == BATEAU_TYPE_CLE_NONE) {
	    set_case_adv(c->vue, x, y, "Images/Raté.png");
	}
	
	/* jouee, un bateau touche */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_TOUCHE && type > BATEAU_TYPE_CLE_NONE && type < BATEAU_TYPE_CLE_NB) {
	    set_case_adv(c->vue, x, y, "Images/Touché.png");
	}
	
	/* jouee, un bateau coule */
	else if (etat == CASE_GRILLE_ETAT_JOUEE_COULE) {
	      set_case_adv(c->vue, x, y, "Images/Coulé.png");
        }
}

/*	Pre-conditions 	: c adresse d'un controleur construit entièrement
	Post-conditions : met à jour sur la fenetre de jeu le nombre de parties gagnées et perdues
*/
void controleur_afficher_nb_wl(ctrl_t * c)
{
    char c_win[10], c_lose[10] ;
    int win, lose;
    win = jeu_get_nb_parties_gagnees(c->modele) ;
    lose = jeu_get_nb_parties_perdues(c->modele) ;
    sprintf(c_win, "%d", win) ;
    set_nb_victoires_defaites(c->vue, c_win, 1) ;
    sprintf(c_lose, "%d", lose) ;
    set_nb_victoires_defaites(c->vue, c_lose, 2) ;
}

/// Fonctions de rappel ///

/*	Pre-conditions 	: - b adresse d'un widget
                          - c adresse d'un controleur construit entièrement
	Post-conditions : récupère le pseudo du joueur rentré dans la zone de saisie dédiée et le renseigne dans les données du jeu lorsque la fonction est appelée.
	En conséquence, la fenetre de connexion est mise à jour pour afficher le pseudo rentré en modifiant un label. Si rien n'est renseigné dans la zone de saisie,
	le pseudo du joueur sera STR_JOUEUR_PSEUDO_DEFAULT
*/
void controleur_definir_pseudo(GtkWidget* b, ctrl_t * c)
{
    char pseudo[JOUEUR_PSEUDO_TAILLE + 1] = STR_JOUEUR_PSEUDO_DEFAULT ; // on initialise avec le pseudo par défaut
    char welcome[JOUEUR_PSEUDO_TAILLE + 11] = "Bienvenue " ;
    const gchar* texte = gtk_entry_get_text(vue_get_barre_de_saisie(c->vue, 0)) ;
    if (strlen(texte) != 0) 
        strncpy(pseudo, texte, JOUEUR_PSEUDO_TAILLE) ; // si l'utilisateur n'écrit rien, son pseudo restera celui par défaut
    pseudo[JOUEUR_PSEUDO_TAILLE] = '\0' ; // caractère de fin de chaine de caractères si besoin
    jeu_set_pseudo(c->modele, pseudo) ;
    chng_etat_bouton(vue_get_bouton_pseudo(c->vue), 0) ;
    strcat(welcome, pseudo) ;
    set_texte_co(c->vue, welcome, 0) ;
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - event est de type GdkEventButton* est vaut explicitement "button-press-event"
                          - casee adresse d'une variable de type case_t dont le champ controleur pointe vers un controleur entièrement construit
	Post-conditions : permet d'effectuer un tour pour le joueur, en envoyant les données du coup joué à l'adversaire. Si suite à cela la partie est gagnée ou
	perdue, on bascule vers la fenetre de fin de partie et on affiche la bonne image selon l'issue de la partie.
	Quoi qu'il advienne, l'interface graphique est mise à jour lorsque l'on quitte cette fonction de rappel.
	Valeur retour : booleen FALSE
*/
gboolean controleur_attaquer(GtkWidget* b, GdkEventButton* event, case_t * casee)
{
    if (!(jeu_partie_est_en_cours(casee->controleur->modele)))
        controleur_quitter(b, casee->controleur) ;
    else if (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_TOUR_JOUEUR)
    {
        chng_etat_bouton(vue_get_bouton_jeu(casee->controleur->vue, casee->lig, casee->col), 0) ;
        jeu_tour_joueur(casee->controleur->modele, casee->lig, casee->col);
        controleur_afficher_tour(casee->controleur) ;
        controleur_afficher_grilles(casee->controleur) ;    
    }
    if ((jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_JOUEUR) || (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_ADVERSAIRE))
    {
        chng_vue_jeu_to_wl(casee->controleur->vue) ;
        if (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_JOUEUR)
            set_image_wl(casee->controleur->vue, "Images/Victoire.png") ;
        else
            set_image_wl(casee->controleur->vue, "Images/Defaite.png") ;
    }
    
    return FALSE ;
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - event est de type GdkEventButton* est vaut explicitement "button-release-event"
                          - casee adresse d'une variable de type case_t dont le champ controleur pointe vers un controleur entièrement construit
	Post-conditions : permet d'effectuer un tour pour l'adversaire, en recevant les données envoyées par le joueur, à savoir le coup joué. Si suite à cela la partie est gagnée ou
	perdue, on bascule vers la fenetre de fin de partie et on affiche la bonne image selon l'issue de la partie.
	Quoi qu'il advienne, l'interface graphique est mise à jour lorsque l'on quitte cette fonction de rappel.
	Valeur retour : booleen FALSE
*/
gboolean controleur_recevoir(GtkWidget* b, GdkEventButton* event, case_t * casee)
{
    if (!(jeu_partie_est_en_cours(casee->controleur->modele)))
        controleur_quitter(b, casee->controleur) ;
    else if (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_TOUR_ADVERSAIRE)
    {
        jeu_tour_adversaire(casee->controleur->modele) ;
        controleur_afficher_tour(casee->controleur) ;
        controleur_afficher_grilles(casee->controleur) ;       
    }
    if ((jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_JOUEUR) || (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_ADVERSAIRE))
    {
        chng_vue_jeu_to_wl(casee->controleur->vue) ;
        if (jeu_get_etat_partie(casee->controleur->modele) == JEU_ETAT_GAGNEE_JOUEUR)
            set_image_wl(casee->controleur->vue, "Images/Victoire.png") ;
        else
            set_image_wl(casee->controleur->vue, "Images/Defaite.png") ;
    }
    
    return FALSE ;
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le mode client est initialisé et la fenetre de connexion est mise à jour afin d'y faire figurer les instructions de connexion
*/
void controleur_partie_client(GtkWidget* b, ctrl_t * controleur)
{
    char info_hote_adversaire[(NOM_HOTE_TAILLE_MAX >= IP_TAILLE_MAX ? NOM_HOTE_TAILLE_MAX : IP_TAILLE_MAX) +1] = "" ;
    chng_etat_bouton(vue_get_bouton_client_serveur(controleur->vue, 0), 0) ;
    chng_etat_bouton(vue_get_bouton_client_serveur(controleur->vue, 1), 0) ;
    set_texte_co(controleur->vue, "Si vous souhaitez rejoindre la partie via le nom de l'hôte, renseignez-le dans la zone de saisie ci-dessous", 2) ;
    set_texte_co(controleur->vue, "Si vous souhaitez rejoindre la partie via l'IP de l'hôte, renseignez-le dans la zone de saisie ci-dessous", 3) ;
    /* memorisation client cree pour traitement suite a capture du signal CTRL +C */
    c = jeu_get_client(controleur->modele);
    set_texte_co(controleur->vue, "Mode client", 1) ;
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le mode serveur est initialisé et la fenetre de connexion est mise à jour afin d'y faire figurer les instructions de connexion
*/
void controleur_partie_serveur(GtkWidget* b, ctrl_t * controleur)
{
    chng_etat_bouton(vue_get_bouton_client_serveur(controleur->vue, 0), 0) ;
    chng_etat_bouton(vue_get_bouton_client_serveur(controleur->vue, 1), 0) ;
    /* memorisation serveur creee pour traitement suite a capture du signal CTRL +C */
    s = jeu_get_serveur(controleur->modele);	/* pour le handler */
    jeu_creer_partie(controleur->modele); 
    
    /* Construction des chaines de caractères à afficher */
    char message_1[300] = "" ;
    char message_2[300] = "" ;
    char* blabla_debut1 = "Vous êtes l'hôte, inutile de remplir cette zone de saisie. Par contre, vous pouvez transmettre le nom d'hôte " ; 
    char* blabla_debut2 = "Vous êtes l'hôte, inutile de remplir cette zone de saisie. Par contre, vous pouvez transmettre l'ip " ; 
    char* blabla_fin = " à votre adversaire pour pouvoir jouer." ; 
    strcpy(message_1, blabla_debut1) ;
    strcat(message_1, jeu_get_nom_hote(controleur->modele)) ;
    strcat(message_1, blabla_fin) ; 
    strcpy(message_2, blabla_debut1) ;
    strcat(message_2, jeu_get_ip(controleur->modele)) ;
    strcat(message_2, blabla_fin) ; 
    /* On change les labels de la fenêtre */
    set_texte_co(controleur->vue, message_1, 2) ;
    set_texte_co(controleur->vue, message_2, 3) ;
    set_texte_co(controleur->vue, "Mode serveur", 1) ;
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : établit la connexion. Lorsque les deux joueurs sont connectés, la partie est initialisée et on bascule sur la fenetre de jeu.
	                  Si la connexion a échoué, la boite de dialogue de la fenetre de connexion s'ouvre pour préciser que la connexion a échoué.
*/
void controleur_connexion(GtkWidget* b, ctrl_t * controleur)
{
    char saisie_nom[NOM_HOTE_TAILLE_MAX + 1] = "" ; 
    char saisie_ip[IP_TAILLE_MAX + 12] = "" ; 
    char texte_vs[JOUEUR_PSEUDO_TAILLE + 1] = "" ;
    char* debut_texte_vs = "Jeu contre " ;
    const gchar* get_nom = gtk_entry_get_text(vue_get_barre_de_saisie(controleur->vue, 1)) ;
    const gchar* get_ip = gtk_entry_get_text(vue_get_barre_de_saisie(controleur->vue, 2)) ;
    strcpy(saisie_nom, get_nom) ;
    strcpy(saisie_ip, get_ip) ;
    int res ;
    if (jeu_get_statut_joueur(controleur->modele) == JEU_JOUEUR_STATUT_PRINCIPAL) // cas serveur
    {
        if (jeu_get_etat_partie(controleur->modele) == JEU_ETAT_ATTENTE_JOUEUR)
            jeu_etre_rejoint_partie(controleur->modele) ;
    }
    else // cas client
    {
        if ((strlen(saisie_nom) > 0) && (strlen(saisie_ip) == 0))      
        {
            res = jeu_rejoindre_partie_par_nom_hote(controleur->modele, saisie_nom);
            if (res == 0)
                dialogue_invalide(controleur->vue, "Échec de la connexion. Vérifie que tu as le bon nom d'hôte ou la bonne adresse ip. Et n'oublie pas qu'il faut remplir une des deux zones de saisie, pas les deux !") ;
        }
        else if ((strlen(saisie_ip) > 0) && (strlen(saisie_nom) == 0))
        {
            res = jeu_rejoindre_partie_par_ip(controleur->modele, saisie_ip);
            if (res == 0)
                dialogue_invalide(controleur->vue, "Échec de la connexion. Vérifie que tu as le bon nom d'hôte ou la bonne adresse ip. Et n'oublie pas qu'il faut remplir une des deux zones de saisie, pas les deux !") ;
        }
        else
            dialogue_invalide(controleur->vue, "Échec de la connexion. Vérifie que tu as le bon nom d'hôte ou la bonne adresse ip. Et n'oublie pas qu'il faut remplir une des deux zones de saisie, pas les deux !") ;
    }
    
    struct sigaction* handler_interruption = controleur->handler_interruption;
    /* en cas de socket creee : associer la fonction nettoyer_socket() au signal SIGINT */
    if ((jeu_get_etat_partie(controleur->modele) == JEU_ETAT_ATTENTE_JOUEUR) || (jeu_get_etat_partie(controleur->modele) == JEU_ETAT_PAS_PARTIE_ENCOURS))
    {
        handler_interruption->sa_handler =(void (*)(int)) nettoyer_socket;
	sigaction(SIGINT, handler_interruption, NULL);
    }
    
    if (jeu_est_partage(controleur->modele))
    {
        chng_vue_co_to_jeu(controleur->vue) ;
        /* Initialisation partie */
        jeu_nouvelle_partie(controleur->modele) ;
        jeu_placer_flotte(controleur->modele) ; // placement aléatoire des bateaux
        controleur_afficher_tour(controleur) ;
        controleur_afficher_grilles(controleur) ;
        jeu_set_nb_parties_gagnees(controleur->modele, 0) ;
        controleur_afficher_nb_wl(controleur) ;
        strcpy(texte_vs, debut_texte_vs) ;
        strcat(texte_vs, jeu_get_pseudo_adversaire(controleur->modele)) ;
        set_texte_vs(controleur->vue, texte_vs) ;
        controleur->etat_prog = 1 ;
    }
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : relance une partie en basculant sur la fenetre de jeu, en mettant à jour le nombre de victoires/défaites et l'état du programme.
*/
void controleur_rejouer(GtkWidget* b, ctrl_t * controleur)
{
    if (!(jeu_est_partage(controleur->modele)))
        controleur_quitter(b, controleur) ;
    else
    {
        jeu_nouvelle_partie(controleur->modele) ;
        jeu_placer_flotte(controleur->modele) ; // placement aléatoire des bateaux
        chng_vue_wl_to_jeu(controleur->vue) ;
        controleur_afficher_tour(controleur) ;
        controleur_afficher_grilles(controleur) ;
        controleur_afficher_nb_wl(controleur) ;
        controleur->etat_prog = 1 ;
    } 
}

/*	Pre-conditions 	: - b adresse d'un widget
                          - controleur adresse d'un controleur construit entièrement
	Post-conditions : le joueur se déconnecte et le programme se ferme.
*/
void controleur_quitter(GtkWidget* b, ctrl_t * controleur)
{
    jeu_se_deconnecter(controleur->modele) ;
    gtk_main_quit() ;
}

/// Fonction appelée à chaque itération de boucle gtk via g_idle_add ///

/*	Pre-conditions 	: - data pointe vers les données d'un controleur
	Post-conditions : - si c'est le tour de l'adversaire, on assure la réception des données et le bon déroulement de la partie
	                  - si la partie est gagnée/perdue, on assure le passage à la fenetre de fin de partie et on met à jour l'état du programme. 
*/
gboolean controleur_assurage_reception(gpointer data)
{
    ctrl_t * c = (ctrl_t *) data ;
    if (c->etat_prog == 1)
    {
        if (jeu_get_etat_partie(c->modele) == JEU_ETAT_TOUR_ADVERSAIRE)
        {
            jeu_tour_adversaire(c->modele) ;
            controleur_afficher_tour(c) ;
            controleur_afficher_grilles(c) ; 
        }
        if ((jeu_get_etat_partie(c->modele) == JEU_ETAT_GAGNEE_JOUEUR) || (jeu_get_etat_partie(c->modele) == JEU_ETAT_GAGNEE_ADVERSAIRE))
        {
            chng_vue_jeu_to_wl(c->vue) ;
            if (jeu_get_etat_partie(c->modele) == JEU_ETAT_GAGNEE_JOUEUR)
                set_image_wl(c->vue, "Images/Victoire.png") ;
            else
                set_image_wl(c->vue, "Images/Defaite.png") ;
            c->etat_prog = 2 ;
        }
    }
    return TRUE ;
}

/* _______________________________
	Accesseurs en lecture	*/

/*	Pre-conditions 	: ctrl est l'adresse d'un controleur construit entièrement
	Post-conditions : aucune
	Valeur retour : adresse d'un jeu
*/
jeu* controleur_get_jeu(ctrl_t ctrl)
{
    return ctrl.modele ;
}

/*	Pre-conditions 	: ctrl est l'adresse d'un controleur construit entièrement
	Post-conditions : aucune
	Valeur retour : adresse d'une vue graphique
*/
vue_t * controleur_get_vue(ctrl_t ctrl)
{
    return ctrl.vue ;
}

/*	Pre-conditions 	: - ctrl est l'adresse d'un controleur construit entièrement
                          - x est un entier non signé compris entre 0 et JOUEUR_GRILLE_L_DEFAULT - 1
                          - y est un entier non signé compris entre 0 et JOUEUR_GRILLE_H_DEFAULT - 1
	Post-conditions : aucune
	Valeur retour : type structuré de type case_t appartenant au controleur (dépend de (x;y)) 
	N.B : inutilisé car provoque une erreur
*/
case_t controleur_get_case(ctrl_t ctrl, unsigned int x, unsigned int y)
{
    return ctrl.id[x][y] ;
}

// :)
