#include "heap.h"

#define BP_SIZE 12

typedef struct{
	int head,tail;
	int data[BP_SIZE];
} Queue;

typedef struct{
	Queue lru;
	int least;
	int pin_count[BP_SIZE];
} Bmanager;

void init_queue();
void printq();
int put_queue(int put);
int get_queue();

void init_bm();
void bp_status();
int aloca_buffer();
int free_buffer(int index);

Bmanager bm;
char buffer_pool[BP_SIZE][TAM_PGN + 1];
