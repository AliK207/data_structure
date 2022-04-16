int hash_fun_airline (char *);
int hash_fun_date (int, int);
int hash_fun_airport (char dest_airp[3]);

void add_airlines (struct cell_airlines **, struct airls);
void add_airport (struct cell_airports **, struct airpo);
void add_flights (struct cell_vol **, struct flights);

void affiche_airl (struct cell_airlines *);
void affiche_airport (struct cell_airports *);
void load_airlines (struct cell_airlines **, FILE *);

void load_airports (struct cell_airports **, FILE *);
void init_hash (Hashtable);
void update_ht (Hashtable, struct flights);

void affiche_vol (struct cell_vol *);
void load_vol1 (Hashtable, Hashtable, Hashtable, FILE *);

int  occurence_vol (struct cell_vol *);
void dead_airlines (struct cell_airlines **);
void delete_aiport (struct cell_airports **);
void delete_vol (struct cell_vol **);
void go ();
void by_file (struct cell_airlines *, struct cell_airports *, Hashtable, Hashtable, Hashtable);
