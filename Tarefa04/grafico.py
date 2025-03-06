import matplotlib.pyplot as plt
import pandas as pd

# Carregar os dados do arquivo CSV
df = pd.read_csv('execution_times.csv')

# Gráfico 1: Tempo total de execução vs Número de threads
plt.figure(figsize=(10, 5))
plt.plot(df['NumThreads'], df['TotalTime'], marker='o', label='Tempo Total')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo (segundos)')
plt.title('Tempo Total de Execução vs Número de Threads')
plt.grid(True)
plt.legend()
plt.show()

# Gráfico 2: Tempo de pesquisa vs Número de threads
plt.figure(figsize=(10, 5))
plt.plot(df['NumThreads'], df['SearchTime'], marker='o', color='red', label='Tempo de Pesquisa')
plt.xlabel('Número de Threads')
plt.ylabel('Tempo (segundos)')
plt.title('Tempo de Pesquisa vs Número de Threads')
plt.grid(True)
plt.legend()
plt.show()
