#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"

int metricas_swap(int pid, MemMetrics *mem){
    char proc_path[256]; // variável para guardar caminho do /proc
    FILE *fp; // ponteiro para abrir o proc_path para leitura

    sprintf(proc_path, "/proc/%d/status", pid); // função para formatar o proc_path
    fp = fopen(proc_path, "r"); // abrir caminho do proc para leitura

    if(fp == NULL){ // se o ponteiro for nulo, retorna -1 (insucesso)
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096]; // variavel de buffer para guardar informacoes do fopen
    char *pt; // variavel para percorrer a string
    mem -> swap = 0; // inicialização das variáveis de memoria
    int valor_encontrado = 0; // inicialização do valor para parar o loop

    while(fgets(buffer, sizeof(buffer), fp) != NULL){ // enquanto o buffer nao ficar vazio com as informacoes de fp armazenadas no buffer 
        if (valor_encontrado == 0 && strstr(buffer, "VmSwap") != NULL){ // se achar o VmSwap no buffer e o valor ainda não for encontrado
            pt = strchr(buffer, ':'); // pega os valores depois do ":"
            if(pt != NULL){
                mem -> swap = atol(pt + 1) * 1024; // armazena o valor
                valor_encontrado = 1; // valor encontrado
            }
        }
        if(valor_encontrado == 1){
            break; // sai do loop
        }
    }

    fclose(fp); //fecha a leitura

    return 0;
}

int metricas_MEM(int pid, MemMetrics *mem){
    char proc_path[256]; // variável para guardar caminho do /proc
    FILE *fp; // ponteiro para abrir o proc_path para leitura

    sprintf(proc_path, "/proc/%d/stat", pid); // função para formatar o proc_path
    fp = fopen(proc_path, "r"); // abrir caminho do proc para leitura

    if(fp == NULL){ // se o ponteiro for nulo, retorna -1 (insucesso)
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096]; // variavel de buffer para guardar informacoes do fopen
    if(fgets(buffer, sizeof(buffer), fp) == NULL){  /// se o conteudo do fp que esta armazenando no buffer for vazio, erro
        printf("Erro ao ler dados do arquivo %s", proc_path);
        fclose(fp);
        return -1;
    }

    fclose(fp);

    char *token; //Para percorrer o buffer
    int contagem = 1; // contagem de acordo com o número de tokens
    mem -> rss = 0; // inicialização da variável que vem da struct
    mem -> vsize = 0; // inicialização da variável que vem da struct
    long minor_faults = 0; // inicialização da variável para page_faults
    long major_faults = 0; // inicialização da variável para page_faults

    token = strtok(buffer, " "); // tokeniza o buffer

    while (token != NULL && contagem <= 24)
    {
        switch (contagem)
        {
        case 10: // se o token for 10
            minor_faults = atol(token); // armazena minor_faults
            break;
        case 12:
            major_faults = atol(token); // armazena major_faults
            break;
        case 23:
            mem -> vsize = atol(token); // armazena vsize
            break;
        case 24:
            mem -> rss = atol(token) * 4096; // armazena rss
            break;
        }
        token = strtok(NULL, " "); // coloca um nulo nos espaços
        contagem++;
    }
    mem -> page_faults = minor_faults + major_faults; // page faults será a soma de minor e major faults
    
    return 0;
}