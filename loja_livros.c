/// ALGORITMO DE PARA REGISTRO DE LIVROS DE LIVRARIA
// IMPORT DE BIBLIOTECAS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100

    typedef struct {  
        // int id;
        char titulo[50]; 
        char autor[50];
        char categoria[50];
        char editora[50];
        int  lancamento;
        float  preco;
    } Livro; //pseudo nome para minha struct

void toLower(char string[], int tamanho);
int tamanhoString(char string[], int bytes);

int quantidadeRegistros(FILE *arquivo, char nome[]);
void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]);
int evitarDuplicata(FILE *arquivo, char nome[] ,Livro livros_estoque[], char titulo[]);



int main()
{
    Livro livros_estoque[MAX]; // Vetor que guarda instâncias da minha estrutura Livro, permite ter vários livros
    
    FILE *bd_livraria;
    
    inserirRegistro(bd_livraria, "tb_livros.txt", livros_estoque);
    return 0;
}

void toLower(char string[], int tamanho){
    for(int i = 0; i < tamanho; i++ ){
  		if( (int) string[i] == 0 ) break; //QUEBRAR AO IDENTIFICAR FIM DE STRING \0
        if(!((int) string[i] >= 97 && (int) string[i] <= 122))
        	if((int) string[i] >= 65 && (int) string[i] <= 90)
        		string[i] = (int) string[i] + 32; //UTILIZANDO DA TABELA ASCII | EXEMPLO A(=65) -> a(=97)
    }
}

int tamanhoString(char string[], int bytes){
	int indx = 0;
	while((int)string[indx] != 0){
		indx++;
	}
	return indx;
}

int quantidadeRegistros(FILE *arquivo, char nome[]){
    int quant = 0;
    
    char titulo[50], autor[50], categoria[50],editora[50];
    int lancamento;
    float preco;
    
    arquivo = fopen(nome, "r");
    
    if(arquivo != NULL){ 
        while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f",
        titulo, autor, categoria, editora, &lancamento, &preco) == 6
        ) quant++;
    fclose(arquivo);
    return quant;
    }
    else {
        printf("\nImpossível ler o arquivo\n");
        fclose(arquivo);
        return 0;
    }
} 

int evitarDuplicata(FILE *arquivo, char nome[] ,Livro livros_estoque[], char titulo[]){
    arquivo = fopen(nome, "r");
    char titulo_arquivo[50];
    if(arquivo != NULL){
        toLower(titulo, tamanhoString(titulo,50));       
        while (fscanf(arquivo, " %49[^;]%*[^\n]", titulo_arquivo) == 1){
            toLower(titulo_arquivo, tamanhoString(titulo_arquivo,50)); 
            if(strcmp(titulo,titulo_arquivo) == 0){
                printf("\nJá existe um livro com esse titulo\n"); // apesar que podem existir livros com nomes iguais  ;-;
                fclose(arquivo);
                return 1;
            }
        }
        fclose(arquivo);
        return 0;
        
    } else {
        printf("\nImpossível ler o arquivo\n");
        fclose(arquivo);
        return 0;    
    }
}

void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]){
    if(arquivo == NULL){
        arquivo = fopen(nome, "w");
    }
    else {
        arquivo = fopen(nome, "a");
    }
        int indx = quantidadeRegistros(arquivo, nome);
        
        printf("\nIndice %d\n", indx);
        
        while(1){
            printf("\nInforme o nome do livro:\n");
            scanf("%s", &livros_estoque[indx].titulo);
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
            

            
            fprintf(arquivo, "%s;%s;%s;%s;%d;%.2f\n",
            livros_estoque[indx].titulo,
            livros_estoque[indx].autor,
            livros_estoque[indx].categoria,
            livros_estoque[indx].editora,
            livros_estoque[indx].lancamento,
            livros_estoque[indx].preco
            );
            
            int escolha = 0;
            printf("\nDeseja encerrar? 1. Sim  | 0. Não\n");
            scanf("%d", &escolha);
            if(escolha == 1) {
                fclose(arquivo);
                break;
            }
            
            indx++;
        }
}


