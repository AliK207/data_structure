#include "../include/data.h"
#include "../include/load_data.h"
#include "../include/requetes.h"

/******************* FONCTIONS UTILES DANS LES REQUETES********************/


// fonction qui ajout dans une liste chainee des airports d'une facon unique sans doublon
void ajout_unique_airport (struct cell_airports **pl, struct airpo x)
{

    struct cell_airports *new = malloc (sizeof (struct cell_airports));
    struct cell_airports *t = *pl; // copie de la liste passe en parametre

    if (new == NULL) return;

    if (t == NULL)
    {
        add_airport (pl, x);
        return;
    }

    while (t != NULL) t = t->suiv_airpo;

    t = *pl;

    while (t->suiv_airpo != NULL && strcmp (x.iata_airp, t->val_airpo.iata_airp) != 0)
        t = t->suiv_airpo;


    if (strcmp (t->val_airpo.iata_airp, x.iata_airp) == 0)
    {
        free (new);
        return;
    }
    new->val_airpo  = x;
    new->suiv_airpo = NULL;
    t->suiv_airpo   = new;
}


// fonction qui ajout dans une liste chainee des airlines d'une facon unique sans doublon
void ajout_unique_airline (struct cell_airlines **pl, struct airls x)
{
    struct cell_airlines *tmp = *pl;

    if (*pl == NULL)
    {
        add_airlines (pl, x);
        return;
    }

    while (tmp != NULL) tmp = tmp->suiv_airl;
    tmp = *pl;
    while (tmp->suiv_airl != NULL && 0 != strcmp (x.iata_airl, tmp->val_airl.iata_airl))
        tmp = tmp->suiv_airl;

    // on veut tester si il sort a la valeur d'iata corresp
    if (strcmp (x.iata_airl, tmp->val_airl.iata_airl) == 0) return;
    struct cell_airlines *nouv = malloc (sizeof (struct cell_airlines));
    nouv->val_airl             = x;
    nouv->suiv_airl            = tmp->suiv_airl;
    tmp->suiv_airl             = nouv;
}

// ajout dans une liste des vols et les triees par rapport au retard
// pour qu'elles seraient utilisees dans
// la fonction
void ajout_trie_flight (struct cell_vol **pl, struct flights x)
{

    int              nbe = 0;
    struct cell_vol *tmp = *pl;

    if (*pl == NULL)
    {
        add_flights (pl, x); // ajout tete
        return;
    }

    while (tmp != NULL)
    {
        nbe += 1;
        tmp = tmp->suiv_vol;
    }

    tmp = *pl;

    // on cherche a ajouter que  5 vols comme il est demande dans l'ennoce

    if (nbe < 5)
    {
        if (x.arr_delay < tmp->val_vol.arr_delay)
        {
            add_flights (pl, x);
            return;
        }
        while (tmp->suiv_vol != NULL && x.arr_delay > tmp->suiv_vol->val_vol.arr_delay)
        {
            tmp = tmp->suiv_vol;
        }


        // allocation de memoire pour la nouvelle cellule a ajouter

        struct cell_vol *n = malloc (sizeof (struct cell_vol));
        n->val_vol         = x;

        // ajout queue de la cellule
        if (tmp->suiv_vol == NULL && tmp->val_vol.arr_delay < x.arr_delay)
        {
            n->suiv_vol   = NULL;
            tmp->suiv_vol = n;
            return;
        }
        // ajout dans l'endroit qui respect son emplacement pour que la liste sera triee
        n->suiv_vol   = tmp->suiv_vol;
        tmp->suiv_vol = n;
        return;
    }
    else
    {
        if (tmp->val_vol.arr_delay > x.arr_delay) return;
        while (tmp->suiv_vol != NULL && tmp->suiv_vol->val_vol.arr_delay < x.arr_delay)
            tmp = tmp->suiv_vol;
        struct cell_vol *n = malloc (sizeof (struct cell_vol));

        n->val_vol = x;
        if (tmp->suiv_vol == NULL)
        {
            n->suiv_vol   = tmp->suiv_vol;
            tmp->suiv_vol = n;
        }

        else
        {
            n->suiv_vol   = tmp->suiv_vol;
            tmp->suiv_vol = n;
        }
        struct cell_vol *m = *pl;
        *pl                = (*pl)->suiv_vol;
        free (m);
        return;
    }
}


