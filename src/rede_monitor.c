REDE_MONITOR.C


//bibliotecas utilizadas para manipulação de arquivos, strings e entrada e saida
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"// aqui temos a definição da struct RedeMetrics e funções auxiliares de monitoramento de rede


// função recebe um PID e um ponteiro para a struct RedeMetrics utilizada para preenchimento de dados de uso de rede lidos do sistema
// função inicia montando caminho do arquivo /proc/<PID>/net/dev que armazena estatisticas de uso de rede associadas ao PID
// abre o arquivo para ler, caso ocorra falha, retorna -1
// inicializa buffer auxiliar para leitura do arquivo e ponteiro auxiliar para manipulação de strings
// zera todos os campos da struct, garantindo que os dados ''lixo'' sejam devidamente ''descartados''
int metricas_REDE(int pid, RedeMetrics* red) {
    char proc_path[256];
    FILE* fp;

    sprintf(proc_path, "/proc/%d/net/dev", pid);
    fp = fopen(proc_path, "r");

    if (fp == NULL) {
        perror("Erro ao abrir o processo");
        return -1;
    }

    char buffer[4096];
    char* pt;
    red->bytes_rx = 0;
    red->bytes_tx = 0;
    red->packets_rx = 0;
    red->packets_tx = 0;


    // O loop é responsavel por ler o arquivo e procurar a entrada referente a interface de rede "eth0",
    // quando a encontra, processa os valores separados por espaços, utilizando strtok para dividir os campos,
    // extrai os principais dados, bytes recebidos (bytes_rx), pacotes recebidos (packets_rx), bytes transmitidos(bytes_tx) e...
    //... pacotes transmitidos (packtes_rx), esses dados sao extraídos conforme a ordem do arquivo
    // campos intermediarios sao ignorados por meio de lacos ou chamadas de strtok
    // após os processamentos o loop é encerrado para evitar leituras desnecessarias
    // toda logica depende da estrutura da linha "eth0" e da ordem especifica dos campos estatisticos do kernel Linux
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (strstr(buffer, "eth0") != NULL) {
            pt = strchr(buffer, ':');
            if (pt != NULL) {

                char* token;

                token = strtok(pt + 1, " ");
                if (token != NULL) {
                    red->bytes_rx = atol(token);
                }

                token = strtok(NULL, " ");
                if (token != NULL) {
                    red->packets_rx = atol(token);
                }

                for (int i = 3; i <= 8; i++) {
                    token = strtok(NULL, " ");
                    if (token == NULL) {
                        break;
                    }
                }

                if (token != NULL) {
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        red->bytes_tx = atol(token);
                    }
                }

                if (token != NULL) {
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        red->packets_tx = atol(token);
                    }
                }
                break;
            }
        }
    }
    fclose(fp);
    return 0;
}