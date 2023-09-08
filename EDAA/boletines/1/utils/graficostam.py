import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# Leer los datos desde el archivo CSV
data = pd.read_csv("1/resultadostam.csv")

# Extraer los valores de las columnas
x = data["n"].tolist()
tiempo_sec = data["TiempoSec"].tolist()
tiempo_bin = data["TiempoBin"].tolist()
tiempo_exp = data["TiempoExp"].tolist()

n = np.linspace(0, len(x) - 1, len(x))

# Gráfico con las 3 curvas de tiempo
plt.figure(figsize=(10, 6))
# plt.plot(n, tiempo_sec, label="Secuencial")
plt.plot(n, tiempo_bin, label="Binaria")
plt.plot(n, tiempo_exp, label="Galopante")
plt.xlabel("Tamaño (n)")
plt.ylabel("Tiempo (nanosegundos)")
plt.title("Tiempos de ejecución todos")
plt.xticks(n, x)
plt.legend()
plt.grid(True)
# for xi, yi in zip(n, tiempo_sec):
#     plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8, color="blue")
for xi, yi in zip(n, tiempo_bin):
    plt.text(xi, yi, f"{yi:.0f}", va="top", ha="left", fontsize=8, color="blue")
for xi, yi in zip(n, tiempo_exp):
    plt.text(xi, yi, f"{yi:.0f}", va="top", ha="right", fontsize=8, color="orange")
plt.show()

# Gráfico solo con la curva de tiempo sec
plt.figure(figsize=(10, 6))
plt.plot(n, tiempo_sec)
plt.xlabel("Tamaño (n)")
plt.ylabel("Tiempo (nanosegundos)")
plt.title("Tiempo de ejecución Secuencial")
plt.xticks(n, x)
plt.grid(True)
for xi, yi in zip(n, tiempo_sec):
    plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8)
plt.show()

# Gráfico solo con la curva de tiempo bin
plt.figure(figsize=(10, 6))
plt.plot(n, tiempo_bin)
plt.xlabel("Tamaño (n)")
plt.ylabel("Tiempo (nanosegundos)")
plt.title("Tiempo de ejecución Binaria")
plt.xticks(n, x)
plt.grid(True)
for xi, yi in zip(n, tiempo_bin):
    plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8)
plt.show()

# Gráfico solo con la curva de tiempo exp
plt.figure(figsize=(10, 6))
plt.plot(n, tiempo_exp)
plt.xlabel("Tamaño (n)")
plt.ylabel("Tiempo (nanosegundos)")
plt.title("Tiempo de ejecución Galopante")
plt.xticks(n, x)
for xi, yi in zip(n, tiempo_exp):
    plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8)
plt.grid(True)
plt.show()
