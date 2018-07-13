#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <limits.h>
#include <sys/time.h>

#define TAM_SLT 8      //tamanho de um campo do slot em uma pagina   
#define TAM_PGN 1024 //tamanho da página

typedef struct {
	int skey;	    // new
	int page;           // nro da pagina do arquivo que o registro esta armazenado
	int slot;           // nro do slot da pagina em que o registro esta armazenado
} Rid;

typedef struct { // struct de um slot
	int size; // tamanho do registro
	int move; // ponteiro para o registro
} Reg;

typedef struct { // struct do registro
	int id;
	int idade;
	char *nome;
} Aluno;

char* int_to_hex(int val);
int hex_to_int(char* hex);
char* uint_to_hex(int val);
int hex_to_uint(char* hex);
char* cria_registro(int id,int age,char* name);

void cria_arquivo(char *file); //
int aloca_pagina(char *file);
int le_pagina(int nro,char *file);
int escreve_pagina(int nro,char *file);
Reg* le_slot(int nro);
void grava_slot(int nro,int slt1,int slt2);
Aluno* le_registro(int move,int size);
void set_max_size(char *file);

Rid* escreve_registro(char* registro,char *file);
int varredura_sequencial(char *file);
int remove_registro(int id,char *file);
Rid* atualizar_registro(int old,char *new,char *file);

Aluno* acesso_direto(Rid *rid,char *file);
void remove_direto(Rid *rid,char *file);

int leitura_sequencial(int id,char *file);

char bp_page[TAM_PGN]; //abstracao de uma pagina no buffer pool
int size_table;        //numero de paginas no arquivo
