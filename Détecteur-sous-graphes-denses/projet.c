/* Projet : Sous-graphe dense avec résolution via colonie de fourmis
Réalisation : Yassine BENMERAH et Toky RATOVOHARINONY */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <string.h>
 
typedef struct {int i; int j; double poids;} arete_t ;

/* Déclaration des fonctions */

int * creer_tab_entier(int, int);
double * creer_tab_double(int, double);
arete_t * creer_tab_arete(int);
void detruire_tab_int(int *, int);
void detruire_tab_double(double *, int);
void detruire_tab_arete(arete_t *, int);

int * resolution(int, int, int, arete_t *, int, double);
void premier_jet(int *, arete_t *, int, int) ;
double calcul_poids(int *, arete_t *, int) ;
int * nouvelle_solution(int *, arete_t *, int, int, double *) ;
void normalisation(double *, double, int) ;
double moyenne_du_poids(double, double, int) ;
double set_parfum(double, double) ;
double * depot_pheromones(double *, int *, int *, double, int) ;
double * evaporation(double *, int, double) ;
double * nouvelles_probas(double *, double *, int) ;
void afficher_solution(int *, double, int) ;
void ouverture_fichiers(FILE **, FILE **, int *, int *, int *);

/* Programme principal */

int main(void)
{
  FILE * graphe ;
  FILE * parametres ;
  int k ; // nombre de noeuds du sous-graphe recherché
  int n ; // nombre de noeuds du graphe à disposition
  int m ; // nombre d'aretes du graphe à disposition
  ouverture_fichiers(&graphe, &parametres, &k, &n, &m); // on ouvre deux fichiers nécessaires à l'exécution du programme et on initialise k, n et m
  arete_t * a = creer_tab_arete(m) ;
  int compt = 0 ;
  int caractere = 0 ;
  // On lit les lignes restantes du fichier graphe de sorte à récupérer les aretes du graphe complet
  while ((fscanf(graphe, "%d", &caractere) != EOF) && (compt < m)) 
  {
    a[compt].i = caractere ;
    if ((fscanf(graphe, "%d", &caractere) != EOF))
      a[compt].j = caractere ;
    if ((fscanf(graphe, "%d", &caractere) != EOF))
      a[compt].poids = (double)caractere ;
    ++compt ;
  }
  int iter = 0 ; // nombre d'itérations effectuées choisi arbitrairement dans le fichier parametres
  double evap = 0.0 ; // coefficient d'évaporation des phéromones choisi arbitrairement dans le fichier parametres. Les phéromones diminueront très légèrement à chaque itération
  int seed = 0 ; // graine choisie arbitrairement dans le fichier parametre permettant de controler l'aléatoire
  // On lit les lignes du fichier parametres de sorte à affecter à iter, evap et seed des valeurs
  fscanf(parametres, "%d", &iter);
  fscanf(parametres, "%lf", &evap);
  fscanf(parametres, "%d", &seed);
  srand(seed) ; // initialisation de la seed de nombre aléatoires
  int * solution ; // variable contenant la solution trouvée
  printf("Résolution en cours...\n") ;
  solution = resolution(n, k, m, a, iter, evap) ; // On appelle la fonction permettant de générer la solution
  afficher_solution(solution, calcul_poids(solution, a, m), n) ; // on affiche la solution et son poids, que l'on calcule
  // On libère la mémoire des structures de données restantes et on ferme les fichiers utilisés
  detruire_tab_int(solution, n) ;
  detruire_tab_arete(a, m) ;
  fclose(graphe) ;
  fclose(parametres) ;
  return 0 ;
}

/* Définition des fonctions */

int * creer_tab_int(int taille, int val)
{
  // Fonction qui créé un tableau d'entiers dynamique de taille "taille" initialisé avec la valeur val à chaque indice
  int * tab = (int *)malloc(sizeof(int)*taille) ;
  if (NULL == tab)
  {
    perror("Erreur dans l'allocation de la mémoire") ;
    exit(-1) ;
  }
  for (int i = 0; i<taille; ++i)
    tab[i] = val ;
  return tab ;
}

