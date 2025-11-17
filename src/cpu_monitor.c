#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

int metricas_switches(int pid, CpuMetrics *cpu){
    char proc_path[256]; // variável para guardar caminho do /proc
    FILE *fp; // ponteiro para abrir o proc_path para leitura

    sprintf(proc_path, "/proc/%d/status", pid); // função para formatar o proc_path
    fp = fopen(proc_path, "r"); // abrir caminho do proc para leitura

    if(fp == NULL){ // se o ponteiro for nulo, retorna -1 (insucesso)
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096]; // variavel ve buffer para guardar informacoes do fopen
    char *pt; // variavel para percorrer a string
    long voluntary = 0; // inicialização das variáveis de context switches
    long nonvoluntary = 0; // inicialização das variáveis de context switches

    int valor_encontrado_voluntary = 0; // inicialização do valor para parar o loop
    int valor_encontrado_nonvoluntary = 0; // inicialização do valor para parar o loop


    while(fgets(buffer, sizeof(buffer), fp) != NULL){ // enquanto o buffer nao ficar vazio com as informacoes de fp
        if (valor_encontrado_voluntary == 0 && strstr(buffer, "voluntary_ctxt_switches") != NULL){ // strstr é uma função de string de string e aqui está procurando a string voluntary_ctxt_switches
            pt = strchr(buffer, ':'); // pega o : da linha
            if(pt != NULL){
                voluntary = atol(pt + 1); // pega o valor do context switches voluntary
                valor_encontrado_voluntary = 1; // encontrou o valor
            }
        }
        if (valor_encontrado_nonvoluntary == 0 && strstr(buffer, "nonvoluntary_ctxt_switches") != NULL){ // strstr é uma função de string de string e aqui está procurando a string nonvoluntary_ctxt_switches
            pt = strchr(buffer, ':');
            if(pt != NULL){
                nonvoluntary = atol(pt + 1); // pega o valor de context switches non voluntary
                valor_encontrado_nonvoluntary = 1; // encontrou o valor
            }
        }
        if (valor_encontrado_voluntary == 1 && valor_encontrado_nonvoluntary == 1){ // se ambos valores foram encontrados
            break; // sai do loop
        }
    }

    fclose(fp); // fecha o arquivo que abrimos para leitura

    cpu -> switches = voluntary + nonvoluntary; // a variavel long switches lá no CpuMetrics recebe a soma do voluntary + nonvoluntary;

    return 0;
}

int metricas_CPU(int pid, CpuMetrics *cpu){
    char proc_path[256]; // variável para guardar caminho do /proc
    FILE *fp; // ponteiro para abrir o proc_path para leitura

    sprintf(proc_path, "/proc/%d/stat", pid); // função para formatar o proc_path
    fp = fopen(proc_path, "r"); // abrir caminho do proc para leitura

    if(fp == NULL){ // se o ponteiro for nulo, retorna -1 (insucesso)
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096]; // variavel ve buffer para guardar informacoes do fopen
    if (fgets(buffer, sizeof(buffer), fp) == NULL) /// se o conteudo do fp que esta armazenando no buffer for vazio, erro
    {
        printf("Erro ao ler dados do arquivo %s", proc_path);
        fclose(fp);
        return -1;
    }

    fclose(fp);

    char *token; //Para percorrer o buffer
    int contagem = 1; // contagem de acordo com o número de tokens
    cpu -> user_time = 0; // inicialização da variável que vem da struct
    cpu -> system_time = 0; // inicialização da variável que vem da struct
    cpu -> threads = 0; // inicialização da variável que vem da struct

    token = strtok(buffer, " "); // quebra os espaços do /proc/pid/status e tokeniza as variáveis

    while (token != NULL && contagem <= 20){ // enquanto o token nao for nulo e a contagem for menor que 20
        switch (contagem)
        {
        case 14: // se o token for 14
            cpu -> user_time = atol(token); // armazena o ascii para o long (user time)
            break;
        case 15: // se o token for 15
            cpu -> system_time = atol(token); // armazena o ascii para o long (system time)
            break;
        case 20: // se o token for 20
            cpu -> threads = atol(token); // armazena o ascii para o long (threads)
            break;
        }
        token = strtok(NULL, " "); // coloca as variáveis lidas com um NULL no lugar do espaço para continuar a tokenizacao
        contagem++;
    }

    return 0;
}

long total_ticks_sistema() {
    FILE *fp;
    fp = fopen("/proc/stat", "r");

    if (fp == NULL){
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096];
    if(fgets(buffer, sizeof(buffer), fp) == NULL){
        fclose(fp);
        return -1;
    }
    fclose(fp);

    char *token = strtok(buffer, " ");
    long total_ticks = 0;
    int i = 0;

    while((token = strtok(NULL, " ")) != NULL){
        if (i >= 7){
            break;
        }
        total_ticks = total_ticks + atol(token);
        i++;
    }

    return total_ticks;
}