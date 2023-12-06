import matplotlib.pyplot as plt
import pandas as pd

# Cargar datos de los archivos CSV
datos_kmp = pd.read_csv('./3/kmp1.csv')
datos_tsa = pd.read_csv('./3/sa1.csv')
n = datos_kmp['n'].tolist()
# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(n, datos_kmp['tkmp'], label='KMP',marker='o')
plt.plot(n, datos_tsa['tsa'], label='SA',marker='o')

plt.xlabel('n')
plt.ylabel('Tiempo')
plt.title('String variando, patron constante')
plt.legend()
plt.grid(True)
plt.xticks(n)

for i, txt in enumerate(n):
    plt.annotate(f'{datos_kmp["tkmp"][i]:.0f}', (n[i], datos_kmp['tkmp'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos_tsa["tsa"][i]:.0f}', (n[i], datos_tsa['tsa'][i]), textcoords="offset points", xytext=(0,10), ha='center')
plt.show()

