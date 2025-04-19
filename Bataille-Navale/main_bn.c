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
# include "controleur_bataille_navale.h" 

/* ____________________ Declaration fonction principale */

int main(void) ;

/* ____________________ Définition fonction principale */

int main(void)
{
    gtk_init(NULL, NULL); // initialisation de l'environnement gtk
    srand(time(NULL)) ; 
    ctrl_t bat ;
    vue_t v ;
    controleur_init_bataille(&bat, &v) ; // construction du controleur, et donc du jeu et de la vue graphique
    
    /*_____________________ 
            Fonctions de rappel */
    
    /* Pour la fenêtre de connexion */
    
    g_signal_connect(G_OBJECT(vue_get_bouton_pseudo(controleur_get_vue(bat))), "clicked", G_CALLBACK(controleur_definir_pseudo), &bat); // pour valider la saisie du pseudo
    g_signal_connect(G_OBJECT(vue_get_bouton_client_serveur(controleur_get_vue(bat), 0)), "clicked", G_CALLBACK(controleur_partie_client), &bat); // pour se placer en mode client
    g_signal_connect(G_OBJECT(vue_get_bouton_client_serveur(controleur_get_vue(bat), 1)), "clicked", G_CALLBACK(controleur_partie_serveur), &bat); // pour créer une partie en mode serveur
    g_signal_connect(G_OBJECT(vue_get_bouton_connexion(controleur_get_vue(bat))), "clicked", G_CALLBACK(controleur_connexion), &bat); // pour se connecter à la partie
    g_signal_connect(G_OBJECT(vue_get_fenetre_co(controleur_get_vue(bat))), "destroy", G_CALLBACK(gtk_main_quit), NULL); // porte de sortie à l'utilisateur
    
    /* Pour la fenêtre de jeu */
    
    for (int k = 0; k < JOUEUR_GRILLE_L_DEFAULT; ++k)
    {
        for (int l = 0; l < JOUEUR_GRILLE_H_DEFAULT; ++l)
        {
              g_signal_connect(G_OBJECT(vue_get_bouton_jeu(controleur_get_vue(bat), k, l)), "button-press-event", G_CALLBACK(controleur_attaquer), &(bat.id[k][l])); // coup joué, envoi des données à l'adversaire (accesseur en lecture inutilisable)
              g_signal_connect(G_OBJECT(vue_get_bouton_jeu(controleur_get_vue(bat), k, l)), "button-release-event", G_CALLBACK(controleur_recevoir), &(bat.id[k][l])); // reception des données de l'adversaires (accesseur en lecture inutilisable)
        }
    }
    g_signal_connect(G_OBJECT(vue_get_fenetre_jeu(controleur_get_vue(bat))), "destroy", G_CALLBACK(gtk_main_quit), NULL); // porte de sortie à l'utilisateur
    
    /* Pour la fenêtre de fin de partie */
    
    g_signal_connect(G_OBJECT(vue_get_bouton_rejouer(controleur_get_vue(bat))), "clicked", G_CALLBACK(controleur_rejouer), &bat); // pour faire une nouvelle partie contre le même adversaire
    g_signal_connect(G_OBJECT(vue_get_bouton_quitter(controleur_get_vue(bat))), "clicked", G_CALLBACK(controleur_quitter), &bat); // pour arrêter de jouer
    g_signal_connect(G_OBJECT(vue_get_fenetre_wl(controleur_get_vue(bat))), "destroy", G_CALLBACK(gtk_main_quit), NULL); // porte de sortie à l'utilisateur
    
    g_idle_add(controleur_assurage_reception, &bat) ; // fonction appelée à chaque itération de la boucle gtk
    
    gtk_main() ; // lancement de la boucle gtk
    
    jeu_detruire(&(bat.modele)) ; // on libère la mémoire allouée lorsque l'on quitte le programme (accesseur en lecture inutilisable)
  
    return EXIT_SUCCESS ;
}

// :)
