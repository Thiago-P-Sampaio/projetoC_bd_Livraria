/// ALGORITMO DE PARA REGISTRO DE LIVROS DE LIVRARIA
// IMPORT DE BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100 // constante, escopo global, delimita nums de instancias do meu vetor

    typedef struct { // struct global  
        int id;
        char titulo[50]; 
        char autor[50];
        char categoria[50];
        char editora[50];
        int  lancamento;
        float  preco;
    } Livro; //pseudo nome para minha struct

// Prot�tipos de funcoes

void toLower(char string[], int tamanho);
int tamanhoString(char string[], int bytes);

int quantidadeRegistros(FILE *arquivo, char nome[]);
void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]);
int evitarDuplicata(FILE *arquivo, char nome[] ,Livro livros_estoque[], char titulo[]);

//


int main()
{
    Livro livros_estoque[MAX]; // Vetor que guarda instancias da minha estrutura Livro, permite ter varios livros
    
    FILE *bd_livraria;
    
    inserirRegistro(bd_livraria, "tb_livros.txt", livros_estoque);
    return 0;
}

// Transformando caracter por caracter em minusculo, utilizando codigo da tabela ASCII
void toLower(char string[], int tamanho){
    for(int i = 0; i < tamanho; i++ ){
    	// CASTING (acho que nao obrigatorio)
  		if( (int) string[i] == 0 ) break; //QUEBRAR AO IDENTIFICAR FIM DE STRING \0
        if(!((int) string[i] >= 97 && (int) string[i] <= 122)) // Desconsidera alfabeto ja em minusculo
        	if((int) string[i] >= 65 && (int) string[i] <= 90) // Considera, unicamente, letras A-Z.
        		string[i] = (int) string[i] + 32; //UTILIZANDO DA TABELA ASCII | EXEMPLO A(=65) -> a(=97)
    }
}

// Funcao que retorna tamanho util(ocupado por texto) de uma STRING
int tamanhoString(char string[], int bytes){
	int indx = 0;
	while((int)string[indx] != 0){ // Para quando vier caracter NULL, fim da string
		indx++;
	}
	return indx;
}

// Retorna quantidade de registros(tuplas) do arquivo
int quantidadeRegistros(FILE *arquivo, char nome[]){
    int quant = 0;
    
    char titulo[50], autor[50], categoria[50],editora[50];
    int id,lancamento;
    float preco;
    
    arquivo = fopen(nome, "r");
    
    if(arquivo != NULL){ 
    
    	// Garantindo que o  fscanf capture 7 campos, forma��o de uma linha/registro
        while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f;%d", // utilizando RegEX para sequencia de caracter e eleminar ";"
        titulo, autor, categoria, editora, &lancamento, &preco, &id) == 7
        ) quant++;
    fclose(arquivo);
    return quant;
    }
    else {
        printf("\nImposs�vel ler o arquivo\n");
        fclose(arquivo);
        return 0;
    }
} 
// Comparando valor passado pelo usuario, com valor para titulo dentro do arquivo
int evitarDuplicata(FILE *arquivo, char nome[] ,Livro livros_estoque[], char titulo[]){
    arquivo = fopen(nome, "r"); // Modo leitura
    char titulo_arquivo[50];
    if(arquivo != NULL){
        toLower(titulo, tamanhoString(titulo,50)); // transformando texto em minusculo      
        while (fscanf(arquivo, " %49[^;]%*[^\n]", titulo_arquivo) == 1){ // Capturando, exclusivamente, um campo da linha, utilizando RegEx
            toLower(titulo_arquivo, tamanhoString(titulo_arquivo,50)); // transformando texto em minusculo  
            if(strcmp(titulo,titulo_arquivo) == 0){ // Comparacao de strings
                printf("\nJ� existe um livro com esse titulo\n"); // apesar que podem existir livros com nomes iguais  ;-;
                fclose(arquivo);
                return 1; // Return 1 TRUE -- sera capturado pelo IF
            }
        }
        fclose(arquivo);
        return 0;
        
    } else {
        printf("\nImposs�vel ler o arquivo\n");
        fclose(arquivo);
        return 0;    
    }
}

void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]){
	int indx = 0; // Definindo 0 para o indice que ira controlar as posicoes do meu vetor
    if(arquivo == NULL){ // Caso o arquivo nao exista: crie
        arquivo = fopen(nome, "w");
    }
    else { // Caso o arquivo exista: abra em modo "append" - evitando sobrescrever
    	indx = quantidadeRegistros(arquivo, nome); // Retornando para o indice a quantidade de elementos, ao qual sera o indice da proxima insercaoo!
        arquivo = fopen(nome, "a"); 
    }
        
        printf("\nIndice %d\n", indx); // print de debug
        
        // While "TRUE"
        while(1){
        	
        	// Capturando dados via teclado do usuario e setando no meu vetor da struct Livro
        	
            printf("\nInforme o nome do livro:\n");
            scanf("%s", &livros_estoque[indx].titulo);
            // Chamada de funcao que verifica titulo duplicado, evitando cadastro do mesmo item
            if(evitarDuplicata(arquivo, nome, livros_estoque,livros_estoque[indx].titulo)) continue;
            
            printf("\nInforme o autor(a) do livro:\n");
            scanf("%s", &livros_estoque[indx].autor);
            
            
            printf("\nInforme a categoria do livro:\n");
            scanf("%s", &livros_estoque[indx].categoria);
            
            
            printf("\nInforme a editora do livro:\n");
            scanf("%s", &livros_estoque[indx].editora);
            
            
            printf("\nInforme o ano de lancamento do livro:\n");
            scanf("%d", &livros_estoque[indx].lancamento);
            
            
            printf("\nInforme o preco do livro:\n");
            scanf("%f", &livros_estoque[indx].preco);
            
            // Geracao de Ids automaticos, reaproveitando funcao
			if(indx == 0) livros_estoque[indx].id = 1;
			else livros_estoque[indx].id = indx+1;
            
            // "imprimindo" no arquivo os dados, separados por ";" para formatacao em ".CSV"
            fprintf(arquivo, "%s;%s;%s;%s;%d;%.2f;%d\n",
            livros_estoque[indx].titulo,
            livros_estoque[indx].autor,
            livros_estoque[indx].categoria,
            livros_estoque[indx].editora,
            livros_estoque[indx].lancamento,
            livros_estoque[indx].preco,
            livros_estoque[indx].id
            );
            
            // Loop de escolha --> Continuar cadastrando livros ou nao
            int escolha = 0;
            printf("\nDeseja encerrar? 1. Sim  | 0. N�o\n");
            scanf("%d", &escolha);
            if(escolha == 1) {
                fclose(arquivo);
                break; // Forcando parada do Loop
            }
            
            indx++; // Caso prossiga com cadastros consecutivos, incrementos no meu indice
        }
}


