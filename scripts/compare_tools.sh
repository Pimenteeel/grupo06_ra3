
#armazenamento do primeiro argumento(informacao) jogada ao script e interpretada como o PID que deve ser analisado
#caso esteja vazio, sem informacao do PID, retorna erro e encerra
#script só sera executado de forma correta se e somente se o usuario mandar o PID correto do processo alvo
PID_ALVO=$1

if [ -z "$PID_ALVO" ]; then
    echo "Uso: ./compare_tools.sh <PID>"
    exit 1
fi

#cabeçalho com PID
echo "---------------------------------------------"
echo " Comparação de métricas para o PID: $PID_ALVO"
echo "---------------------------------------------"

echo ""
echo "===> Métricas coletadas pelo resource-monitor"

#Execução do programa, entrando em modo de monitoramento com a flag -r (faz coleta e registro das informacoes em tempo real)
#limita o tempo de execução do resource-monitor para nao ter loops desnecessarios
timeout 10 ../resource-monitor -r "$PID_ALVO"
echo "Coleta encerrada."
echo ""


#Realiza novas coletas metricas com o comando comum ps aux para o processo alvo, mostra PID, pcpu(porcentagem de uso de CPU)...
#... pmem(uso de memória), vsz (memoria virtual), rss (memoria fisica)
echo "===> Métricas coletadas no ps aux:"
ps -p "$PID_ALVO" -o pid,pcpu,pmem,vsz,rss
echo ""

#Exibição do total de memoria do sistema com o comando free, para comparar utilizacao total com uso individual do processo
echo "===> Memória total do sistema (free -m)"
free -m
echo ""

#Exibição do uso geral de CPU do sistema, pegando as primeiras linhas de execução do comando top(uma unica amostra)...
#... e mostra o contexto do uso de recursos apresentados por todas as ferramentas acima
echo "===> Uso de CPU (top -b -n 1)"
top -b -n 1 | head -n 10
echo ""

echo "---------------------------------------------"
echo " Comparação finalizada"
echo "---------------------------------------------"