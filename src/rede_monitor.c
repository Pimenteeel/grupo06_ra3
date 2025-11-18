//bibliotecas utilizadas para manipula��o de arquivos, strings e entrada e saida
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "monitor.h"// aqui temos a defini��o da struct RedeMetrics e fun��es auxiliares de monitoramento de rede


// fun��o recebe um PID e um ponteiro para a struct RedeMetrics utilizada para preenchimento de dados de uso de rede lidos do sistema
// fun��o inicia montando caminho do arquivo /proc/<PID>/net/dev que armazena estatisticas de uso de rede associadas ao PID
// abre o arquivo para ler, caso ocorra falha, retorna -1
// inicializa buffer auxiliar para leitura do arquivo e ponteiro auxiliar para manipula��o de strings
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


    // O loop � responsavel por ler o arquivo e procurar a entrada referente a interface de rede "eth0",
    // quando a encontra, processa os valores separados por espa�os, utilizando strtok para dividir os campos,
    // extrai os principais dados, bytes recebidos (bytes_rx), pacotes recebidos (packets_rx), bytes transmitidos(bytes_tx) e...
    //... pacotes transmitidos (packtes_rx), esses dados sao extra�dos conforme a ordem do arquivo
    // campos intermediarios sao ignorados por meio de lacos ou chamadas de strtok
    // ap�s os processamentos o loop � encerrado para evitar leituras desnecessarias
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