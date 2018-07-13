#include "hash.h"

Rid* le_slot_bucket(int move,int slot,char *val){
	Rid *rid;
	FILE *fd;
	char aux[TAM_SLT];
	int i;	

	rid = (Rid*) malloc(sizeof(Rid));

	fd = fopen(val,"r");
	if(fd == NULL) return 0;

	fseek(fd,move + (slot)*3*TAM_SLT,SEEK_SET);
	for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);// new
	rid->skey = hex_to_uint(aux);		      // new 

	for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);
	rid->page = hex_to_uint(aux);

	for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);
	rid->slot = hex_to_uint(aux);

	fclose(fd);

	return rid;
}

int grava_slot_bucket(int move,int slot,Rid *rid,char *val){
	char *aux;
	FILE *fd;
	int i;	

	fd = fopen(val,"r+");
	if(fd == NULL) return 0;

	fseek(fd,move + (slot)*3*TAM_SLT,SEEK_SET);
	aux = uint_to_hex(rid->skey);
	for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd);
	free(aux);

	aux = uint_to_hex(rid->page);
	for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd);
	free(aux);

	aux = uint_to_hex(rid->slot);
	for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd);
	free(aux);

	fclose(fd);
	return 1;
}

int aloca_bucket(char *val){
	FILE *fd;
	int i;

	fd = fopen(val,"a");
	if(fd == NULL) return 0;
	for(i = 0;i < TAM_BCK;i++) fputc('0',fd);
	fclose(fd);

}

int printb(int move,char *val){
	Rid *rid;
	int i;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(move,i,val);
		printf("|<%05d;<%05d;%05d>>|",rid->skey,rid->page,rid->slot);
		free(rid);
	}

	return 1;
}

void printh(Hash *ht,char *val){ // mostra a hash table de forma humana
	int i;

	printf("     |%02d|\n",ht->pr_global);
	for(i = 0;i < pow(2,ht->pr_global);i++){
		printf("|%04d|%02d|%05d| -> ",i,ht->bucks[i].pr_local,ht->bucks[i].move);
		printb(ht->bucks[i].move,val);
		printf("\n");
	}
}

int grava_hash(Hash *ht,char *dir){ // grava hash table em um arquivo
	FILE *fd;
	int nro_bck;
	char *aux;
	int i,j;

	fd = fopen(dir,"w"); // esse arquivo armazenara o diretorio do hash
	if(fd == NULL) return 0;

	aux = uint_to_hex(ht->pr_global);
	for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd); // armazena a profundidade global
	nro_bck = pow(2,ht->pr_global);

	for(j = 0;j < nro_bck;j++) {
		aux = uint_to_hex(ht->bucks[j].move);
		for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd); // armazena o ponteiro para os dados do bucket

		aux = uint_to_hex(ht->bucks[j].pr_local);    // armazena a profundiade local
		for(i = 0;i < TAM_SLT;i++) fputc(aux[i],fd);
	}
	fclose(fd);

	return 1;
}

Hash* le_hash(char *file,char *dir){ // le hash table de um arquivo
	Hash *new;
	FILE *fd;
	int nro_bck;
	char aux[TAM_SLT];
	int i,j;

	set_max_size(file);

	new = (Hash*) malloc(sizeof(Hash));

	fd = fopen(dir,"r"); // esse arquivo armazenara o diretorio do hash
	if(fd == NULL) return 0;

	for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);
	new->pr_global = hex_to_uint(aux);
	nro_bck = pow(2,new->pr_global);

	new->bucks = (Bucket*) malloc(sizeof(Bucket)*nro_bck);	

	for(j = 0;j < nro_bck;j++) {
		for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);
		new->bucks[j].move = hex_to_uint(aux);
		
		for(i = 0;i < TAM_SLT;i++) aux[i] = fgetc(fd);
		new->bucks[j].pr_local = hex_to_uint(aux);
	}
	return new;
}

Hash* cria_hash(char *file,char *dir,char *val){ // inicializa hash table
	Hash *new;
	FILE *fd;
	char *aux;
	int move = 0;
	int i,j;

	cria_arquivo(file);
	set_max_size(file);

	new = (Hash*) malloc(sizeof(Hash));
	new->bucks = (Bucket*) malloc(sizeof(Bucket)*NRO_BCK);

	new->pr_global = 2;
	for(i = 0;i < NRO_BCK;i++) {
		new->bucks[i].move = move;
		new->bucks[i].pr_local = new->pr_global;
		move += TAM_BCK;
	}
	grava_hash(new,dir);

	fd = fopen(val,"w");
	if(fd == NULL) return 0;
	for(i = 0;i < NRO_BCK*TAM_BCK;i++) fputc('0',fd);
	fclose(fd);

	return new;	
}

