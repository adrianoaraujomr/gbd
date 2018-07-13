#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "buff.h"

#define NRO_BCK 4      //numero inicial de  buckets
#define SLT_BCK 4      //numero de slots bucket
#define TAM_BCK 96

typedef struct {
	int move;  // onde seus dados estao localizados no arquivo h_values
	int pr_local;       // profundidade local
} Bucket;

typedef struct {
	int pr_global;      // profundiade global do hash (função hash % 2^pr_global)
	Bucket *bucks;      // vetor de buckets
} Hash;

int grava_hash(Hash *ht,char *dir);
Hash* le_hash(char *file,char *dir);
Hash* cria_hash(char *file,char *dir,char *val);

Rid* le_slot_bucket(int move,int slot,char *val);
int grava_slot_bucket(int move,int slot,Rid *rid,char *val);
int aloca_bucket(char *val);
int printb(int move,char *val);
void printh(Hash *ht,char *val);

int redistribui(Hash *ht,int b1,int b2,char *val);
int dobra_dir(Hash **ht,int nro,char *val);

int ht_insere(Hash *ht,int id,char* registro,char *file,char *val);
int ht_busca(Hash *ht,int id,char *file,char *val);
int ht_remove(Hash *ht,int id,char *file,char *val);

int ht_busca_2(Hash *ht,int id,char *file,char *val);