double * creer_tab_double(int taille, double val)
{
  // Fonction qui créé un tableau de doubles dynamique de taille "taille" initialisé avec la valeur val à chaque indice
  double * tab = (double *)malloc(sizeof(double)*taille) ;
  if (NULL == tab)
  {
    perror("Erreur dans l'allocation de la mémoire") ;
    exit(-1) ;
  }
  for (int i = 0; i<taille; ++i)
    tab[i] = val ;
  return tab ;
}

arete_t * creer_tab_arete(int taille)
{
  // Fonction qui créé un tableau d'aretes dynamique de taille "taille"
  arete_t * tab = (arete_t *)malloc(sizeof(arete_t)*taille) ;
  if (NULL == tab)
  {
    perror("Erreur dans l'allocation de la mémoire") ;
    exit(-1) ;
  }
  for (int k = 0; k<taille; ++k)
  {
    tab[k].i = 0 ;
    tab[k].j = 0 ;
    tab[k].poids = 0.0 ;
  }
  return tab ;
}

void detruire_tab_int(int * tab, int taille)
{
  // Fonction qui détruit un tableau d'entiers dynamique en libérant la mémoire
  assert(NULL != tab) ;
  for (int i = 0; i<taille; ++i)
    tab[i] = 0 ;
  tab = NULL ;
  free(tab) ;
}

void detruire_tab_double(double * tab, int taille)
{
  // Fonction qui détruit un tableau de doubles dynamique en libérant la mémoire
  assert(NULL != tab) ;
  for (int i = 0; i<taille; ++i)
    tab[i] = 0.0 ;
  tab = NULL ;
  free(tab) ;
}

void detruire_tab_arete(arete_t * tab, int taille)
{
  // Fonction qui détruit un tableau d'aretes dynamique en libérant la mémoire
  assert(NULL != tab) ;
  for (int k = 0; k<taille; ++k)
  {
    tab[k].i = 0 ;
    tab[k].j = 0 ;
    tab[k].poids = 0 ;
  }
  tab = NULL ;
  free(tab) ;
}

int * resolution(int n, int k, int m, arete_t * lst_aretes, int iter, double evap)
{
  // Fonction permettant de générer la meilleure solution possible selon des paramètres
  int * sol = creer_tab_int(n, 0) ; // meilleure solution ayant jamais existée 
  premier_jet(sol, lst_aretes, n, k) ; // on génère une première solution aléatoire
  double p_init = (double)1/n ;
  double * probas = creer_tab_double(n, p_init) ; // on initialise les probabilités, qui seront altérées ultérieurement via les phéromones
  double poids_max = calcul_poids(sol, lst_aretes, m) ; // on initialise le poids maximal comme étant le poids de la première solution
  double poids_moyen = poids_max ;
  int i = 1 ; // i correspond au nombre d'itérations effectuées jusqu'à présent
  int * nv ; // variable qui contiendra une nouvelle solution
  double * phero = creer_tab_double(n, 1.0) ; // tableau contenant la quantité de phéromones sur chaque noeud
  double parfum = 0.0 ; // coefficient déterminant la quantité de phéromones à attribuer
  double poids_nv = 0.0 ; // poids d'une nouvelle solution
  while (i <= iter) // On effectue un certain nombre d'itérations défini dans le fichier parametres
  {
    nv = creer_tab_int(n, 0) ; // on alloue de la mémoire afin de calculer une nouvelle solution
    nv = nouvelle_solution(nv, lst_aretes, n, k, probas) ; // on génère une nouvelle solution
    poids_nv = calcul_poids(nv, lst_aretes, m) ; // on calcule le poids de la nouvelle solution
    poids_moyen = moyenne_du_poids(poids_nv, poids_moyen, i) ; // on effectue la moyenne du poids de toutes les solutions obtenues
    parfum = set_parfum(poids_nv, poids_moyen) ; // on définit le parfum comme étant la qualité de la nouvelle solution
    phero = depot_pheromones(phero, sol, nv, parfum, n) ; // on utilise ce parfum afin d'augmenter le nombre de phéromones sur certains noeuds
    phero = evaporation(phero, n, evap) ; // on réduit légèrement l'ensemble des phéromones à chaque itération
    if (poids_nv > poids_max) // on remplace la solution de référence par la nouvelle solution si celle-ci est meilleure
    {
      for (int y = 0; y < n; ++y)
        sol[y] = nv[y] ;
      poids_max = poids_nv ;
    }
    probas = nouvelles_probas(probas, phero, n) ; // on calcule les nouvelles probabilités en fonction de la quantité de phéromones sur chaque noeud
    detruire_tab_int(nv, n) ; // on libère la mémoire de la nouvelle solution
    // Messages personnalisés 
    if (i == iter/2)
      printf("Résolution à mi-chemin, encore un peu de patience...\n") ;
    if (i == (int)(iter/1.5))
      printf("On y est presque...\n") ;
    ++i ;
  }
  // On libère la mémoire des structures de données qui ne nous sont plus utiles
  detruire_tab_double(probas, n) ; 
  detruire_tab_double(phero, n) ;
  return sol ;
}

