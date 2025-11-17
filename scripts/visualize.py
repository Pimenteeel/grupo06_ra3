from flask import Flask, jsonify, render_template
from pathlib import Path
import pandas as pd

# Inicializa a aplicação Flask com pasta de templates personalizada
app = Flask(__name__, template_folder = "../templates")

# Define o caminho para o arquivo CSV de monitoramento
CSV = Path("../Resource_Monitor.csv")

@app.route("/")
def dashboard():
    """Rota principal que renderiza o dashboard HTML"""
    return render_template("dashboard.html")

@app.route("/api/dados")
def api_dados():
    """API que retorna os dados do CSV em formato JSON para o frontend"""
    
    # Verifica se o arquivo CSV existe
    if not CSV.exists():
        return jsonify([])
    
    try:
        # Lê o arquivo CSV com pandas
        dataframe = pd.read_csv(CSV)
    except:
        # Retorna lista vazia em caso de erro na leitura
        return jsonify([])
    
    # Pega apenas as últimas 100 linhas para performance
    dataframe = dataframe.tail(100)

    # Converte DataFrame para dicionário e depois para JSON
    dados = dataframe.to_dict(orient = "records")
    return jsonify(dados)

if __name__ == "__main__":
    # Executa o servidor Flask em modo debug
    app.run(debug = True)