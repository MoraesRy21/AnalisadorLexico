/**
 * Autor: Iarley Porto Cruz Moraes
 * Matéria: Teoria da Computação e Compiladores
 *
 * Analisador léxico
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

#define MAX 5000
#define TC ' '    //Termino da cadeia
#define PL '\n'  //Pular linha
#define TAMAUX 30
#define MAX_SIMBOLOS 38
#define MAX_ESTADOS 250

//=============================== Variáveis Globais =======================================
char estados[50][5];
char estadoInicial[5];
char estadoFinal[5];
char alfabetoFita[10];
int contTransacoes, contEstadosFinais, contEstados;

int cabeca_leitura;
int numCadeias;

char alfabeto[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 
'=', '<', '>',
'(', ')', '[', ']', '{', '}',  '"', 
'+', '-', '/', '%','*', 
',', ';',
':'};

char caracteres[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

char numeros[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

char fechamento[] = {'(', ')', '[', ']', '{', '}',  '"'};

char atribuicao[] = {':'};

char relacional[] = {'=', '<', '>'};

char aritimetico[] = {'+', '-', '/', '%', '*'};

char* palavras[] = {"for", "step", "sqrt", "if", "else", "return", "print", "read", "to", "num", "string"};

//=========================================================================================

//	Leitura do arquivo e retornando um valor inteiro para 
//	alocar dinâmicamente em um vetor parra não consumir muita memória; 
int lerArquivo(FILE *arq){
	//setlocale(LC_ALL, "portuguese-brazilian");
	char c;
	int i = 0, j = -1;
	while ((c = fgetc(arq)) != EOF){
        i++;
		//Sleep(5);
		printf("%c",c);
	}
	printf("%d", i);
    printf("\nArquivo lido com sucesso!\n");
    return i;
}

void montarFita(FILE *arq, char *fita){
	char c;
	int j = -1;
	
	while ((c = fgetc(arq)) != EOF){
		j++;
        fita[j] = c;
	}
	printf("\nCaracteres pasados para fita com sucesso!\n");
}

/*
 * @param char letra
 * @return boolean
 */
