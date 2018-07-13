#include "heap.h"

char* int_to_hex(int val){ //small int em hexadeciaml
	char *str;
	int i = 3;
	int aux;
	str = (char*) malloc(sizeof(char)*5);
	str[4] = '\0';
	while(i >= 0){
		aux = val % 16;
		if(aux < 10) str[i] = aux + '0';
		else str[i] = aux%10 + 'A';
		val = val/16;
		i--;
	}
	return str;
}

int hex_to_int(char* hex){ //hexadeciaml para samll int
	int tot = 0;
	int i = 3;
	int aux = 1;
	while(i >= 0){
		if(isdigit(hex[i])){
			tot += aux*(hex[i] - 48);
			aux *= 16;
		} else {
			tot += aux*(hex[i] - 55);
			aux *= 16;
		}
		i--;
	}
	return tot;
}

char* uint_to_hex(int val){// unsigned int para string hexadeciaml
	char *str;
	int i = 7;
	int aux;
	str = (char*) malloc(sizeof(char)*9);
	str[8] = '\0';
	if(val == -1) {
		for(i = 0;i < 8;i++) str[i] = 'X';
		return str;
	}
	while(i >= 0){
		aux = val % 16;
		if(aux < 10) str[i] = aux + '0';
		else str[i] = aux%10 + 'A';
		val = val/16;
		i--;
	}
	return str;
}

int hex_to_uint(char* hex){// string hexadeciaml para  int
	int tot = 0;
	int i = 7;
	int aux = 1;
	if(hex[0] == 'X') return -1; // quer dizer que um registro foi excluido
	while(i >= 0){
		if(isdigit(hex[i])){
			tot += aux*(hex[i] - 48);
			aux *= 16;
		} else {
			tot += aux*(hex[i] - 55);
			aux *= 16;
		}
		i--;
	}
	return tot;
}

char* cria_registro(int id,int age,char* name){ // cria uma string do registro que sera armazenada no arquivo
	int size = strlen(name) + 3*sizeof(int);
	char *aux;
	aux = (char*) malloc(sizeof(char)*size);
	strcpy(aux,uint_to_hex(id ));
	strcpy(aux+2*sizeof(int),int_to_hex(age));
	strcpy(aux+3*sizeof(int),name);
	return aux;
}

/* Paginas --------------------------------------------------------------------------------------------------------------------------------------*/

void cria_arquivo(char *file){
	FILE *fd;

//	fd = fopen("try.txt","w");
	fd = fopen(file,"w");
	fclose(fd);
	aloca_pagina(file);
	size_table = 0;
}

void set_max_size(char *file){ // descobre quantas paginas tem o arquivo executar uma vez, quando inicia o programa
	int page = 0;
	while(1){
		if(!le_pagina(page,file)) break;
		page++;
	}
	size_table = page - 1;	
}

int aloca_pagina(char *file){ // alocar uma pagina no arquivo
	FILE *fd;
	int i = 0;

//	fd = fopen("try.txt","a");
	fd = fopen(file,"a");

	if(fd != NULL){
		for(i = 0;i < TAM_PGN;i++) fputc('0',fd);
		fclose(fd);
		size_table++;

		return 1;		
	}
	else {
		printf("HOJE NAO BB\n");
		return 0;
	}
}

int le_pagina(int nro,char *file){ // ler uma pagina do arquivo e armazenar em bp_page
	FILE *fd;
	char c;
	int i;

	fd = fopen(file,"r");

	if(fd == NULL) return -1;
	fseek(fd,nro*TAM_PGN,SEEK_SET);

	c = fgetc(fd);
	if(c == EOF) {
		fclose(fd);
		return 0;
	}

	bp_page[0] = c;
	fgets(bp_page + 1,TAM_PGN,fd);
	fclose(fd);

	return 1;
}

int escreve_pagina(int nro,char *file){ // escreve o que esta em bp_page no arquivo
	FILE *fd;
	int i;

//	fd = fopen("try.txt","r+");
	fd = fopen(file,"r+");
	if(fd == NULL) return -1;

	fseek(fd,nro*TAM_PGN*sizeof(char),SEEK_SET);
	for(i = 0;i < TAM_PGN;i++) fputc(bp_page[i],fd);

	fclose(fd);

	return 1;
}

Reg* le_slot(int nro){ // retorna o tamanho e onde comeca um registro dado o numero do slot
	int i,j;
	char aux[TAM_SLT];
	Reg *out;

	out = (Reg*) malloc(sizeof(Reg));

	i = TAM_PGN - 2*(nro + 1)*TAM_SLT;

	j = 0;
	while(j < TAM_SLT) aux[j++] = bp_page[i++];
	out->move = hex_to_uint(aux);

	j = 0;
	while(j < TAM_SLT) aux[j++] = bp_page[i++];
	out->size = hex_to_uint(aux);

	return out;
}

void grava_slot(int nro,int slt1,int slt2){ // grava o tamaho e onde comeca um registro
	int i,j;
	char *pnt;

	i = TAM_PGN - 2*(nro + 1)*TAM_SLT;

	pnt = uint_to_hex(slt1); // ponteiro para o registro
	for(j = 0;j < TAM_SLT;j++) bp_page[i++] = pnt[j];	

	pnt = uint_to_hex(slt2); // tamanho do registro
	for(j = 0;j < TAM_SLT;j++) bp_page[i++] = pnt[j];
}

