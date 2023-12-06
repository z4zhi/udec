import matplotlib.pyplot as plt
import pandas as pd

# Cargar datos de los archivos CSV
datos = pd.read_csv('./proyectos/p2/2.csv')
n = datos['n'].tolist()
# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(n, datos['t1kmp'], label='KMP',marker='o')
plt.plot(n, datos['t1sa'], label='SA',marker='o')
plt.plot(n, datos['t1fmi'], label='FMI',marker='o')

plt.xlabel('m')
plt.ylabel('Tiempo')
plt.title('Tiempo de busqueda, patron variando y string constante')
plt.legend()
plt.grid(True)
plt.xticks(n)

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t1kmp"][i]:.0f}', (n[i], datos['t1kmp'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t1sa"][i]:.0f}', (n[i], datos['t1sa'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos["t1fmi"][i]:.0f}', (n[i], datos['t1fmi'][i]), textcoords="offset points", xytext=(0,10), ha='center')    

plt.show()

