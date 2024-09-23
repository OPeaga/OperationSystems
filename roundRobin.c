#include <stdio.h>
#include <stdlib.h>

typedef enum {
    PRONTO,
    EM_EXECUCAO,
    BLOQUEADO,
    FINALIZADO
} EstadoProcesso;

typedef struct {
    int id;
    int tempoDeExecucao;  // Quanto tempo resta para este processo terminar
    int tempoDeChegada;   // Tempo de chegada do processo
    int tempoParaCompletar; // Tempo total que o processo precisa para completar
    int tempoDeEspera;    // Tempo que o processo esperou
    EstadoProcesso estado;
} Processo;

// Protótipos
void inicializaProcessos(Processo *processos, int n);
void roundRobin(Processo *processos, int n, int quantum);
void calcularTempoDeEspera(Processo *processos, int n, int tempoAtual, int processoAtual);

int main() {
    int n = 5;  // Número de processos
    int quantum = 4;  // Quantum para Round Robin

    Processo processos[5];

    // Inicializando os processos (tempo de chegada e tempo de execução)
    inicializaProcessos(processos, n);

    printf("..................\n");

    // Executar o algoritmo Round Robin
    roundRobin(processos, n, quantum);

    return 0;
}

// Função para inicializar processos com tempos de chegada e de execução
void inicializaProcessos(Processo *processos, int n) {
    for (int i = 0; i < n; i++) {
        processos[i].id = i + 1;
        processos[i].tempoDeChegada = i * 2;  // Tempo de chegada fictício
        processos[i].tempoParaCompletar = (i + 1) * 5;  // Tempo necessário para completar o processo (fictício)
        processos[i].tempoDeExecucao = processos[i].tempoParaCompletar;  // Inicialmente, o tempo de execução restante é o total
        processos[i].tempoDeEspera = 0;  // Inicialmente, não há tempo de espera
        processos[i].estado = PRONTO;
        printf("Processo %d: Chegada = %d, Execução = %d\n", processos[i].id, processos[i].tempoDeChegada, processos[i].tempoDeExecucao);
    }
}

// Implementação do algoritmo Round Robin
void roundRobin(Processo *processos, int n, int quantum) {
    int tempoAtual = 0;
    int processosRestantes = n;

    while (processosRestantes > 0) {
        for (int i = 0; i < n; i++) {
            if (processos[i].estado == FINALIZADO) {
                continue;  // Pula o processo se ele já foi finalizado
            }

            if (processos[i].estado == PRONTO && processos[i].tempoDeChegada <= tempoAtual) {
                processos[i].estado = EM_EXECUCAO;  // Coloca o processo em execução
            }

            if (processos[i].estado == EM_EXECUCAO) {
                printf("Processo %d está em execução...\n", processos[i].id);

                // Executa o processo por um quantum de tempo
                if (processos[i].tempoDeExecucao > quantum) {
                    processos[i].tempoDeExecucao -= quantum;
                    tempoAtual += quantum;
                    printf("Processo %d não foi finalizado, tempo restante: %d\n", processos[i].id, processos[i].tempoDeExecucao);
                } else {
                    // O processo pode ser finalizado
                    tempoAtual += processos[i].tempoDeExecucao;
                    processos[i].tempoDeExecucao = 0;
                    processos[i].estado = FINALIZADO;
                    processosRestantes--;
                    printf("Processo %d foi finalizado.\n", processos[i].id);
                }

                // Atualiza o tempo de espera de todos os outros processos
                for (int j = 0; j < n; j++) {
                    if (j != i && processos[j].estado == PRONTO && processos[j].tempoDeChegada <= tempoAtual) {
                        processos[j].tempoDeEspera += quantum;
                    }
                }
            }
            printf("Temporizador: %d\n",tempoAtual);
        }
    }

    // Exibe o tempo de espera e tempo de execução total dos processos
    printf("\nResultados finais:\n");
    for (int i = 0; i < n; i++) {
        printf("Processo %d: Tempo de espera = %d, Tempo para completar = %d\n", processos[i].id, processos[i].tempoDeEspera, processos[i].tempoParaCompletar);
    }
    printf("Todos os processos foram finalizados.\n");
}