#include "../include/data.h"
#include "../include/load_data.h"
#include "../include/requetes.h"


// fonction de hachage par % airlines
int hash_fun_airline (char *airline)
{
    int res = 0, i;
    for (i = 0; airline[i] != '\0'; ++i)
    {
        res += airline[i] - 'A' + 1;
    }

    return (res + i) % TABSIZE;
}


// fonction de hachage % date(jours, mois)
int hash_fun_date (int j, int m)
{
    int i = 0;

    if (j >= 10)
        i = (100 * m) + j;
    else
        i = (10 * m) + j;

    return (i % TABSIZE_DATE);
}

// fonction de hachage % aeroport(destination ou origine)
int hash_fun_airport (char dest_airp[3])
{


    int i = 0;

    i = 2 * (i + dest_airp[0]);
    i = i + dest_airp[1];
    i = i * dest_airp[1];
    i = 2 * (i + dest_airp[2]);

    return i % TABSIZE;
}


// ajout tete des cellule airlines dans une liste chainee
void add_airlines (struct cell_airlines **al, struct airls val)
{
    struct cell_airlines *new = malloc (sizeof (struct cell_airlines));

    new->val_airl  = val;
    new->suiv_airl = *al;
    *al            = new;
}

// ajout tete des cellule airports dans une liste chainee
void add_airport (struct cell_airports **ap, struct airpo val)
{
    struct cell_airports *new = malloc (sizeof (struct cell_airports));

    new->val_airpo  = val;
    new->suiv_airpo = *ap;
    *ap             = new;
}

// ajout tete des cellules des vols dans une liste chainee
void add_flights (struct cell_vol **fl, struct flights val)
{
    struct cell_vol *new = malloc (sizeof (struct cell_vol));

    new->val_vol  = val;
    new->suiv_vol = *fl;
    *fl           = new;
}


/* void load_airlines(FILE * f){ */

/*   char res[200]; */
/*   char x[500]; */
/*   char *tmp; */
/*   //fgets(x,500,f); */
/*   const char * stop = ",;"; */
/*   while(fgets(x,500,f) != NULL){ */
/*     tmp = strtok(x,stop); */
/*     if (tmp == NULL) { */
/*       printf("Pas de CODE\n"); */
/*     } else { */
/*       strcpy(res, tmp); */
/*       printf("%s      "); */
/*     } */
/*     tmp = strtok(NULL,stop); */
/*     if (tmp == NULL) { */
/*       printf("Pas de SDJF\n"); */
/*     } else { */
/*       strcpy(res, tmp); */
/*       printf("%s\n",res); */
/*     }     */
/*   } */
/* } */


// affichage liste chainee airlines
void affiche_airl (struct cell_airlines *l)
{
    while (l != NULL)
    {
        printf ("%s , %s\n", l->val_airl.iata_airl, l->val_airl.airline);
        l = l->suiv_airl;
    }
}

// affichage liste chainee airports
void affiche_airport (struct cell_airports *l)
{
    while (l != NULL)
    {
        printf ("%s,%s,%s,%s,%s,%f,%f\n", l->val_airpo.iata_airp, l->val_airpo.airport, l->val_airpo.city,
                l->val_airpo.state, l->val_airpo.country, l->val_airpo.latitude, l->val_airpo.longitude);
        l = l->suiv_airpo;
    }
}

// chargement des donnees du fichier airlines.csv dans une liste chainee
void load_airlines (struct cell_airlines **pl, FILE *f)
{

    char  x[N];
    char *tmp;
    struct airls new;
    const char *stop  = ",";
    const char *stop1 = "\n";

    if (f == NULL) return;

    fscanf (f, "%*[^\n]\n");

    while (fgets (x, 500, f) != NULL)
    {
        tmp = strtok (x, stop);
        if (tmp == NULL)
            printf ("Pas de CODE pour AIRLINES\n");
        else
            strcpy (new.iata_airl, tmp);

        tmp = strtok (NULL, stop1);

        if (tmp == NULL)
            printf ("Pas de SDJF\n");

        else
            strcpy (new.airline, tmp);

        add_airlines (pl, new);
    }
}

// chargement des donnees du fichier airports.csv dans une liste chainee
void load_airports (struct cell_airports **pl, FILE *f)
{

    char  x[N];
    char *tmp;
    struct airpo new;
    const char *stop  = ",";
    const char *stop1 = "\n";
    if (f == NULL) return;

    fscanf (f, "%*[^\n]\n");

    while (fgets (x, 800, f) != NULL)
    {

        tmp = strtok (x, stop);
        strcpy (new.iata_airp, tmp);

        tmp = strtok (NULL, stop);
        strcpy (new.airport, tmp);

        tmp = strtok (NULL, stop);
        strcpy (new.city, tmp);

        tmp = strtok (NULL, stop);
        strcpy (new.state, tmp);

        tmp = strtok (NULL, stop);
        strcpy (new.country, tmp);

        tmp = strtok (NULL, stop);
        if (tmp == NULL)
            new.latitude = 0.0;
        else
            new.latitude = atof (tmp);

        tmp = strtok (NULL, stop1);

        if (tmp == NULL)
            new.longitude = 0.0;
        else
            new.longitude = atof (tmp);

        add_airport (pl, new);
    }
}


