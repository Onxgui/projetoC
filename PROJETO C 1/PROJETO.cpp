#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define ARQUIVO "jogos.bin"


// ESTRUTURA DO REGISTRO
typedef struct {
    char titulo[50];
    char genero[30];
    int ano;
} Jogo;



// PROTÓTIPOS

void limpaBuffer();
void ler_string(char *str, int tamanho);
int tamanho(FILE *fp);
void cadastrar(FILE *fp);
void consultar(FILE *fp);



// Função para limpar o buffer

void limpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}



// Leitura segura de string com fgets + remoção do '\n'

void ler_string(char *str, int tamanho) {
    fgets(str, tamanho, stdin);
    str[strcspn(str, "\n")] = '\0';
}



// Calcula quantos registros existem no arquivo binário

int tamanho(FILE *fp) {
    fseek(fp, 0, SEEK_END);
    long bytes = ftell(fp);
    return bytes / sizeof(Jogo);
}



// Cadastra novo jogo no final do arquivo

void cadastrar(FILE *fp) {
    Jogo game;

    printf("\n=== CADASTRAR JOGO ===\n");

    printf("Título do jogo: ");
    ler_string(game.titulo, sizeof(game.titulo));

    printf("Gênero: ");
    ler_string(game.genero, sizeof(game.genero));

    printf("Ano de lançamento: ");
    scanf("%d", &game.ano);
    limpaBuffer();

    fseek(fp, 0, SEEK_END);
    fwrite(&game, sizeof(Jogo), 1, fp);

    printf("Jogo cadastrado com sucesso!\n");
}


// Consulta jogo por índice (posição no arquivo)

void consultar(FILE *fp) {
    int indice;
    int total = tamanho(fp);

    printf("\nDigite o índice do jogo (0 a %d): ", total - 1);
    scanf("%d", &indice);
    limpaBuffer();

    if (indice < 0 || indice >= total) {
        printf("Índice inválido!\n");
        return;
    }

    Jogo game;

    fseek(fp, indice * sizeof(Jogo), SEEK_SET);
    fread(&game, sizeof(Jogo), 1, fp);

    printf("\n=== DADOS DO JOGO ===\n");
    printf("Título: %s\n", game.titulo);
    printf("Gênero: %s\n", game.genero);
    printf("Ano: %d\n", game.ano);
}


// Função principal com menu

int main() {

    setlocale(LC_ALL, "");

    FILE *fp = fopen(ARQUIVO, "r+b");

    if (fp == NULL) {
        fp = fopen(ARQUIVO, "w+b");
        if (fp == NULL) {
            printf("Erro ao abrir/criar o arquivo!\n");
            return 1;
        }
    }

    int op;

    do {
        printf("\n=========== CATÁLOGO DE JOGOS ===========\n");
        printf("1 - Cadastrar jogo\n");
        printf("2 - Consultar jogo por índice\n");
        printf("3 - Ver quantidade de cadastros\n");
        printf("0 - Sair\n");
        printf("Opção: ");
        scanf("%d", &op);
        limpaBuffer();

        switch (op) {
            case 1:
                cadastrar(fp);
                break;
            case 2:
                consultar(fp);
                break;
            case 3:
                printf("Total de jogos cadastrados: %d\n", tamanho(fp));
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }

    } while (op != 0);

    fclose(fp);
    return 0;
}