int redistribui(Hash *ht,int b1,int b2,char *val){ // reditribui buckets
	Aluno *a;
	Rid *rid;
	Rid zero;
	int i,j,bck;

	bck = pow(2,ht->pr_global); // ? - em duvida se eh a global ou local
	zero.skey = 0;
	zero.page = 0;
	zero.slot = 0;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(ht->bucks[b1].move,i,val);

		j = rid->skey % bck;
		grava_slot_bucket(ht->bucks[j].move,i,rid,val);
		if(j != b1) grava_slot_bucket(ht->bucks[b1].move,i,&zero,val);
		free(rid);
	}

	return 1;
}

int dobra_dir(Hash **ht,int nro,char *val){ // dobra diretorio
	int nro_bck,aux_bck,max,i,j;
	FILE *fd;

	max = 0;

	nro_bck = pow(2,(*ht)->bucks[nro].pr_local); //nro de buckets de acordo com a profundidade local
	aux_bck = pow(2,(*ht)->pr_global);	     //nro total de buckets na hash
	nro = nro % nro_bck;			     //qual o bucket que sera dividido

	for(i = 0;i < aux_bck;i++) if((*ht)->bucks[i].move > max) max = (*ht)->bucks[i].move; // acha qual a ultima poiscao apontada

	(*ht)->bucks[nro].pr_local++;
	if((*ht)->bucks[nro].pr_local > (*ht)->pr_global){ // se tiver que duplicar diretorio
		(*ht)->pr_global++;
		(*ht)->bucks = (Bucket*) realloc((*ht)->bucks,sizeof(Bucket)*2*nro_bck);

		for(i = nro_bck;i < 2*nro_bck;i++){ // ajeita ponteiros dos novos buckets criados
			(*ht)->bucks[i].pr_local = (*ht)->bucks[i % nro_bck].pr_local;
			(*ht)->bucks[i].move     = (*ht)->bucks[i % nro_bck].move;
		}
	}
	(*ht)->bucks[nro + nro_bck].pr_local = (*ht)->bucks[nro].pr_local;	// ajeita profundiade local do bucket duplicado
	(*ht)->bucks[nro + nro_bck].move = max + TAM_BCK;			// arruma ponteiro do novo bucket

	aloca_bucket(val);
	redistribui(*ht,nro,nro + nro_bck,val);

	return 1;
}

int ht_insere(Hash *ht,int id,char* registro,char *file,char *val){ // insere um registro
	FILE *fd;
	Rid *rid;
	char aux[TAM_SLT],*pnt;
	int nro,nro_bck;
	int i,j;

	nro_bck = pow(2,ht->pr_global); // numero de buckets
	nro = id % nro_bck;		// aplicacao da funcao de hash

	for(j = 0;j < SLT_BCK;j++){
		rid = le_slot_bucket(ht->bucks[nro].move,j,val);
		if(!rid->slot){ // se o nro do slot for zero ele esta vazio
			free(rid);
			rid = escreve_registro(registro,file); // ? - problema ta aqui
			rid->skey = id;
			grava_slot_bucket(ht->bucks[nro].move,j,rid,val);
			free(rid);
			return 1;
		}
		free(rid);
	}
	dobra_dir(&ht,nro,val);
	return ht_insere(ht,id,registro,file,val);
}

int ht_busca(Hash *ht,int id,char *file,char *val){ // procura por um registro
	int nro_bck,nro,i;
	Rid *rid;
	Aluno *a;

	nro_bck = pow(2,ht->pr_global);
	nro = id % nro_bck;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(ht->bucks[nro].move,i,val);
		if(rid->skey == id){
			a = acesso_direto(rid,file);

			printf("%d %d %s\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2);
			free(a);
			return 1;
		}
		free(rid);
	}
	return 0;
}


int ht_remove(Hash *ht,int id,char *file,char *val){ // remove um registro
	int nro_bck,nro,i;
	Rid *rid,zero;
	Aluno *a;

	nro_bck = pow(2,ht->pr_global);
	nro = id % nro_bck;
	zero.skey = 0;
	zero.page = 0;
	zero.slot = 0;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(ht->bucks[nro].move,i,val);
		if(rid->skey == id){
			a = acesso_direto(rid,file);
			remove_direto(rid,file);			
			grava_slot_bucket(ht->bucks[nro].move,i,&zero,val);	
			free(rid);
			return 1;
		}
		free(rid);
	}
	return 0;
}

int ht_busca_2(Hash *ht,int id,char *file,char *val){ // procura por um registro
	int nro_bck,nro,i;
	Rid *rid;
	Aluno *a;
	struct timeval first,final;

	nro_bck = pow(2,ht->pr_global);
	nro = id % nro_bck;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(ht->bucks[nro].move,i,val);
		if(rid->skey == id){
			gettimeofday(&first, NULL);
			a = acesso_direto(rid,file);
			gettimeofday(&first, NULL);

			printf("Execuao leitura aleatoria : %lu s | %lu us\n",final.tv_sec - first.tv_sec,final.tv_usec - first.tv_usec);

			printf("%d %d %s\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2);
			free(a);
			return 1;
		}
		free(rid);
	}


	return 0;
}

