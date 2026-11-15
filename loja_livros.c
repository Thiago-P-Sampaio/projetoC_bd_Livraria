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

// Prototipos de funcoes

void toLower(char string[], int tamanho);
int tamanhoString(char string[], int bytes);

int quantidadeRegistros(FILE *arquivo, char nome[]);
void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]);
int evitarDuplicata(FILE *arquivo, char nome[] ,Livro livros_estoque[], char titulo[]);
void buscarRegistro(char nome[], char termo[]);
void deletarRegistro(char nome[], char termo[]);
void editarRegistro(char nome[], char termo[]);

//

int main()
{

    Livro livros_estoque[MAX]; // Vetor que guarda instancias da minha estrutura Livro, permite ter varios livros
    
    FILE *bd_livraria;
    char termo[50];
    int opcao = -1;
    
    while (opcao != 0) {
        printf("\n1. Inserir\n2. Buscar\n3. Editar\n4. Deletar\n0. Sair\n");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: 
                inserirRegistro(bd_livraria, "tb_livros.txt", livros_estoque);
                break;
            case 2:
                printf("\nTitulo ou Categoria a buscar:\n");
                scanf(" %49[^\n]", termo);
                buscarRegistro("tb_livros.txt", termo);
                break;
            case 3: 
                printf("\nTitulo a Editar:\n");
                scanf(" %49[^\n]", termo);
                editarRegistro("tb_livros.txt", termo);
                break;
            case 4:
                printf("\nTitulo a deletar:\n");
                scanf(" %49[^\n]", termo);
                deletarRegistro("tb_livros.txt", termo);
                break;
            case 0:
                printf("\nEncerrando...\n");
                break;
            default:
                printf("\nOpção Invalida\n");
        }
    }
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
    
    char titulo[50], autor[50], categoria[50], editora[50];
    int id, lancamento;
    float preco;
    
    arquivo = fopen(nome, "r");
    
    if(arquivo != NULL){ 
    	// Garantindo que o  fscanf capture 7 campos, formacao de uma linha/registro
        while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f;%d", // utilizando RegEX para sequencia de caracter e eleminar ";"
        titulo, autor, categoria, editora, &lancamento, &preco, &id) == 7
        ) quant++;
    fclose(arquivo);
    return quant;
    }
    else {
        printf("\nImpossivel ler o arquivo\n");
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
                printf("\nJa existe um livro com esse titulo\n"); // apesar que podem existir livros com nomes iguais  ;-;
                fclose(arquivo);
                return 1; // Return 1 TRUE -- sera capturado pelo IF
            }
        }
        fclose(arquivo);
        return 0;
        
    } else {
        printf("\nImpossivel ler o arquivo\n");
        fclose(arquivo);
        return 0;    
    }
}

void inserirRegistro(FILE *arquivo, char nome[], Livro livros_estoque[]){
	    int indx = 0; // Definindo 0 para o indice que ira controlar as posicoes do meu vetor
    // if(arquivo == NULL){ // Caso o arquivo nao exista: crie
    //     arquivo = fopen(nome, "w");
    // }
    // else { // Caso o arquivo exista: abra em modo "append" - evitando sobrescrever
    //     arquivo = fopen(nome, "a"); 
    // }
        indx = quantidadeRegistros(arquivo, nome); // Retornando para o indice a quantidade de elementos, ao qual sera o indice da proxima insercaoo!
        
        printf("\nIndice %d\n", indx); // print de debug
        
        // While "TRUE"
        while(1){
        	arquivo = fopen(nome, "a"); // Se o arquivo existir abra, se nao crie -- Nao ira sobrescrever!
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
            if(scanf("%d", &livros_estoque[indx].lancamento) == 1 && !(livros_estoque[indx].lancamento > 0 && livros_estoque[indx].lancamento < 2027) ){
                printf("\nData de Lancamento Invalida...\n");
                fclose(arquivo);
                continue;
            } 
            
            printf("\nInforme o preco do livro:\n");
            if(scanf("%f", &livros_estoque[indx].preco) == 1 && !(livros_estoque[indx].preco > 0) ){
                printf("\nPreco Invalido...\n");
                fclose(arquivo);
                continue;
            }
            
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
            fclose(arquivo);
            
            
            // Loop de escolha --> Continuar cadastrando livros ou nao
            int escolha = 1;
            printf("\nDeseja inserir um novo livro? 1. Sim  | 0. Não\n");
            scanf("%d", &escolha);
            if(escolha == 0) {
                break; // Forcando parada do Loop
            }
            
            indx++; // Caso prossiga com cadastros consecutivos, incrementos no meu indice
        }
}

