// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
// Objetivos:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// ============================================================================

/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

/* Constantes Globais*/
#define N_TERRITORIOS 10
#define N_MISSOES 6
#define MAX_NOME 30

// cores de exército
enum { NEUTRO = 0, VERMELHO, AZUL, VERDE, AMARELO };

// estrutura de cada território
typedef struct {
    char nome [MAX_NOME];
    int owner; // 0 = neutro, 1 = vermelho, 2 = azul, 3 = verde, 4 = amarelo
    int tropas;
} Territorio;

// protóyipo de funções

// setup e gerenciamento de memoria
Territorio* allocatemap(size_t n);
void iniciaisTerritorios(Territorio* mapa, size_t n);
void freemap(Territorio* mapa);

// interface usuário
void limparBuffer(void);
void exibirMenu(void);
void exibirMapa(const Territorio* mapa, size_t n);
void exibirMissao(int missaoID, int corJogador);

//lógica principal
void faseAtaque(Territorio* mapa, size_t n, int corJogador);
void simulandoAtaque(Territorio* mapa, int idxFrom, int idxTo);
int verificavoiditoria(const Territorio* mapa, size_t n, int missaoId, int corJogador);
int escolhaMissao(void);

// utilitárias
const char* nomeCor(int cor);

int main(void) {
    setlocale(LC_ALL, "Português");
    srand ((unsigned)time(NULL));

    Territorio *mundo = allocatemap(N_TERRITORIOS);
    if (!mundo) {
        fprintf(stderr, "Erro: memória insuficiente.\n");
        return EXIT_FAILURE;
    }
    iniciaisTerritorios(mundo, N_TERRITORIOS);
    int corJogador = (rand() % 4) + 1;
    int missaoID = escolhaMissao();

    int opcao;
    do {
        exibirMapa(mundo, N_TERRITORIOS);
        exibirMissao(missaoID, corJogador);
        exibirMenu();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                faseAtaque(mundo, N_TERRITORIOS, corJogador);
                break;
            case 2:
                if (verificavoiditoria(mundo, N_TERRITORIOS, missaoID, corJogador)) {
                    printf("\n Parabéns! Você cumpriu sua missão!\n\n");
                    opcao = 0; // encerra
                } else {
                    printf("\nMissão ainda não concluida. Continue lutando!\n\n");
                }
                break;
            case 0:
                printf("\nSaindo do jogo...\n\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n\n");
        }
        if (opcao != 0) {
            printf("Pressione Enter para continuar...");
            getchar();
        }
    } while (opcao != 0);
    freemap(mundo);
    return EXIT_SUCCESS;
}

// Aloca dinamicamente um vetor de territórios
Territorio* allocatemap(size_t n) {
    return calloc(n, sizeof(Territorio));
}

// Preenche nomes, donos e tropas iniciais
void iniciaisTerritorios(Territorio* mapa, size_t n) {
    const char *defaultNames[N_TERRITORIOS] = {"Brasil", "Venezuela", "Peru", "Argentina", "Argélia", "Egito", "Sudão", "Congo", "África do Sul", "Madagascar",};
    for (size_t i = 0; i < n; i++) {
        strncpy(mapa[i].nome, defaultNames[i], MAX_NOME - 1);
        mapa[i].owner = (rand() % 4) + 1; 
        mapa[i].tropas = (rand() % 8) + 1;
    }
}

// Liberar o vetor de territórios