// On chercher a trier les airlines dans un ordre croissant
// pour utiliser cette fonction dans most-delayed-airline

void tri_airline (struct airline_delayed tab[], int size)
{
    int i, j;

    // une nouvelle structure qui a ete creer auparavant
    struct airline_delayed tmp;


    // tri par bulle

    for (i = 1; i <= size; i++)
    {
        for (j = 0; j < size - i; j++)
        {
            if (tab[j + 1].retard > tab[j].retard)
            {

                tmp        = tab[j];
                tab[j]     = tab[j + 1];
                tab[j + 1] = tmp;
            }
        }
    }
}

/******************* REQUETE (S) ********************/


// DONE

void show_airports (char iata[], Hashtable ht, struct cell_airports *a)
{

    // hachage par rapport a l'iata
    struct cell_vol *     v      = ht[hash_fun_airline (iata)];
    struct cell_airports *unique = NULL;
    struct cell_airports *l      = a;
    printf ("> show-airports %s\n", iata);


    // paracours des listes des aeroport et des vols

    while (v != NULL)
    {
        while (l != NULL)
        {

            // test pour eviter les duplication
            if (strcmp (l->val_airpo.iata_airp, v->val_vol.org_air) == 0 ||
                strcmp (v->val_vol.dest_air, l->val_airpo.iata_airp) == 0)
            {

                ajout_unique_airport (&unique, l->val_airpo);
            }
            l = l->suiv_airpo;
        }
        l = a;
        v = v->suiv_vol;
    }


    // affichage du resultat
    while (unique != NULL)
    {
        printf ("%s,%s,%s,%s\n", unique->val_airpo.iata_airp, unique->val_airpo.airport,
                unique->val_airpo.city, unique->val_airpo.state);
        unique = unique->suiv_airpo;
    }
    printf ("\n");
    delete_aiport (&unique);
}


// DONE
void show_flights (char iatap[], int j, int m, Hashtable ht_date)
{

    // hachage par rapport a la date

    struct cell_vol *l = ht_date[hash_fun_date (j, m)];
    printf ("> show-flights %s %d-%d\n", iatap, m, j);

    while (l != NULL)
    {
        // affichage du resultat qui respecte la condition afin d'imarginalier les cellules de collision

        if (strcmp (iatap, l->val_vol.org_air) == 0 && l->val_vol.month == m && l->val_vol.day == j)
        {
            printf ("%d,%d,%d,", l->val_vol.month, l->val_vol.day, l->val_vol.weekday);
            printf ("%s,%s,", l->val_vol.airline, iatap);
            printf ("%s", l->val_vol.dest_air);
            printf (",%d,%.1f,%.1f,", l->val_vol.sched_dep, l->val_vol.dep_delay, l->val_vol.air_time);
            printf ("%d,%d", l->val_vol.dist, l->val_vol.sched_arr);
            printf (",%.1f,%d,%d\n", l->val_vol.arr_delay, l->val_vol.diverted, l->val_vol.cancel);
        }

        l = l->suiv_vol;
    }
    printf ("\n");
    delete_vol (&l);
}

// DONE
void show_airlines (char org[], struct cell_airlines *al, Hashtable ht_ap)
{


    struct cell_airlines *fin  = NULL;
    struct cell_vol *     vol  = ht_ap[hash_fun_airport (org)];
    struct cell_airlines *copy = NULL;


    if (vol == NULL) return;
    if (al == NULL) return;

    printf ("> show-airlines %s\n", org);


    // parcours et test afin d'eviter les duplications et les collisions
    // dans le resultat final

    while (vol != NULL)
    {
        copy = al;
        while (copy != NULL)
        {
            if (strcmp (copy->val_airl.iata_airl, vol->val_vol.airline) == 0)
            {
                ajout_unique_airline (&fin, copy->val_airl);
            }
            copy = copy->suiv_airl;
        }
        vol = vol->suiv_vol;
    }


    // affichage du resultat !
    while (fin != NULL)
    {
        printf ("%s,%s\n", fin->val_airl.iata_airl, fin->val_airl.airline);
        fin = fin->suiv_airl;
    }
    dead_airlines (&fin);
    printf ("\n");
}