void init_hash (Hashtable ht)
{

    for (int i = 0; i < TABSIZE; ++i) ht[i] = NULL;
}

// fonction non-utilise dans notre projet
// peut servir a faire des ajouts de tete dans une hache table
void update_ht (Hashtable ht, struct flights new)
{
    add_flights (&(ht[hash_fun_airline (new.airline)]), new);
}

// affichage des cellule d'une liste chainee des vol
void affiche_vol (struct cell_vol *l)
{
    while (l != NULL)
    {
        printf ("%d,%d,%d,", l->val_vol.month, l->val_vol.day, l->val_vol.weekday);
        printf ("%s,%s,%s,", l->val_vol.airline, l->val_vol.org_air, l->val_vol.dest_air);
        printf ("%d,%.1f,%.1f,", l->val_vol.sched_dep, l->val_vol.dep_delay, l->val_vol.air_time);
        printf ("%d,%d,%.1f,", l->val_vol.dist, l->val_vol.sched_arr, l->val_vol.arr_delay);
        printf ("%d,%d\n", l->val_vol.diverted, l->val_vol.cancel);
        l = l->suiv_vol;
    }
}

// chargement des donnees du fichier flights.csv dans une liste chainee
void load_vol1 (Hashtable ht_al, Hashtable ht_dest, Hashtable ht_date, FILE *f)
{

    if (f == NULL) return;

    struct flights new;
    char tmp[60];

    if (fscanf (f, "%*[^\n]\n") != 0) return;
    while (fscanf (f, "%d,", &(new.month)) == 1)
    {

        if (fscanf (f, "%d,", &(new.day)) != 1) return;
        if (fscanf (f, "%d,", &(new.weekday)) != 1) return;
        if (fscanf (f, "%2s,", new.airline) != 1) return;
        if (fscanf (f, "%3s,", new.org_air) != 1) return;
        if (fscanf (f, "%3s,", new.dest_air) != 1) return;
        if (fscanf (f, "%d,", &(new.sched_dep)) != 1) return;

        if (fscanf (f, "%[^,]s,", tmp) != 1)
        {
            tmp[0] = '0';
            tmp[1] = '\0';
        }
        new.dep_delay = atof (tmp);

        if (fscanf (f, ",%[^,]s,", tmp) != 1)
        {
            tmp[0] = '0';
            tmp[1] = '\0';
        }
        new.air_time = atof (tmp);

        if (fscanf (f, ",%d,", &(new.dist)) != 1) return;
        if (fscanf (f, "%d,", &(new.sched_arr)) != 1) return;

        if (fscanf (f, "%[^,]s,", tmp) != 1)
        {
            tmp[0] = '0';
            tmp[1] = '\0';
        }
        new.arr_delay = atof (tmp);

        if (fscanf (f, ",%d,", &(new.diverted)) != 1) return;
        if (fscanf (f, "%d\n", &(new.cancel)) != 1) return;

        add_flights (&(ht_al[hash_fun_airline (new.airline)]), new);
        add_flights (&(ht_date[hash_fun_date (new.day, new.month)]), new);
        add_flights (&(ht_dest[hash_fun_airport (new.dest_air)]), new);
    }
}

// comptage des occurences (cellules) d'une liste chainee des vols
int occurence_vol (struct cell_vol *l)
{
    int occu = 0;

    while (l != NULL)
    {
        occu++;
        l = l->suiv_vol;
    }

    return occu;
}

// destruction d'un liste chainee des airlines & liberation de memoire
void dead_airlines (struct cell_airlines **pl)
{
    if (*pl == NULL) return;
    struct cell_airlines *fin = *pl;
    while (*pl != NULL)
    {
        fin = *pl;
        *pl = (*pl)->suiv_airl;
        free (fin);
    }
}

// destruction d'un liste chainee des airports & liberation de memoire
void delete_aiport (struct cell_airports **pl)
{

    if (*pl == NULL) return;

    struct cell_airports *tmp = NULL;

    while (*pl != NULL)
    {
        tmp   = *pl;
        (*pl) = (*pl)->suiv_airpo;
        free (tmp);
    }
}


// destruction d'un liste chainee des vols & liberation de memoire
void delete_vol (struct cell_vol **pl)
{

    if (*pl == NULL) return;
    struct cell_vol *tmp = NULL;

    while (*pl != NULL)
    {
        tmp = *pl;
        *pl = (*pl)->suiv_vol;
        free (tmp);
    }
}

/*                                 Fonction utilise dans le main pour saisir les requetes par un fichier                             */

