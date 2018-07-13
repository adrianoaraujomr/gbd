#include "piii.h"

char file1[100] = "../alunos.txt";
char file2[100] = "../disciplinas.txt";
char dir01[100] = "../h_dir1.txt";
char dir02[100] = "../h_dir2.txt";
char val01[100] = "../h_val1.txt";
char val02[100] = "../h_val2.txt";

void operacoes_heap(){
	char nomearquivo[100] = "../teste.txt";

//	cria_arquivo(nomearquivo);

// 1 - Inserir registro : e necessario criar o registro e ja ter o arquivo criado
	char nome[100] = "String";
	char *registroformatado;
	int id,numero;

	id = 1;
	numero = 4898;
/*
	while(id < 6){

		registroformatado = cria_registro(id,numero,nome);
		escreve_registro(registroformatado,nomearquivo);
		free(registroformatado);
	
		id++;
	}

// 2 - Remover registro sem alterar localizacoa de outros

	remove_registro(3,nomearquivo);
*/
// 3 - Atualizacao de um registro 

//	registroformatado = cria_registro(25,170,nome);
//	atualizar_registro(1,registroformatado,nomearquivo);

// 4 - Varredura sequencial 

	varredura_sequencial(nomearquivo);

}

void operacoes_indice(){
	char nomearquivo[100] = "../hteste.txt",diretorioarquivo[100] = "../hdir.txt",bucketarquivo[100] = "../hbuck.txt";
	Hash *hashtable;

// 0 - cria hash

	hashtable = le_hash(nomearquivo,diretorioarquivo);
	printh(hashtable,bucketarquivo);

// 1 - Inserir registro
	char nome[100] = "EvolSiDoolbEht";
	char *registroformatado;
	int id = 8,numero = 255;
	int i = 0;
/*
	for(i = 1;i < 4;i++){
		registroformatado = cria_registro(id,numero,nome);
		ht_insere(hashtable,i*8,registroformatado,nomearquivo,bucketarquivo);
		free(registroformatado);
	}
	grava_hash(hashtable,diretorioarquivo);	

	printh(hashtable,bucketarquivo);
*/
// 2 - Buscar registro

//	ht_busca(hashtable,id,nomearquivo,bucketarquivo);

// 3 - Remover registro

	ht_remove(hashtable,id,nomearquivo,bucketarquivo);
	printh(hashtable,bucketarquivo);

}

void juncoes(){

// juncoes tem como base o id

// 1 - Gerar arquivos grandes 
	char nomearquivo1[100] = "../teste1.txt",nomearquivo2[100] = "../teste2.txt";
	char nome[100] = "EybdoogWolsGnol";
	char *registroformatado;
	int id,numero1,numero2;	

	cria_arquivo(nomearquivo1);
	numero1 = 4898;

	for(id = 1;id <= 10000;id++) {

		registroformatado = cria_registro(id,numero1,nome);
		escreve_registro(registroformatado,nomearquivo1);
		free(registroformatado);

	}
/*
	cria_arquivo(nomearquivo2);
	numero2 = 8984;

	for(id = 1;id <= 10000;id++) {

		registroformatado = cria_registro(id,numero1,nome);
		escreve_registro(registroformatado,nomearquivo2);
		free(registroformatado);

	}

// 2 - Juncao por blocos
/*
	block_join(nomearquivo1,nomearquivo2);
*/
// 3 - Gerar arquivo grande com hash
	char nomearquivo[100] = "../hteste.txt",diretorioarquivo[100] = "../hdir.txt",bucketarquivo[100] = "../hbuck.txt";
	Hash *hashtable;
	int numero = 255;


	hashtable = cria_hash(nomearquivo,diretorioarquivo,bucketarquivo);
	printh(hashtable,bucketarquivo);


	for(id = 1;id <= 5000;id++) {

		registroformatado = cria_registro(id,numero,nome);
		ht_insere(hashtable,id,registroformatado,nomearquivo,bucketarquivo);
		free(registroformatado);

	}
	
	grava_hash(hashtable,diretorioarquivo);
	printh(hashtable,bucketarquivo);

//	hashtable = le_hash(nomearquivo,diretorioarquivo);

// 3 - Juncoa por indice

	index_join(nomearquivo1,nomearquivo,hashtable,bucketarquivo);

}

void sequencial_x_aleatorio(){
	char nomearquivo[100] = "../hteste.txt",diretorioarquivo[100] = "../hdir.txt",bucketarquivo[100] = "../hbuck.txt";
	Hash *hashtable;
	int id = 8550;

	hashtable = le_hash(nomearquivo,diretorioarquivo);
	leitura_sequencial(id,nomearquivo);
	ht_busca_2(hashtable,id,nomearquivo,bucketarquivo);
}

int main(int argc,char *argv[]){
//	operacoes_heap();
//	operacoes_indice();
//	juncoes();
	sequencial_x_aleatorio();
}
