import csv
import matplotlib.pyplot as plt
import numpy as np

# Nombres de los archivos CSV
archivo1 = "2/binary.csv"
archivo2 = "2/binomial.csv"

# Métodos que deseas comparar
metodos = ["tInsert", "tRemove", "tUnion"]

# Crear gráficos para cada método
for metodo in metodos:
    plt.figure(figsize=(10, 6))

    # Leer datos del primer archivo CSV
    n = []
    tiempos1 = []

    with open(archivo1, "r") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            n.append(int(row["n"]))
            tiempos1.append(float(row[metodo]))

    # Leer datos del segundo archivo CSV
    tiempos2 = []

    with open(archivo2, "r") as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            tiempos2.append(float(row[metodo]))

    x = np.linspace(0, len(n) - 1, len(n))
    # Graficar los datos
    plt.plot(x, tiempos1, label=f"Binary Heap")
    plt.plot(x, tiempos2, label=f"Binomial Heap")

    plt.xlabel("n")
    plt.ylabel("Tiempo (nanosegundos)")
    plt.title(f"Tiempo {metodo[1:]}")
    plt.legend()
    plt.grid(True)
    plt.xticks(x, n)
    for xi, yi in zip(x, tiempos1):
        plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="left", fontsize=8, color="blue") 
    for xi, yi in zip(x, tiempos2):
        plt.text(xi, yi, f"{yi:.0f}", va="bottom", ha="right", fontsize=8, color="orange")      
    plt.show()