Aluno* le_registro(int move, int size){ // retorna o valor de um registro dada uma posicao e tamaho
	int i,j;
	char aux[TAM_SLT];
	Aluno *std;
		
	std = (Aluno*) malloc(sizeof(Aluno));
	std->nome = (char*) malloc(sizeof(char)*(size + 1));
	std->nome[size] = '\0';
	
	i = move;
	for(j = 0;j < TAM_SLT;j++) aux[j] = bp_page[i++];
	std->id = hex_to_uint(aux);

	for(j = 0;j < TAM_SLT/2;j++) aux[j] = bp_page[i++];
	std->idade = hex_to_int(aux);

	i = move;
	for(j = 0;j < size;j++) std->nome[j] = bp_page[i++];

	return std;
}

/* Heap file operações --------------------------------------------------------------------------------------------------------------------------*/

Rid* escreve_registro(char* registro,char *file){ // escreve um registro no arquivo head (final)
	Rid *rid;
	Reg *reg;
	int slots,vazio;
	int i,j;

	if(le_pagina(size_table,file) == -1) printf("Erro ao ler pagina. Abortando ...\n");
	reg = le_slot(0);
	vazio = reg->move;
	slots = reg->size;
	free(reg);

	if(strlen(registro) + (vazio + 2*TAM_SLT*(2 + slots)) > TAM_PGN){ // se nao tiver espaco na pagina
		if(!aloca_pagina(file)) printf("Erro ao alocar pagina.\n");
		return escreve_registro(registro,file);
	} 

	/* Aloca registro*/
	for(i = vazio;(i - vazio) < strlen(registro);i++) bp_page[i] = registro[i - vazio];
	slots += 1;

	grava_slot(slots,vazio,strlen(registro)); // slot do registro
	vazio += strlen(registro);

	grava_slot(0,vazio,slots); // atualiza cabecalho pagina

	rid = (Rid*) malloc(sizeof(Rid));
	rid->page = size_table;
	rid->slot = slots;

	if(escreve_pagina(size_table,file) == -1) printf("Erro ao escrever pagina\n");

	return rid; // retorna rid do registro inserido
}

int varredura_sequencial(char *file){ // varre sequencialmente o arquivo heap
	int page;
	int slots;
	int i,j,k;
	char *pnt;
	Reg *fck;
	Aluno *a;

	for(page = 0;page <= size_table;page++){
		le_pagina(page,file);

 		/* Le cabecalho pagina*/
		fck = le_slot(0);
		slots = fck->size;

		/* Percorre a pagina printando o que tem em cada slot*/
		for(k = 1;k <= slots;k++) {
			fck = le_slot(k);
			if(fck->move != -1) {
				a = le_registro(fck->move,fck->size);	
				printf("| %d | %d | %s |\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2);
				free(a->nome);
				free(a);
			}
			free(fck);
		}
	}
	return 1;
}

int remove_registro(int id,char *file){ //remove um registro com certo id, do arquivo heap
	int page = 0;
	int slots,vazio;
	int k;
	Reg *fck;
	Aluno *a;

	for(page = 0;page <= size_table;page++){
		le_pagina(page,file);

 		/* Le cabecalho pagina*/
		fck = le_slot(0);
		slots = fck->size;
		vazio = fck->move;
		free(fck);

		for(k = 1;k <= slots;k++) {
			fck = le_slot(k);
			if(fck->move != -1) {
				a = le_registro(fck->move,fck->size);
				if(id == a->id) {
					grava_slot(k,-1,fck->size);
					escreve_pagina(page,file);
					return 1;
				}
				free(a->nome);
				free(a);	
			}
			free(fck);
		}
	}
	return 0;
}

Rid* atualizar_registro(int old,char *new,char *file){ // atualiza um registro
	remove_registro(old,file);
	return escreve_registro(new,file);
}

/* Parte II -------------------------------------------------------------------------------------------------------------------------------------*/

Aluno* acesso_direto(Rid *rid,char *file){ // retorna um registro dado uma certa posicao
	int slots,vazio,i;
	Reg *fck;
	Aluno *a;	

	le_pagina(rid->page,file);
	fck = le_slot(rid->slot);
	a = le_registro(fck->move,fck->size);

	return a;
}

void remove_direto(Rid *rid,char *file){ // remove um registro dado uma certa posicao
	int slots,vazio,i;
	Reg *fck;
	Aluno *a;	

	le_pagina(rid->page,file);
	grava_slot(rid->slot,-1,fck->size);
	escreve_pagina(rid->page,file);

}

/* Parte IV -------------------------------------------------------------------------------------------------------------------------------------*/

int leitura_sequencial(int id,char *file){
	int page;
	int slots;
	int i,j,k;
	char *pnt;
	Reg *fck;
	Aluno *a;
	struct timeval first,final;

	gettimeofday(&first, NULL);
	for(page = 0;page <= size_table;page++){
		le_pagina(page,file);

 		/* Le cabecalho pagina*/
		fck = le_slot(0);
		slots = fck->size;

		/* Percorre a pagina printando o que tem em cada slot*/
		for(k = 1;k <= slots;k++) {
			fck = le_slot(k);
			if(fck->move != -1) {
				a = le_registro(fck->move,fck->size);	
				if(a->id == id) {
					printf("%d %d %s\n",a->id,a->idade,a->nome + TAM_SLT + TAM_SLT/2);
					free(a->nome);
					free(a);
					gettimeofday(&first, NULL);
					printf("Execuao leitura sequencial : %lu s | %lu us\n",final.tv_sec - first.tv_sec,final.tv_usec - first.tv_usec);

					return 1;
				}
				free(a->nome);
				free(a);
			}
			free(fck);
		}
	}
	gettimeofday(&first, NULL);
	printf("Registro não esta presente no banco de dados\n");
	printf("Execuao leitura sequencial : %lu s | %lu us\n",final.tv_sec - first.tv_sec,final.tv_usec - first.tv_usec);

	return 0;
}
