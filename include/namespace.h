
//Headers de proteção que garantem que o conteudo dos headers seja incluido apenas uma vez no codigo
#ifndef NAMESPACE_H
#define NAMESPACE_H

//Definição da struct "ProcessNamespaces" que ARMAZENA inodes para cada tipo de namespace do linux
//Sendo cada campo um tipo de namespace e permite a manipulação dos dados obtidos em cada um
//usada por todas as funções modulos do namespace
 
typedef struct
{
    long cgroup;
    long ipc;
    long mnt;
    long net;
    long pid;
    long time;
    long user;
    long uts;
} ProcessNamespaces;

//Declaração das funções módulo de namespaces

int namespaces_por_pid(int pid, ProcessNamespaces *ns); //coleta e preenche identificadores de namespaces atrelados a um certo PID

void comparar_namespaces(int pid1, int pid2); //Comparação de namespaces de processos distintos

void mapear_todos_processos(); //percorre os processos do sistema, usando a funcao de "namespaces_por_pid" para pegar os dados de...
//... certo namespace e gerar um relatorio disso

void namespace_overhead(long iteracao); //mede custo extra de tempo para criacao de namespaces PID em relaçao a criacao de outros...
//... processos
// Todas as funcoes sao utilizadas no arquivo .c

#endif