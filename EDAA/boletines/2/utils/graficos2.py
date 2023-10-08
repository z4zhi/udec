import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Lee el archivo CSV y almacena los datos en un DataFrame
data = pd.read_csv('2/binary.csv')

# Extrae las columnas relevantes para la gráfica
x = data['n']
n = np.linspace(0, len(x) - 1, len(x))

tInsert = data['tInsert']
tRemove = data['tRemove']
# tUnion = data['tUnion']

# Crea el gráfico
plt.figure(figsize=(10, 6))

# Grafica las curvas tInsert, tRemove y tUnion
plt.plot(n, tInsert, label='Insert')
plt.plot(n, tRemove, label='Remove')
# plt.plot(n, tUnion, label='Union')

# Añade etiquetas y título
plt.xlabel('n')
plt.ylabel("Tiempo (nanosegundos)")
plt.title('Insert vs Remove, BinaryHeap')

# Añade una leyenda
plt.legend()

# Muestra la gráfica
plt.grid(True)
plt.xticks(n, x)
for xi, yi in zip(n, tInsert):
    plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8, color="blue") 
for xi, yi in zip(n, tRemove):
    plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="right", fontsize=8, color="orange")  
# for xi, yi in zip(n, tUnion):
#     plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="right", fontsize=8, color="green")   
plt.show()
