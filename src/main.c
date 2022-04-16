#include "../include/data.h"
#include "../include/load_data.h"
#include "../include/requetes.h"

int main ()
{

    FILE *f1;
    FILE *f2;
    FILE *f3;
    FILE *fp;

    struct cell_airlines *a = NULL;
    struct cell_airports *p = NULL;
    Hashtable             ht_al, ht_dest, ht_date;

    init_hash (ht_al);
    init_hash (ht_dest);
    init_hash (ht_date);


    f1 = fopen ("data/airlines.csv", "r");
    f2 = fopen ("data/airports.csv", "r");
    f3 = fopen ("data/flights.csv", "r");
    fp = fopen ("data/requetes.txt", "r");
    if (f3 == NULL || f2 == NULL || f1 == NULL || fp == NULL)
    {
        fprintf (stderr, "Erreur, file does not exist\n");
        return 1;
    }

    load_airlines (&a, f1);
    load_airports (&p, f2);
    load_vol1 (ht_al, ht_dest, ht_date, f3);

    fclose (f1);
    fclose (f2);
    fclose (f3);

    int choix;

    // fonction bienvenue
    go ();

    scanf ("%d", &choix);

    if (choix == 1)
    {
        by_file (a, p, ht_al, ht_dest, ht_date);
    }


    /*************************************************************************/

    // saisi des requetes et de leurs parametres manuellement
    else
    {
        char requete[80], org[6], dest[6], id_airline[6];
        int  jour, mois, limit, temps;


        while (scanf ("%s", requete) == 1 && strcmp (requete, "q") != 0 && strcmp (requete, "quit") != 0)
        {

            if (strcmp (requete, "show-airports") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> show-airports ");
                scanf ("%s", id_airline);
                show_airports (id_airline, ht_al, p);
                continue;
            }

            if (strcmp (requete, "show-airlines") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> show-airlines ");
                scanf ("%s", org);
                printf ("\n");
                show_airlines (org, a, ht_dest);
                continue;
            }

            if (strcmp (requete, "show-flights") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> show-flights ");
                scanf ("%s %d-%d", org, &mois, &jour);
                printf ("\n");
                show_flights (org, jour, mois, ht_date);
                continue;
            }

            if (strcmp (requete, "most-delayed-flights") == 0)
            {
                printf ("> most-delayed-flights \n");
                printf ("\n");
                most_delayed_fligths (ht_al);
                continue;
            }

            if (strcmp (requete, "most-delayed-airlines") == 0)
            {
                printf ("> most-delayed-airlines \n");
                printf ("\n");
                most_delayed_airlines (ht_al, a);
                continue;
            }

            if (strcmp (requete, "delayed-airline") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> delayed-airline ");
                scanf ("%s", id_airline);
                printf ("\n");
                delayed_airline (id_airline, ht_al, a);
                continue;
            }

            if (strcmp (requete, "changed-flights") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> changed-flights ");
                scanf ("%d-%d", &mois, &jour);
                printf ("\n");
                changed_flights (mois, jour, ht_date);
                continue;
            }

            if (strcmp (requete, "avg-flight-duration") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> avg-flight-duration ");
                scanf ("%s %s", org, dest);
                printf ("\n");
                avg_flight_duration (org, dest, ht_dest);
                continue;
            }

            if (strcmp (requete, "find-itinerary") == 0)
            {
                printf ("entrez les parametres :\n");
                printf ("> find-itinerary ");
                scanf ("%s %s %d-%d %d %d", org, dest, &mois, &jour, &temps, &limit);
                printf ("\n");
                find_itinerary (org, dest, ht_date, temps, mois, jour, limit);
                continue;
            }
        }
    }

    printf ("Le fichier est termine !\nMerci Pour avoir utiliser notre application\n");
    printf ("Ali Khalaf & Florian Prin\nEleves en Systemes Embarques - Polytech Lille\n");

    printf ("\n");

    dead_airlines (&a);
    delete_aiport (&p);

    init_hash (ht_date);
    init_hash (ht_dest);
    init_hash (ht_al);


    return 0;
}