void premier_jet(int * premier, arete_t * a_tot, int n, int k)
{
  // Fonction qui génère une première solution au hasard qui servira de référence
  int tirage ; // cette variable contiendra le tirage aléatoire de l'indice du tableau d'indices
  int indices[n] ; // tableau des indices des noeuds qu'il est possible de visiter
  int taille = n ;
  for (int i = 0; i<n; ++i) // initialisation du tableau d'indices
    indices[i] = i ; 
  for (int j = 0; j<k; ++j)
  {
    tirage = rand() % taille ;
    premier[indices[tirage]] = 1 ; // on fixe un noeud à un
    indices[tirage] = indices[taille-1] ; // on retire l'indice du noeud du tableau 
    taille-- ; // on réduit laint tirage ; // cette variable contiendra le tirage aléatoire de l'indice du tableau d'indices
  }
}

double calcul_poids(int * solution, arete_t * lst_aretes, int m) 
{
  // Fonction qui pour un sous-graphe donné permet de calculer le poids de ce dernier
  double total = 0.0 ;
  for (int l=0; l<m; ++l)
  {
    if ((solution[lst_aretes[l].i - 1] == 1) && (solution[lst_aretes[l].j - 1] == 1)) // vérification de l'existence d'une arete
      total += lst_aretes[l].poids ;
  }
  return total ;
}

int * nouvelle_solution(int * sol, arete_t * lst_aretes, int n, int k, double * probas)
{
  // Fonction qui crée une nouvelle solution en s'appuyant sur les itérations précédentes
  double tirage = 0.0; // la variable contiendra un nombre compris entre 0  et 1 qui permettra de choisir le noeud en fonction des probabilités
  int indices[n] ; // tableau des indices des noeuds qu'il est possible de visiter
  int taille = n ;
  int index = 0 ;
  double compteur ;
  double p_retire ; // probabilité qui sera exclue du tableau à chaque itération
  for (int i = 0; i<n; ++i) // initialisation du tableau d'indices
    indices[i] = i ; 
  for (int j = 0; j<k; ++j)
  {
    tirage = (double)rand() / RAND_MAX ; // tirage d'un nombre compris entre 0 et 1
    compteur = 0.0 ;
    for (int h = 0; h<taille; ++h) // on choisit l'indice en voyant dans quel intervalle de probabilité se situe le nombre tiré
    {
      compteur += probas[h] ; // on ajoute les probas au compteur
      if (tirage <= compteur) // permet de savoir si le nombre tiré appartient à un certain intervalle de probabilité
      {
        index = h ; // affectation lorsque le nombre tiré appartient à un certain intervalle de probabilité
        break ;
      }
    }
    sol[indices[index]] = 1 ; // La transition s'effectue ici (après avoir choisi un sommet selon un ensemble de probabilités)
    // On exclue le noeud sélectionné et sa probabilité de sorte à ne pas retomber dessus dans un prochain tirage aléatoire
    indices[index] = indices[taille-1] ;
    p_retire = probas[index] ;
    probas[index] = probas[taille-1] ;
    taille-- ; // on réduit la taille du tableau
    normalisation(probas, p_retire, taille) ; // on modifie les probabilités afin de conserver la propriété sur la somme des probas étant égale à 1
  }
  return sol ;
}

