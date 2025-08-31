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

typedef enum {
    PHASE_SETUP,
    PHASE_MISSION,
    PHASE_MENU,
    PHASE_ATTACK,
    PHASE_VERIFY,
    PHASE_EXIT
} Phase;

static int conquistas = 0;
static int missaoTipo = 0; // 0 = destruir Verde, 1 = conquistar 3

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int rolarDado(const char *label) {
    int resultado;
    printf("%s", label);
    fflush(stdout);
    // "Animação" simples de rolagem
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        for (volatile int d = 0; d < 20000000; d++);  // atraso aproximado
    }
    resultado = rand() % 6 + 1;
    printf(" %d\n", resultado);
    return resultado;
}

void inicializaTerritorios(Territorio *t, int n) {
    puts("=== Cadastro dos Territórios ===\n");
    for (int i = 0; i < n; i++) {
        printf("Território %d\n", i + 1);
        printf("  Nome: ");
        fgets(t[i].nome, MAX_NOME, stdin);
        t[i].nome[strcspn(t[i].nome, "\n")] = '\0';

        printf("  Cor do exército: ");
        fgets(t[i].cor, MAX_COR, stdin);
        t[i].cor[strcspn(t[i].cor, "\n")] = '\0';

        printf("  Tropas iniciais: ");
        scanf("%d", &t[i].tropas);
        limparBuffer();
        putchar('\n');
    }
}

void exibirMapa(const Territorio *t, int n) {
    puts("=== Estado Atual do Mapa ===");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-20s | Cor: %-8s | Tropas: %2d\n",
               i + 1, t[i].nome, t[i].cor, t[i].tropas);
    }
    putchar('\n');
}

void atacar(Territorio *t, int n) {
    int atk, def;
    printf("Escolha o ATACANTE (1-%d): ", n);
    scanf("%d", &atk); limparBuffer();
    printf("Escolha o DEFENSOR  (1-%d): ", n);
    scanf("%d", &def); limparBuffer();
    putchar('\n');

    if (atk < 1 || atk > n || def < 1 || def > n || atk == def) {
        puts("Seleção inválida. Pressione ENTER e tente de novo.\n");
        getchar();
        return;
    }
    if (t[atk-1].tropas < 1) {
        puts("Território atacante sem tropas suficientes.\n");
        return;
    }
    if (t[def-1].tropas < 1) {
        puts("Território defensor já está sem tropas.\n");
        return;
    }

    // rolagem de dados
    int dadoAtk = rolarDado("Atacante rola o dado");
    int dadoDef = rolarDado("Defensor rola o dado");

    // comparação
    if (dadoAtk >= dadoDef) {
        t[def-1].tropas--;
        if (t[def-1].tropas == 0) {
            printf("Território %s conquistado!\n\n", t[def-1].nome);
            strcpy(t[def-1].cor, t[atk-1].cor);
            t[def-1].tropas = 1;
            conquistas++;
        } else {
            puts("Defensor perde 1 tropa.\n");
        }
    } else {
        t[atk-1].tropas--;
        puts("Atacante perde 1 tropa.\n");
    }
}

int verificarMissao(const Territorio *t, int n) {
    if (missaoTipo == 0) {
        // destruir exército Verde
        for (int i = 0; i < n; i++)
            if (strcmp(t[i].cor, "Verde") == 0)
                return 0;
        return 1;
    } else {
        // conquistar 3 territórios
        return conquistas >= 3;
    }
}

int main(void) {
    srand((unsigned)time(NULL));
    Territorio *mapa = calloc(MAX_TERRITORIOS, sizeof *mapa);
    if (!mapa) {
        perror("Erro na alocação");
        return EXIT_FAILURE;
    }

    Phase fase = PHASE_SETUP;

    while (fase != PHASE_EXIT) {
        switch (fase) {
            case PHASE_SETUP:
                inicializaTerritorios(mapa, MAX_TERRITORIOS);
                fase = PHASE_MISSION;
                break;

            case PHASE_MISSION:
                missaoTipo = rand() % 2;
                if (missaoTipo == 0)
                    puts("Missão: Destruir o exército Verde.\n");
                else
                    puts("Missão: Conquistar 3 territórios.\n");
                exibirMapa(mapa, MAX_TERRITORIOS);
                fase = PHASE_MENU;
                break;

            case PHASE_MENU: {
                int opc;
                puts("=== Menu Principal ===");
                puts("1 - Atacar");
                puts("2 - Verificar Missão");
                puts("0 - Sair");
                printf("Escolha: ");
                scanf("%d", &opc);
                limparBuffer();
                putchar('\n');
                if (opc == 1) fase = PHASE_ATTACK;
                else if (opc == 2) fase = PHASE_VERIFY;
                else if (opc == 0) fase = PHASE_EXIT;
                else {
                    puts("Opção inválida. Tente novamente.\n");
                }
                break;
            }

            case PHASE_ATTACK:
                atacar(mapa, MAX_TERRITORIOS);
                exibirMapa(mapa, MAX_TERRITORIOS);
                fase = PHASE_MENU;
                break;

            case PHASE_VERIFY:
                if (verificarMissao(mapa, MAX_TERRITORIOS)) {
                    puts("Parabéns! Missão cumprida.\n");
                    fase = PHASE_EXIT;
                } else {
                    puts("Missão ainda não concluída.\n");
                    fase = PHASE_MENU;
                }
                break;

            default:
                fase = PHASE_EXIT;
                break;
        }
    }

    puts("Saindo do jogo. Até a próxima!");
    free(mapa);
    return EXIT_SUCCESS;
}
