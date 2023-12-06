import matplotlib.pyplot as plt
import pandas as pd

# Cargar datos de los archivos CSV
datos = pd.read_csv('./proyectos/p2/2.csv')
n = datos['n'].tolist()
# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(n, datos['skmp'], label='KMP',marker='o')
plt.plot(n, datos['ssa'], label='SA',marker='o')
plt.plot(n, datos.iloc[:, -1], label='FMI',marker='o')

plt.xlabel('m')
plt.ylabel('Tiempo')
plt.title('Espacio con patron variando, string constante')
plt.legend()
plt.grid(True)
plt.xticks(n)

for i, txt in enumerate(n):
    plt.annotate(f'{datos["skmp"][i]:.0f}', (n[i], datos['skmp'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos["ssa"][i]:.0f}', (n[i], datos['ssa'][i]), textcoords="offset points", xytext=(0,10), ha='center')

for i, txt in enumerate(n):
    plt.annotate(f'{datos.iloc[:, -1][i]:.0f}', (n[i], datos.iloc[:, -1][i]), textcoords="offset points", xytext=(0,10), ha='center')    

plt.show()

