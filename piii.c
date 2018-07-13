#include "piii.h"

//char bp_page[TAM_PGN]; //abstracao de uma pagina no buffer pool
//int size_table;        //numero de paginas no arquivo
//char buffer_pool[BP_SIZE][TAM_PGN + 1];

int read_block(int nro,char *file){
	FILE *fd;
	char c;
	int i;

	for(i = 0;i < BLOCK;i++) block_index[i] = aloca_buffer();

	fd = fopen(file,"r");
	if(fd == NULL) return -1;
	fseek(fd,nro*TAM_PGN,SEEK_SET);

	for(i = 0;i < BLOCK;i++){
		c = fgetc(fd);
		if(c == EOF) {
			fclose(fd);
			while(i < BLOCK) {
				free_buffer(block_index[i]);
				block_index[i] = -1;
				i++;
			}
			return 2;
		}

		buffer_pool[block_index[i]][0] = c;
		fgets(buffer_pool[block_index[i]] + 1,TAM_PGN,fd);
	}
	return 1;
}

int free_block(){
	int i = 0;

	while(block_index[i] != -1 && i < BLOCK){
		free_buffer(block_index[i]);
		block_index[i] = -1;
		i++;
	}
}

int read_page(int nro,char *file){
	FILE *fd;
	char c;
	int i;

	page_index = aloca_buffer();
	fd = fopen(file,"r");

	if(fd == NULL) return -1;
	fseek(fd,nro*TAM_PGN,SEEK_SET);

	c = fgetc(fd);
	if(c == EOF) {
		fclose(fd);
		return 0;
	}

	buffer_pool[page_index][0] = c;
	fgets(buffer_pool[page_index] + 1,TAM_PGN,fd);
	fclose(fd);

	return 1;
}

int free_page(){
	free_buffer(page_index);
	page_index = -1;

	return 1;
}

Reg* read_slot(int nro,int bp){
	int i,j;
	char aux[TAM_SLT + 1];
	Reg *out;

	out = (Reg*) malloc(sizeof(Reg));

	i = TAM_PGN - 2*(nro + 1)*TAM_SLT;

	j = 0;
	while(j < TAM_SLT) aux[j++] = buffer_pool[bp][i++];
	aux[TAM_SLT] = '\0';
	out->move = hex_to_uint(aux);

	j = 0;
	while(j < TAM_SLT) aux[j++] = buffer_pool[bp][i++];
	aux[TAM_SLT] ='\0';
	out->size = hex_to_uint(aux);

	return out;
}

Aluno* read_register(int move, int size, int bp){ // retorna o valor de um registro dada uma posicao e tamaho
	int i,j;
	char aux[TAM_SLT];
	Aluno *std;
		
	std = (Aluno*) malloc(sizeof(Aluno));
	std->nome = (char*) malloc(sizeof(char)*(size + 1));
	std->nome[size] = '\0';
	
	i = move;
	for(j = 0;j < TAM_SLT;j++) aux[j] = buffer_pool[bp][i++];
	std->id = hex_to_uint(aux);

	for(j = 0;j < TAM_SLT/2;j++) aux[j] = buffer_pool[bp][i++];
	std->idade = hex_to_int(aux);

	i = move;
	for(j = 0;j < size;j++) std->nome[j] = buffer_pool[bp][i++];

	return std;
}

Aluno* ht_search(Hash *ht,int id,char *file,char *val){ // procura por um registro
	int nro_bck,nro,i;
	Rid *rid;
	Aluno *a;

	nro_bck = pow(2,ht->pr_global);
	nro = id % nro_bck;

	for(i = 0;i < SLT_BCK;i++){
		rid = le_slot_bucket(ht->bucks[nro].move,i,val);
		if(rid->skey == id){
			a = acesso_direto(rid,file);
			return a;
		}
		free(rid);
	}
	return NULL;
}

int block_join(char *table1,char *table2){ // nao ta lendo ultima pagina
	int sizet1,sizet2,s1,s2,i;

	init_bm();

	set_max_size(table1);
	sizet1 = size_table;

	set_max_size(table2);
	sizet2 = size_table;

	s1 = 0;
	s2 = 0;

	printf("s1 : %d s2 : %d\n",sizet1,sizet2);
	while(s1 < sizet1){// percorre table 1 por blocos
		read_block(s1,table1);

		while(s2 <= sizet2){// percorre table 2 pagina a pagina
			Aluno *b;
			Reg *tnp;
			int slts,k;

			read_page(s2,table2);
			tnp = read_slot(0,page_index);
			slts = tnp->size;
			free(tnp);
			for(k = 1;k <= slts ;k++) {
				tnp = read_slot(k,page_index);
				if(tnp->move != -1){
					b = read_register(tnp->move,tnp->size,page_index);

					for(i = 0;block_index[i] != -1 && i < BLOCK;i++){
						Reg *pnt;
						Aluno *a;
						int slots,j;

						pnt = read_slot(0,block_index[i]);
						slots = pnt->size;
						free(pnt);
						for(j = 1;j <= slots;j++){
							pnt = read_slot(j,block_index[i]);
							if(pnt->move != -1) {
								a = read_register(pnt->move,pnt->size,block_index[i]);	
if(a->id == b->id) printf("%d %d %s %d %s\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2,b->idade,b->nome + TAM_SLT + TAM_SLT/2); 
								free(a->nome);
								free(a);
							}
							free(pnt);
						}
					}
					free(b->nome);
					free(b);
				}
				free(tnp);			
			}
			free_page();
			s2++;
		}
		free_block();
		s2 = 0;
		s1 += BLOCK;
	}
}

int index_join(char *table1,char *table2,Hash *ht,char *val){// tambem parece nao ta lendo a ultima pagina
	int size;

	init_bm();	

	set_max_size(table1);
	size = 0;

	for(size = 0;size <= size_table;size++){
		Aluno *b,*a;
		Reg *tnp;
		int slts,k;

		read_page(size,table1);		
		tnp = read_slot(0,page_index);
		slts = tnp->size;
		free(tnp);

		for(k = 1;k <= slts ;k++) {
			tnp = read_slot(k,page_index);
			if(tnp->move != -1){
				a = read_register(tnp->move,tnp->size,page_index);
				b = ht_search(ht,a->id,table2,val);
if(b != NULL) printf("%d %d %s %d %s\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2,b->idade,b->nome + TAM_SLT + TAM_SLT/2);
				free(a);
				free(b);
			}		
		}

		free_page();
	}
}
