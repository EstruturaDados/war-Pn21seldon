#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 5
#define MAX_NOME        30
#define MAX_COR         20

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

// contador global de terras conquistadas
static int conquistas = 0;

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void inicializaTerritorios(Territorio* t, int n) {
    for (int i = 0; i < n; i++) {
        printf("Cadastro do Territorio %d\n", i + 1);

        printf("  Nome: ");
        fgets(t[i].nome, MAX_NOME, stdin);
        t[i].nome[strcspn(t[i].nome, "\n")] = '\0';

        printf("  Cor do exército: ");
        fgets(t[i].cor, MAX_COR, stdin);
        t[i].cor[strcspn(t[i].cor, "\n")] = '\0';

        printf("  Tropas iniciais: ");
        scanf("%d", &t[i].tropas);
        limparBuffer();

        puts("");
    }
}

void exibirMapa(const Territorio* t, int n) {
    puts("Estado atual do Mapa:");
    for (int i = 0; i < n; i++) {
        printf(" [%d] %-25s | Cor: %-10s | Tropas: %2d\n",
               i + 1, t[i].nome, t[i].cor, t[i].tropas);
    }
    puts("");
}

void atacar(Territorio* t, int n) {
    int idAtt, idDef;
    printf("Escolha o atacante (1-%d): ", n);
    scanf("%d", &idAtt);
    limparBuffer();

    printf("Escolha o defensor (1-%d): ", n);
    scanf("%d", &idDef);
    limparBuffer();

    if (idAtt < 1 || idAtt > n || idDef < 1 || idDef > n) {
        puts("Selecao invalida. Tente novamente.\n");
        return;
    }
    if (idAtt == idDef) {
        puts("Atacante e defensor devem ser territorios diferentes.\n");
        return;
    }
    if (t[idAtt-1].tropas < 1) {
        puts("Territorio atacante sem tropas suficientes.\n");
        return;
    }
    if (t[idDef-1].tropas < 1) {
        puts("Territorio defensor ja esta sem tropas.\n\n");
        return;
    }

    int dadoAtt = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;
    printf("  Dado Atacante: %d  |  Dado Defensor: %d\n", dadoAtt, dadoDef);

    if (dadoAtt >= dadoDef) {
        t[idDef-1].tropas--;
        if (t[idDef-1].tropas <= 0) {
            printf("  Territorio %s conquistado!\n", t[idDef-1].nome);
            // conquista: cor e tropas resetadas
            strcpy(t[idDef-1].cor, t[idAtt-1].cor);
            t[idDef-1].tropas = 1;
            conquistas++;
        } else {
            puts("  Defensor perdeu 1 tropa.");
        }
    } else {
        t[idAtt-1].tropas--;
        puts("  Atacante perdeu 1 tropa.");
    }
    putchar('\n');
}

int verificarMissao(const Territorio* t, int n, int missaoTipo) {
    if (missaoTipo == 0) {
        // missão: destruir exército Verde
        for (int i = 0; i < n; i++) {
            if (strcmp(t[i].cor, "Verde") == 0) {
                return 0;
            }
        }
        return 1;
    } else {
        // missão: conquistar 3 territórios
        return (conquistas >= 3);
    }
}

int main(void) {
    srand((unsigned)time(NULL));

    Territorio* mapa = calloc(MAX_TERRITORIOS, sizeof *mapa);
    if (!mapa) {
        perror("Falha na alocacao");
        return EXIT_FAILURE;
    }

    puts("=== Desafio WAR Estruturado ===\n");
    inicializaTerritorios(mapa, MAX_TERRITORIOS);

    int missaoTipo = rand() % 2; 
    if (missaoTipo == 0) {
        puts("Missao: Destruir o exército Verde.\n");
    } else {
        puts("Missao: Conquistar 3 territorios.\n");
    }

    exibirMapa(mapa, MAX_TERRITORIOS);

    int opc;
    do {
        puts("Menu:");
        puts(" 1 - Atacar");
        puts(" 2 - Verificar Missao");
        puts(" 0 - Sair");
        printf("Escolha: ");
        scanf("%d", &opc);
        limparBuffer();
        putchar('\n');

        switch (opc) {
            case 1:
                atacar(mapa, MAX_TERRITORIOS);
                exibirMapa(mapa, MAX_TERRITORIOS);
                break;
            case 2:
                if (verificarMissao(mapa, MAX_TERRITORIOS, missaoTipo)) {
                    puts("Parabens! Missao cumprida.\n");
                    opc = 0;  // encerra o jogo
                } else {
                    puts("Missao ainda nao esta completa.\n");
                }
                break;
            case 0:
                puts("Saindo do jogo. Ate a proxima!\n");
                break;
            default:
                puts("Opcao invalida. Tente novamente.\n");
        }
    } while (opc != 0);

    free(mapa);
    return EXIT_SUCCESS;
}