int verificarAlfabeto(char letra){
	int i;
	for (i = 0; i < 54; i++){
		if(letra == alfabeto[i]) return 1;
	}	
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarCaracteres(char letra){
	int i;
	for (i = 0; i < 26; i++){
		if(letra == caracteres[i]) return 1;
	}	
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarNumeros(char letra){
	int i;
	for (i = 0; i < 10; i++){
		if(letra == numeros[i]) return 1;
	}	
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarFechamento(char letra){
	int i;
	for (i = 0; i < 7; i++){
		if(letra == fechamento[i]) return 1;
	}	
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarAtribuicao(char letra){
	int i;
	if(letra == atribuicao[0]) return 1;
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarRelacional(char letra){
	int i;
	for (i = 0; i < 3; i++){
		if(letra == relacional[i]) return 1;
	}	
	return 0;
}

/*
 * @param char letra
 * @return boolean
 */
int procurarAritimetico(char letra){
	int i;
	for (i = 0; i < 5; i++){
		if(letra == aritimetico[i]) return 1;
	}	
	return 0;
}

/*
 * @param Vetor de caracteres
 * @return boolean
 */
int verificarPalavra(char *aux) {
	printf("\nEstou verificando palavras!");
	int i=0;
	if(aux == NULL) return 0;
	for(i = 0; i < 11; i++){
		printf("\n%d", i);
		printf("\t%s", palavras[i]);
		printf("\t%s", aux);
		//Sleep(500);
		if(strcmp(aux, palavras[i]) == 0) return 1;
	}
	return 0;
}

// Função para negação
int neg(int retorno){
	if(retorno > 0) 
		return 0;
	else
		return 1;
}

void reiniciarAuxiliar(char* aux){
	free(aux);
	aux = NULL;
}

void reiniciarVetAux(char *vetAux){
	int i;
	for(i=0; i<TAMAUX; i++){
		vetAux[i] = '\0';
	}
}

void analisarFita(char *fita, int tamanho){
    
	int contAux = 0, cabecote = 0, i;
	char vetAux[TAMAUX];// Variável auxiliar para fazer comparação com as palavras;
	char c;
	int chaves = 0;
	int cochetes = 0;
	int parenteses = 0;
	int erro = 0;
	FILE *resultado;
	reiniciarVetAux(&vetAux);
	
	while (cabecote < tamanho){
		c = fita[cabecote];
		//Caracteres
		if(procurarCaracteres(c)){
			if(vetAux[0] == '\0'){
				vetAux[contAux] = c;
				contAux++;
				cabecote++;
				continue;
			}else{
				vetAux[contAux] = c;
				contAux++;
				cabecote++;
				continue;
			}
		}
		//Numeros
		if(procurarNumeros(c)){
			if(vetAux[0] == '\0'){
				if(procurarCaracteres(fita[cabecote + 1])){
					vetAux[contAux] = c;
					contAux++;
					cabecote++;
					continue;
				}
			}else{
				vetAux[contAux] = c;
				contAux++;
				cabecote++;
				continue;
			}
		}
		//Fechamentos
		if(procurarFechamento(c)){
			if(c == '"'){
				cabecote++;
				c = fita[cabecote];
				while(cabecote<tamanho && c != '"'){
					c = fita[cabecote];
					++cabecote;
				}
			}
			if(vetAux[0] == '\0'){
				cabecote++;
			}else{
				if(verificarPalavra(&vetAux)){
					printf("\nPalavra valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
				}else{
					printf("\nPalavra nao valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
				} 
			}
			if(c == '{') chaves++;
			if(c == '}') chaves--;
			if(c == '[') cochetes++;
			if(c == ']') cochetes--;
			if(c == '(') parenteses++;
			if(c == ')') parenteses--;
			continue;
		}
		//Atribuição
		if(procurarAtribuicao(c)){
			if(vetAux[0] == '\0'){
				printf("\nEntrei aqui!");
				cabecote++;
				continue;
			}else{
				if(verificarPalavra(&vetAux)){
					printf("\nPalavra valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}else{
					printf("\nPalavra nao valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}
			}
		}
		//Relacional
		if(procurarRelacional(c)){
			if(vetAux[0] == '\0'){
				cabecote++;
			}else{
				if(verificarPalavra(&vetAux)){
					printf("\nPalavra valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}else{
					printf("\nPalavra nao valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}
			}
		}
		//Atritimético
		if(procurarAritimetico(c)){
			if(vetAux[0] == '\0'){
				cabecote++;
			}else{
				if(verificarPalavra(&vetAux)){
					printf("\nPalavra valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}else{
					printf("\nPalavra nao valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}
			}
		}
		
		if(c == ' ' || c == '\n'){
			if(vetAux[0] == '\0'){
				cabecote++;
				continue;
			}else{
				if(verificarPalavra(&vetAux)){
					printf("\nPalavra valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}else{
					printf("\nPalavra nao valida! - %s", vetAux);
					reiniciarVetAux(&vetAux);
					contAux = 0;
					cabecote++;
					continue;
				}
			}
		}
		
		if(verificarAlfabeto(c) && c != ' ') {
			cabecote++;
			continue;
		}else{
			printf("\nCaracter '%c' invalido, nao pertence ao alfabeto!", c);
			cabecote++;
			erro++;
			continue;
		}
	}
	
	printf("\n%d", cabecote);
	if(chaves == 0){
		printf("\nFechamento das chaves OK!");
	}else{
		printf("\nFechamento de chaves faltando!");
	}
	if(cochetes == 0){
		printf("\nFechamento das cochetes OK!");
	}else{
		printf("\nFechamento de cochetes faltando!");
	}
	if(parenteses == 0){
		printf("\nFechamento das parênteses OK!");
	}else{
		printf("\nFechamento de parênteses faltando!");
	}
	
	resultado = fopen("Result.txt", "w");
	printf("\nAnálise do aqruivo terminada!\n");
	if(chaves == 0){
		fprintf(resultado, "\nFechamento das chaves OK!");
	}else{
		fprintf(resultado, "\nFechamento de chaves faltando!");
	}
	if(cochetes == 0){
		fprintf(resultado, "\nFechamento das cochetes OK!");
	}else{
		printf(resultado, "\nFechamento de cochetes faltando!");
	}
	if(parenteses == 0){
		fprintf(resultado, "\nFechamento das parênteses OK!");
	}else{
		fprintf(resultado, "\nFechamento de parênteses faltando!");
	}
	fprintf(resultado, "\nQuntidade de caracteres invalidos: %d", erro);
	fclose(resultado);
	
	
}

int main() {
    
	FILE *arq;
    int tamanho, i;
	char *fita;
	//TODO Altere aqui o nome do arquivo
	char nomeArquivo[] = "ArquivoFonte1.mln";
	setlocale(LC_ALL, "portuguese-brazilian");
	
	printf("|===============================================|\n");
    printf("|============== ANALISADOR LEXICO ==============|\n");
    printf("|===============================================|\n");
    printf("|== Digite o nome do arquivo a ser analisado: ==|\n\n");
	
	//scanf("%s", &nomeArquivo); //Leitura do nome do arquivo a ser testado
	arq = fopen(nomeArquivo, "r");
	if (arq == NULL){
		printf("Arquivo vazio!");
		return -1;
	}
	tamanho = lerArquivo(arq);
	fclose(arq);
	
	fita = (char*) malloc(i * sizeof(char));
	
	printf("\n\n\n");
	arq = fopen(nomeArquivo, "r");
	montarFita(arq, fita);
	fclose(arq);
	
	printf("\n\n\n");
	for(i=0; i<tamanho; i++)
		printf("%c", fita[i]);
	printf("\nTeste");
	
	analisarFita(fita, tamanho); 
	return 0;
}


