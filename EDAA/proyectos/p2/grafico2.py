import matplotlib.pyplot as plt
import pandas as pd

# Cargar datos de los archivos CSV
datos = pd.read_csv('./proyectos/p2/1.csv')
n = datos['n'].tolist()
# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(n, datos['t2kmp'], label='KMP',marker='o')
plt.plot(n, datos['t2sa'], label='SA',marker='o')
plt.plot(n, datos['t2fmi'], label='FMI',marker='o')

plt.xlabel('n')
plt.ylabel('Tiempo')
plt.title('String variando, patron constante')
plt.legend()
plt.grid(True)
plt.xticks(n)

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t2kmp"][i]:.0f}', (n[i], datos['t2kmp'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t2sa"][i]:.0f}', (n[i], datos['t2sa'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t2fmi"][i]:.0f}', (n[i], datos['t2fmi'][i]), textcoords="offset points", xytext=(0,10), ha='center')    

plt.show()

