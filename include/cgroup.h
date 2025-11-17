#ifndef CGROUP_H
#define CGROUP_H

#include <stdbool.h>
#include <sys/types.h>

#define MAX_PATH 1024 // Tamanho máximo para caminhos de arquivo

// Funções auxiliares (agora públicas para o main.c)

// Escreve um valor em um arquivo do sistema (usado para configurar cgroups)
bool write_to_file(const char* filepath, const char* value);

// Lê um valor unsigned long long de arquivos do cgroup (ex: estatísticas de CPU)
unsigned long long read_from_file_ull(const char* filepath);

// Lê um valor long de arquivos do cgroup (ex: uso de memória)
long read_from_file_long(const char* filepath);

// Funções principais do Control Group Manager

// Coleta e exibe métricas de uso do cgroup (CPU, memória, PIDs, etc.)
bool coletar_metricas_cgroup(const char* cgroup_path);

// Cria um novo cgroup e configura limites de CPU e memória
bool criar_configurar_cgroup(const char* controller, const char* name, double cpu_cores, long memoria_mb);

// Move um processo específico para dentro do cgroup
bool move_process_to_cgroup(const char* cgroup_path, int pid);

// Esvazia e remove um cgroup (limpeza de recursos)
bool empty_and_delete_cgroup(const char* cgroup_path);

// Gera um relatório em CSV com métricas de utilização do cgroup
void gerar_relatorio_utilizacao(const char* cgroup_path);

#endif // CGROUP_H