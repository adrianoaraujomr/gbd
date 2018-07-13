#include "hash.h"

#define BLOCK 10

int read_block(int nro,char *file);
int free_block();
int read_page(int nro,char *file);
int free_page();
Reg* read_slot(int nro,int bp);

int block_join(char *table1,char *table2);
int index_join(char *table1,char *table2,Hash *ht,char *val);


int block_index[BLOCK];
int page_index;
