#include "buff.h"

/* Queue ----------------------------------------------------------------------------------------------------------------------------------------*/

void init_queue(){
	int i;

	for(i = 0;i < BP_SIZE;i++) bm.lru.data[i] = i;
	bm.lru.head = 0;
	bm.lru.tail = BP_SIZE - 1;
}

void printq(){
	int i;

	for(i = bm.lru.head;i <= bm.lru.tail;i++) printf("| %03d ",bm.lru.data[i]);
	printf("|\n");
}

int put_queue(int put){
	int i;

	if(bm.lru.tail < BP_SIZE - 1){
		bm.lru.tail++;
		bm.lru.data[bm.lru.tail] = put;
		return 1;
	}

	return 0;
}

int get_queue(){
	int aux,i;

	if(bm.lru.tail < 0) return -1;

	aux = bm.lru.data[bm.lru.head];
	for(i = bm.lru.head;i < bm.lru.tail;i++) bm.lru.data[i] = bm.lru.data[i+1];
	bm.lru.tail--;

	return aux;
}

/* Buffer Manager -------------------------------------------------------------------------------------------------------------------------------*/

void init_bm(){
	int i;

	for(i = 0;i < BP_SIZE;i++){
		bm.pin_count[i] = 0;
	}

	bm.least = 0;
	init_queue();
}

void bp_status(){
	int i;

	printq();
	for(i = 0;i < BP_SIZE;i++) if(bm.pin_count[i]) printf("[%d] %d\n",i,bm.pin_count[i]);
}

int aloca_buffer(){
	int index;

	index = get_queue();
	bm.pin_count[index]++;

	return index;
}

int free_buffer(int index){
	FILE *fd;
	int i;

	bm.pin_count[index]--;

	if(!bm.pin_count[index]) return put_queue(index);
	return 1;
}