// Função de buscar items dentro do registro
void buscarRegistro(char nome[], char termo[]){
    FILE *arquivo = fopen(nome, "r");
    if(arquivo == NULL){
        printf("\nImpossivel ler o arquivo\n");
        return;
    }

    char titulo[50], autor[50], categoria[50], editora[50];
    char copia[50]; 
    char copiagato[50];
    int id, lancamento;
    float preco;
    int encontrou = 0;

    toLower(termo, tamanhoString(termo, 50));

    while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f;%d",
          titulo, autor, categoria, editora, &lancamento, &preco, &id) == 7){
              
        strcpy(copia, titulo); // Busca por titulo
        strcpy(copiagato, categoria); // Busca por categoria
        toLower(copia, tamanhoString(copia, 50)); // Diminui o titulo para minusculo
        toLower(copiagato, tamanhoString(copiagato, 50)); // Diminui a categoria para minusculo
        
        if(strstr(copia, termo) != NULL || strstr(copiagato, termo) != NULL) { // Buscando no registro, caso algo bata ele mostrara no terminal, caso contrario mostrara nada
            printf("\nID: %d\nTitulo: %s\nAutor: %s\nCategoria: %s\nEditora: %s\nAno: %d\nPreco: %.2f\n",
                   id, titulo, autor, categoria, editora, lancamento, preco);
            encontrou = 1;
        }
    }

    fclose(arquivo);
    if(!encontrou) {
        printf("\nNenhum registro encontrado\n");
    }
}

// deletando os registros
void deletarRegistro(char nome[], char termo[]){
    int certeza = 1;
    printf("\nTem certeza que quer apagar um livro do registro? 1. Sim | 0. Não\n");
    scanf("%d", &certeza);
        if (certeza != 1)
        {
            printf("\nOperação cancelada\n");
            return;
        }
    
    FILE *arquivo = fopen(nome, "r"); // Abrindo o arquivo original em modo de leitura
    if(arquivo == NULL){ // Caso o arquivo original não exista, da erro
        printf("\nImpossivel ler o arquivo\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w"); // Criando um arquivo temporario para garantir backup
    if(temp == NULL){
        fclose(arquivo); // Caso o arquivo temporario não abra, fecha o arquivo original
        return;
    }

    char titulo[50], autor[50], categoria[50], editora[50], copia[50];
    int id, lancamento, encontrou = 0;

    float preco;

    toLower(termo, tamanhoString(termo, 50));

    while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f;%d",
          titulo, autor, categoria, editora, &lancamento, &preco, &id) == 7){

        strcpy(copia, titulo);  
        toLower(copia, tamanhoString(copia, 50));

        if(strcmp(copia, termo) == 0){ // Buscando no registro o titulo exato do livro para deletar
            encontrou = 1;               // ele encontrou o livro no registro
        } else {
            fprintf(temp, "%s;%s;%s;%s;%d;%.2f;%d\n", // Alterando o registro temporario para remover o livro deletado
            titulo, autor, categoria, editora, lancamento, preco, id);
        }
    }

    fclose(arquivo);
    fclose(temp);                        // fechando ambos o arquivo e o temporario

    if(encontrou){
        remove(nome);                    // deletando o arquivo anterior
        rename("temp.txt", nome);        // substituindo o arquivo anterior pelo novo temp
        printf("\nRegistro deletado\n");
    } else {
        remove("temp.txt");              // nada foi alterado ent apaga o temp
        printf("\nNenhum registro encontrado\n");
    }
}


void editarRegistro(char nome[], char termo[]){
    FILE *arquivo = fopen(nome, "r");
    if(arquivo == NULL){
        printf("\nImpossivel ler o arquivo\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    if(temp == NULL){
        fclose(arquivo);
        return;
    }

    char titulo[50], autor[50], categoria[50], editora[50], copia[50];
    int id, lancamento, encontrou = 0;
    float preco;

    toLower(termo, tamanhoString(termo, 50));
    
    while(fscanf(arquivo, " %49[^;];%49[^;];%49[^;];%49[^;];%d;%f;%d",
          titulo, autor, categoria, editora, &lancamento, &preco, &id) == 7){

        strcpy(copia, titulo);
        toLower(copia, tamanhoString(copia, 50));

        if(strcmp(copia, termo) == 0){ // Procurando no registro exatamante o livro correto
            encontrou = 1;
            // pegando as informações novas do livro
            printf("\nNovo titulo:\n");    scanf(" %49[^\n]", titulo);
            printf("\nNovo autor:\n");     scanf(" %49[^\n]", autor);
            printf("\nNova categoria:\n"); scanf(" %49[^\n]", categoria);
            printf("\nNova editora:\n");   scanf(" %49[^\n]", editora);
            printf("\nNovo ano:\n");       scanf("%d", &lancamento);
            printf("\nNovo preco:\n");     scanf("%f", &preco);
            // id não foi tocado ent continua o mesmo
        }

        fprintf(temp, "%s;%s;%s;%s;%d;%.2f;%d\n", // alterando no registro temporario as informações do livro em si
                titulo, autor, categoria, editora, lancamento, preco, id);
    }

    fclose(arquivo);
    fclose(temp);                        

    if(encontrou){
        remove(nome);                    
        rename("temp.txt", nome);       
        printf("\nRegistro substituido\n"); // substituindo o registro original pelo temporario para garantir a mudança das informações
    } else {
        remove("temp.txt");             
        printf("\nNenhum registro encontrado\n");
    }
}