void go ()
{
    printf ("Bonjour !\n");
    printf ("Bienvenu dans notre Projet de PA\n");
    printf ("Pour executer un fichier des requetes : entrez 1\nPour introduire mauellement des "
            "requetes entrez autre numero\n");
    printf ("A vous maintenant : ");
}

void by_file (struct cell_airlines *a, struct cell_airports *p, Hashtable ht_al, Hashtable ht_dest, Hashtable ht_date)
{

    char *token;
    char  mot[80], dest[5], orig[5], id_airline[5], id_airline1[3];
    int   mois, jours, temps, limit;

    FILE *f;
    f = fopen ("data/requetes.txt", "r");

    if (f == NULL)
    {
        printf ("Echec d'ouverture du fichier des requetes .txt\n");
        return;
    }

    while (fgets (mot, 50, f) != NULL)
    {

        // printf("le mots = %s\n",mot);

        if (strcmp (mot, "q") == 0)
        {
            printf ("vous avez entre 'q' ! Vous avez quitte\n");
            return;
        }

        token = strtok (mot, " ");

        if (strcmp (mot, "show-airports") == 0)
        {

            while (token != NULL)
            {
                strcpy (id_airline, token);
                // printf("%s\n",token);
                token = strtok (NULL, " -");
            }
            // printf("show-airports --- : %s\n",id_airline);
            show_airports (id_airline, ht_al, p);
            continue;
        }

        if (strcmp (mot, "show-airlines") == 0)
        {
            while (token != NULL)
            {
                strcpy (orig, token);
                token = strtok (NULL, " -");
            }
            // printf("show-airlines --- : %s \n",orig);
            show_airlines (orig, a, ht_dest);
            continue;
        }


        if (strcmp (mot, "show-flights") == 0)
        {
            while (token != NULL)
            {
                token = strtok (NULL, " -");
                strcpy (orig, token);

                // token = strtok(NULL," -");
                // strcpy(dest,token);

                token = strtok (NULL, " -");
                mois  = atoi (token);
                token = strtok (NULL, " -");
                jours = atoi (token);
                token = strtok (NULL, " -");
            }
            // printf("show-flights ----- orig : %s, dest : %s, %d/%d\n",orig,dest,jours,mois);
            show_flights (orig, jours, mois, ht_date);
            continue;
        }

        if (strcmp (mot, "most-delayed-flights") == 0)
        {
            while (token != NULL) token = strtok (NULL, " -");

            // printf("appelle la fonction most_delayed_flights\n");
            most_delayed_fligths (ht_al);
            continue;
        }

        if (strcmp (mot, "most-delayed-airlines") == 0)
        {
            // while(token != NULL)
            // token = strtok(NULL," -");
            // printf("appelle la fonction most_delayed_airlines\n");
            most_delayed_airlines (ht_al, a);
            continue;
        }

        if (strcmp (mot, "delayed-airline") == 0)
        {
            while (token != NULL)
            {
                strcpy (id_airline1, token);
                token = strtok (NULL, " -");
            }
            // printf("id_airline D : %s\n",id_airline);
            delayed_airline (id_airline1, ht_al, a);
            continue;
        }

        if (strcmp (mot, "changed-flights") == 0)
        {
            while (token != NULL)
            {
                token = strtok (NULL, " -");
                mois  = atoi (token);
                token = strtok (NULL, " -");
                jours = atoi (token);
                token = strtok (NULL, " -");
            }
            // printf("change --- : %d/%d \n",jours,mois);
            changed_flights (mois, jours, ht_date);
            continue;
        }

        if (strcmp (mot, "avg-flight-duration") == 0)
        {
            while (token != NULL)
            {
                token = strtok (NULL, " -");
                strcpy (orig, token);
                token = strtok (NULL, " -");
                strcpy (dest, token);
                token = strtok (NULL, " -");
            }
            // printf("duration --- : %s > %s\n",orig,dest);
            avg_flight_duration (orig, dest, ht_dest);
            continue;
        }

        if (strcmp (mot, "find-itinerary") == 0)
        {
            while (token != NULL)
            {
                token = strtok (NULL, " -");
                strcpy (orig, token);
                token = strtok (NULL, " -");
                strcpy (dest, token);
                token = strtok (NULL, " -");
                mois  = atoi (token);
                token = strtok (NULL, " -");
                jours = atoi (token);
                token = strtok (NULL, " -");
                temps = atoi (token);
                token = strtok (NULL, " -");
                limit = atoi (token);
                token = strtok (NULL, " -");
            }
            // printf("find ----: %s > %s , %d/%d\n",orig,dest,jours,mois);

            find_itinerary (orig, dest, ht_date, temps, mois, jours, limit);
            continue;
        }
    }


    fclose (f);
    printf ("Le fichier est termine !\nMerci Pour avoir utiliser notre application\n");
    printf ("Ali Khalaf & Florian Prin\nEleve en Systemes Embarques - Polytech Lille\n");
}