void normalisation(double * probas, double p_retire, int n)
{
  // Fonction qui modifie les probabilités de sorte à ce que la somme des probas soit égale à 1
  double coef_norm = p_retire / n ;
  for (int p = 0; p < n; ++p)
    probas[p] += coef_norm ;
}

double moyenne_du_poids(double poids, double poids_moy, int i)
{
  // Fonction qui effectue la moyenne du poids de tous les sous-graphes déterminés jusqu'à présent
  return (i*poids_moy + poids)/(i + 1) ;
}

double set_parfum(double poids_nv, double poids_moyen)
{
  // Fonction qui évalue simplement la qualité de la solution
  double p = 0.0 ;
  if (poids_moyen != 0)
    p = poids_nv/poids_moyen ; 
  else
    p = 0.0 ;
  return p ;
}

double * depot_pheromones(double * phero, int * ref, int * sol, double parfum, int n)
{
  // Fonction qui dépose des phéromones en fonction de la qualité (parfum) de la solution
  for (int i = 0; i<n; ++i)
  {
    if ((ref[i] == 1) && (ref[i] == sol[i]))
      phero[i] += parfum ; // On rajoute des phéromones à chaque noeud en commun avec la solution de référence
  }
  return phero ;
}

double * evaporation(double * phero, int n, double evap)
{
  // Fonction qui réduit légèrement la quantité de toutes les phéromones
  for (int i = 0; i < n; ++i)
    phero[i] = phero[i] * evap ; // l'évaporation dépend d'evap qui est un paramètre lut dans un fichier
  return phero ;
}

double * nouvelles_probas(double * probas, double * phero, int n)
{
  // Fonction qui modifie la probabilité de visiter chaque noeud en fonction des phéromones
  double somme_phero = 0.0 ;
  for (int i = 0; i < n; ++i)
    somme_phero += phero[i] ;
  for (int j = 0; j < n; ++j)
    probas[j] = phero[j]/somme_phero ; // cette formule permet d'obtenir une probabilité convenable pour chaque noeud 
  return probas ;
}

void afficher_solution(int * sol, double poids, int n)
{
  /* Fonction qui permet d'afficher la solution sous forme d'une liste de noeuds avec 1
  si le noeud est sélectionné et 0 sinon. Le poids de la solution est également affiché
  */
  printf("Solution trouvée :\n(");
  for (int k = 0; k<n; ++k)
  {
    if (k == n-1)
      printf("%d", sol[k]);
    else
      printf("%d, ", sol[k]);
  }
  printf(")\nPoids de la solution : %lf\n", poids) ;
}

void ouverture_fichiers(FILE ** Graphe, FILE ** parametres, int * k, int * n, int * m)
{
  /* 
  Fonction qui permet d'ouvrir les fichiers nécessaires à l'exécution du programme, et affecte
  les paramètres
  */
  char nom[255] ; // 255 car c'est le nombre de caractères maximal d'un nom de fichier sous Linux
  printf("Entrez le nom du fichier texte sans .txt:\n") ; 
  scanf("%s", nom) ;
  strcat(nom, ".txt") ; // concaténation
  *Graphe = fopen(nom, "r") ; // ouverture du fichier graphe rentré par l'utilisateur
  *parametres = fopen("parametres.txt", "r") ;
  if (*Graphe == NULL) // si le fichier rentré par l'utilisateur n'a pas été trouvé
  {
    printf("Impossible d'ouvrir le fichier %s\n", nom);
    exit(0);
  }
  if (*parametres == NULL) // si le fichier rentré par l'utilisateur n'a pas été trouvé
  {
    printf("Impossible d'ouvrir le fichier parametres.txt\n");
    exit(0);
  }
  fscanf(*Graphe, "%d", k); // on affecte à k la première ligne du fichier graphe
  fscanf(*Graphe, "%d", n); // on affecte à n la deuxième ligne du fichier graphe
  fscanf(*Graphe, "%d", m); // on affecte à m la troisième ligne du fichier graphe
}
