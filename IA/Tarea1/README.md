# Descripción
Código en C++ que implementa: 
 -	Búsqueda en profundidad (escogiendo un sucesor al azar)
 -	Búsqueda por costo uniforme
 -	Búsqueda greedy
 -	A*

## Funcionamiento
 1. Se crea un struct "nodo”, que contiene la información del nodo, como su nombre, heurística, etc. Además, contiene un vector que apunta a sus nodos vecinos para representar las aristas del grafo. 
 2. Dependiendo del tipo de búsqueda seleccionado, se escoge el siguiente nodo del vector de nodos vecinos para la siguiente iteración del algoritmo, dentro de un bucle que se ejecuta hasta encontrar un nodo hoja (sin vecinos) o el nodo solución (o la solución óptima), según la búsqueda seleccionada. 
 3. Finalmente, al encontrar un nodo hoja (sin vecinos) o el nodo solución (o la solución óptima) se detendrá la búsqueda y se almacena el camino, costo y nodos expandidos.

## Cosas a tener en cuenta
 - Para la búsqueda por costo uniforme y A*, no esta implementada la poda, es decir, si existe un camino hacia un nodo que tiene un costo menor en comparación con otro camino hacia ese mismo nodo, ambos caminos son expandidos, lo que es innecesario.
 - El programa ha sido probado y funciona en sistema operativo Windows (11) y compilador g++ 12.2.0.

# Instrucciones 
- Para compilar: $ g++ busquedas.cpp

	- `g++ busquedas.cpp`

- Para ejecutar: $ ejecutable grafo.txt

	- `./a.exe grafo.txt`

- Luego, se imprimirá en la consola el grafo ingresado y los resultados de las 4 búsquedas. 


