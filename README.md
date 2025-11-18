# grupo06_ra3

Projeto para estudos e implementação de containers utilizando conceitos de sistemas operacionais.


## Descrição do Projeto

Este projeto visa desenvolver um conjunto de ferramentas para monitoramento, análise e controle do uso de recursos computacionais utilizando os conceitos avançados de containers no Linux, abrangendo namespaces e cgroups. O foco é analisar os principais trade-offs entre isolamento, desempenho e complexidade, bem como produzir experimentos científicos reprodutíveis relacionados ao gerenciamento de recursos do sistema operacional, acompanhando métricas de processos em diferentes níveis de isolamento e limitação.

O trabalho é dividido em três componentes principais:
1. **Resource Profiler**  
   Ferramenta que coleta e reporta métricas detalhadas de processos, incluindo uso de CPU (user time, system time, context switches, threads), memória (RSS, VSZ, page faults, uso de swap), IO (bytes lidos/escritos, syscalls, operações de disco) e rede (bytes rx/tx, pacotes, conexões).
2. **Namespace Analyzer**  
   Ferramenta de análise e relatório do isolamento via namespaces, identificando todos os namespaces ativos no sistema, comparando namespaces entre processos, mapeando processos por namespace, medindo overhead de criação e gerando relatórios.
3. **Control Group Manager**  
   Ferramenta de leitura e manipulação de cgroups, coletando métricas (CPU, memória, blkio, pids), criando cgroups experimentais, aplicando limites de recursos e gerando relatórios de utilização.

## Tecnologias Utilizadas

- **C** (86%) – código principal de manipulação de containers
- **HTML** (6.5%) – front-end ou documentação
- **Makefile** (3.6%) – scripts de build
- **Shell Script** (2.4%)
- **Python** (1.5%) – scripts adicionais
- Plataforma: Ubuntu/Linux

## Estrutura do Projeto

- *OBRIGATÓRIA*

resource-monitor/
├── README.md
├── Makefile
├── docs/
│   ├── ARCHITECTURE.md
│   └── DOCUMENTAÇÃO RESULTADOS EXPERIMENTAIS.pdf
├── include/
│   ├── monitor.h
│   ├── namespace.h
│   └── cgroup.h
├── src/
│   ├── cpu_monitor.c
│   ├── memory_monitor.c
│   ├── io_monitor.c
│   ├── rede_monitor.c
│   ├── namespace_analyzer.c
│   ├── cgroup_manager.c
│   └── main.c
├── tests/
│   ├── test_cpu.c
│   ├── test_memory.c
│   └── test_io.c
└── scripts/
    ├── visualize.py
    └── compare_tools.sh

+ parte extra: dashboard.html


## Pré-requisitos

- **Sistema operacional:** Ubuntu Linux ≥ 22.04 (recomendado 24.04)
- **Kernel:** Suporte a cgroups v1/v2 e namespaces (≥ 5.10)
- **Compilador:** gcc ≥ 9.3 (flags: `-std=c99` ou `-std=c23`, `-Wall -Wextra`)
- **make:** ≥ 4.2
- **Python 3:** ≥ 3.8 (para scripts extras)
- **Permissões:** Algumas operações requerem sudo/root (cgroups/namespaces avançados)
- **Ferramentas recomendadas:** strace, perf, valgrind, gdb, clang-tidy, cppcheck, htop, systemd-cgtop, gnuplot, matplotlib

## Instalação e Compilação

Clone o repositório e compile todos os módulos:
git clone https://github.com/Pimenteeel/grupo06_ra3.git
cd grupo06_ra3
make

Para compilar módulos individualmente:
make monitor # Resource Profiler
make namespace # Namespace Analyzer
make cgroup # Control Group Manager


## Uso

- **Resource Profiler**
  Monitora um processo por PID, coletando CPU/memória/IO e exportando em CSV/JSON.
  ./bin/monitor <PID> [opções]

- **Namespace Analyzer**
  Lista e compara namespaces dos processos, gera relatórios e avalia o overhead de isolamento.
  ./bin/namespaceanalyzer <PID> [opções]

- **Control Group Manager**
  Lê e configura cgroups, aplica limites de recursos e exporta relatórios.
  ./bin/cgroupmanager <PID> [opções]

- Scripts auxiliares para visualização e comparação estão em scripts/


## Funcionalidades Mínimas

- Monitoramento de processos via PID, com métricas de CPU, memória e IO.
- Exportação dos resultados (CSV/JSON).
- Listagem e análise de namespaces dos processos.
- Operações básicas com cgroups: criar, mover processo, aplicar limites e gerar relatórios.
- Compilação sem warnings (-Wall -Wextra) e código bem estruturado.
- Makefile funcional.
- README completo.

## Documentação dos Experimentos

Experimentos obrigatórios a serem documentados:
1. **Overhead de Monitoramento:** medir o impacto do profiler no sistema com comparação de workloads.
2. **Isolamento via Namespaces:** validação de efetividade do isolamento e tabela de overhead.
3. **Throttling de CPU:** análise de precisão de limites aplicados via cgroups em workloads CPU-intensive.
4. **Limitação de Memória:** avaliação do comportamento e do OOM killer ao atingir limites de memória.
5. **Limitação de IO:** validação da precisão das restrições de IO e análise de throughput.

Inclua metodologia, métricas usadas, gráficos/relatórios e conclusões para cada experimento no diretório `docs/`.

## Referências

- Frank Alcântara – Exercícios, atividades e materiais de sistemas operacionais, disponível em: [https://frankalcantara.com/](https://frankalcantara.com/) (acesso em 17/11/2025)
- Kerrisk, M. _The Linux Programming Interface_. No Starch Press, 2010.
- Documentação oficial do kernel Linux: [https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v2.html](https://www.kernel.org/doc/html/latest/admin-guide/cgroup-v2.html)
- man 7 namespaces, man 7 cgroups.
- Docker documentation: [https://docs.docker.com/config/containers/](https://docs.docker.com/config/containers/)
- Rice, L. “Container Security”, O'Reilly Media, 2020.

## Autores e Créditos

- Rafael Pimentel Leal ([Pimenteeel](https://github.com/Pimenteeel))
- Daniel Dagostim Munhoz
- Henrique Sanches de Andrade
- João Vitor Juk Lopes

Cada membro participou de partes distintas (monitoramento, análise de namespaces, manipulação de cgroups), mas todos devem dominar a lógica dos três componentes.



---------------------------------------------------------------------------------------------------------------------------------

