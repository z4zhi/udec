import matplotlib.pyplot as plt
import pandas as pd

# Cargar datos de los archivos CSV
datos_segment = pd.read_csv("./4/segment.csv")
datos_sparse = pd.read_csv("./4/sparse.csv")
n = datos_segment["n"].tolist()
# Graficar los datos
plt.figure(figsize=(10, 6))
plt.plot(n, datos_segment["t2"], label="Segment Tree", marker="o")
plt.plot(n, datos_sparse["t2"], label="Sparse Table", marker="o")

plt.xlabel("n")
plt.ylabel("Tiempo")
plt.title("Tiempo RMQ")
plt.legend()
plt.grid(True)
plt.xticks(n)

for i, txt in enumerate(n):
    plt.annotate(
        f'{datos_segment["t2"][i]:.0f}',
        (n[i], datos_segment["t2"][i]),
        textcoords="offset points",
        xytext=(0, 10),
        ha="center",
    )

for i, txt in enumerate(n):
    plt.annotate(
        f'{datos_sparse["t2"][i]:.0f}',
        (n[i], datos_sparse["t2"][i]),
        textcoords="offset points",
        xytext=(0, 10),
        ha="center",
    )
plt.show()
