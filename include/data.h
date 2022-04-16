#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABSIZE_DATE 600
#define TABSIZE 350
#define TABSIZE_AP 300
#define N 800


struct flights
{
    int   month;
    int   day;
    int   weekday;
    char  airline[4];
    char  org_air[4];
    char  dest_air[4];
    int   sched_dep;
    float dep_delay;
    float air_time;
    int   dist;
    int   sched_arr;
    float arr_delay;
    int   diverted;
    int   cancel;
};

struct cell_vol
{
    struct flights   val_vol;
    struct cell_vol *suiv_vol;
};

struct airpo
{
    char  iata_airp[4];
    char  airport[50];
    char  city[70];
    char  state[3];
    char  country[4];
    float latitude;
    float longitude;
};

struct cell_airports
{
    struct airpo          val_airpo;
    struct cell_airports *suiv_airpo;
};

struct airls
{
    char iata_airl[3];
    char airline[50];
};

struct cell_airlines
{
    struct airls          val_airl;
    struct cell_airlines *suiv_airl;
};

struct airline_delayed
{
    float                retard;
    struct cell_airlines AL;
};

typedef struct cell_vol *Hashtable[TABSIZE];