// DONE
void changed_flights (int m, int j, Hashtable ht_date)
{
    struct cell_vol *l = ht_date[hash_fun_date (j, m)];
    printf ("> changed-flights %d-%d\n", m, j);

    // print des vols qui sont annulees ou deviees ?

    while (l != NULL)
    {
        if (l->val_vol.day == j && l->val_vol.month == m && (l->val_vol.cancel == 1 || l->val_vol.diverted == 1))
        {

            printf ("%d,%d,%d,", m, j, l->val_vol.weekday);
            printf ("%s,%s,", l->val_vol.airline, l->val_vol.org_air);
            printf ("%s,%d,,,%d", l->val_vol.dest_air, l->val_vol.sched_dep, l->val_vol.dist);
            printf (",%d,%d,%d\n", l->val_vol.sched_arr, l->val_vol.diverted, l->val_vol.cancel);
        }

        l = l->suiv_vol;
    }
    printf ("\n");
}


// DONE
/*void find_itinerary(char org[],char dest[],int j,int m,Hashtable ht_date){

  struct cell_vol * v = ht_date[hash_fun_date(j,m)];
  printf("> find-itineray %s %s %d-%d\n",org,dest,m,j);
  while(v != NULL){
    if(v->suiv_vol != NULL && strcmp(v->val_vol.org_air,org) == 0 && j == v->val_vol.day &&
       m == v->val_vol.month){

      printf("%d,%d,%d,",m,j,v->val_vol.weekday);

      printf("%s,%s,",v->val_vol.airline,org);

      printf("%s,%d,",v->val_vol.dest_air,v->val_vol.sched_dep);

      printf("%.1f,%.1f,",v->val_vol.dep_delay,v->val_vol.air_time);

      printf("%d,%d,",v->val_vol.dist,v->val_vol.sched_arr);

      printf("%.1f,",v->val_vol.arr_delay);

      printf("%d,%d\n",v->val_vol.diverted,v->val_vol.cancel);
    }

    if(v->suiv_vol != NULL && strcmp(v->val_vol.dest_air,dest) == 0 &&
            j == v->val_vol.day && v->val_vol.month == m){

      printf("%d,%d,%d,",m,j,v->val_vol.weekday);

      printf("%s,%s,",v->val_vol.airline,v->val_vol.org_air);

      printf("%s,%d,",dest,v->val_vol.sched_dep);

      printf("%.1f,%.1f,",v->val_vol.dep_delay,v->val_vol.air_time);

      printf("%d,%d,",v->val_vol.dist,v->val_vol.sched_arr);

      printf("%.1f,",v->val_vol.arr_delay);

      printf("%d,%d\n",v->val_vol.diverted,v->val_vol.cancel);
      }
    v = v->suiv_vol;
  }
  printf("\n");
}
*/

// DONE
void find_itinerary (char org[], char dest[], Hashtable ht_date, int temps, int j, int m, int limit)
{

    // hachage par rapport a la date
    struct cell_vol *v  = ht_date[hash_fun_date (j, m)];
    int              mx = 0;

    if (v == NULL) return; // aucun vol trouve

    printf ("> find-itinerary %s %s %d-%d %d %d\n", org, dest, j, m, temps, limit);

    while (v != NULL)
    {

        // print du (des) vols qui sont partie de org
        // vers dest, en respectant la limit maximal passe en
        // parametre ainsi que la condition sur le temps
        // du depart

        if (v->val_vol.sched_dep > temps && limit > mx && (strcmp (org, v->val_vol.org_air) == 0) &&
            strcmp (dest, v->val_vol.dest_air) == 0)
        {


            printf ("%d,%d,%d,", m, j, v->val_vol.weekday);

            printf ("%s,%s,", v->val_vol.airline, org);

            printf ("%s,%d,", v->val_vol.dest_air, v->val_vol.sched_dep);

            printf ("%.1f,%.1f,", v->val_vol.dep_delay, v->val_vol.air_time);

            printf ("%d,%d,", v->val_vol.dist, v->val_vol.sched_arr);

            printf ("%.1f,", v->val_vol.arr_delay);

            printf ("%d,%d\n", v->val_vol.diverted, v->val_vol.cancel);

            mx++;
        }
        v = v->suiv_vol;
    }
    printf ("\n");
}

// DONE
void avg_flight_duration (char org[], char dest[], Hashtable ht_airport)
{

    struct cell_vol *l = ht_airport[hash_fun_airport (dest)];

    // nbe des vols en retard
    int cpt = 1;

    // temps emis
    float somme = 0;

    printf ("> avg-flight-duration %s %s\n", org, dest);


    // parcours de la liste l
    while (l != NULL)
    {

        // airport du depart et d'arrive passer en parametre
        if (strcmp (dest, l->val_vol.dest_air) == 0 && strcmp (org, l->val_vol.org_air) == 0)
        {
            cpt++;
            somme += l->val_vol.air_time;
        }
        l = l->suiv_vol;
    }
    // affichage du resultat attendue
    printf ("average: %f minutes (%d flights)\n", somme / (cpt - 1), cpt - 1);
    printf ("\n");
}

// Elle marche <3
void delayed_airline (char iata[], Hashtable h, struct cell_airlines *a)
{
    int              delay = 0;
    char             nom[50];
    int              cpt = 0;
    struct cell_vol *l   = h[hash_fun_airline (iata)];
    printf ("> delayed-airline %s\n", iata);

    if (l == NULL || a == NULL) return;

    /* recherche des airlines desirees et on les stocke dans une */
    /* varaible de type char[] note nom,  a condition qu'elle respecte bien */
    /* la condition d'avoir le meme code que celui passer */
    /* en parametre */

    while (a != NULL)
    {
        if (strcmp (iata, a->val_airl.iata_airl) == 0)
        {
            strcpy (nom, a->val_airl.airline);
        }
        a = a->suiv_airl;
    }

    // parcours de la liste et comparaison du temps si il correspond bien
    // a un temps de retard
    while (l != NULL)
    {
        if (strcmp (iata, l->val_vol.airline) == 0 && (l->val_vol.arr_delay > 0))
        {

            // addition des temps du retarts total faites par la compagnie
            // incrementation a chaque entree d'un cpt qui represente le nbe
            // des fois ou on a du retard
            delay += l->val_vol.arr_delay + l->val_vol.dep_delay;
            cpt++;
        }

        l = l->suiv_vol;
    }
    printf ("%s,%s,%d\n", iata, nom, delay / cpt);
    printf ("\n");
}

void most_delayed_fligths (Hashtable ht_al)
{

    struct cell_vol *v = NULL;
    struct cell_vol *l = NULL;

    int place = 0;
    printf ("> most-delayed-flights \n");
    while (place < TABSIZE)
    {
        l = ht_al[place];
        while (l != NULL)
        {
            ajout_trie_flight (&v, l->val_vol);
            l = l->suiv_vol;
        }
        place++;
    }
    affiche_vol (v);
    printf ("\n");
}

void most_delayed_airlines (Hashtable ht_al, struct cell_airlines *al)
{

    struct airline_delayed table[14];
    struct cell_vol *      v     = NULL;
    int                    cpt   = 0;
    float                  delay = 0;
    int                    i     = 0;

    printf ("> most-delayed-airlines \n");
    while (al != NULL)
    {

        v = ht_al[hash_fun_airline (al->val_airl.iata_airl)];
        while (v != NULL)
        {
            if (strcmp (al->val_airl.iata_airl, v->val_vol.airline) == 0)
            {
                delay = delay + v->val_vol.arr_delay;
                cpt++;
            }
            v = v->suiv_vol;
        }


        table[i].retard      = delay / cpt;
        table[i].AL.val_airl = al->val_airl;
        i++;
        cpt   = 0;
        delay = 0;

        al = al->suiv_airl;
    }

    tri_airline (table, 14);
    int j = 0;
    while (j < 5)
    {
        printf ("%s , %s\n", table[j].AL.val_airl.iata_airl, table[j].AL.val_airl.airline);
        j++;
    }

    printf ("\n");
}
