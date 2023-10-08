# README

Existen dos códigos: uno que implementa BinomialHeap de GeeksforGeeks (comentado) y otro que implementa BinaryHeap (implementación propia).

## Compilación y Ejecución

Para compilar cualquiera de los 2 archivos:

    1. $ g++ binary_heap.cpp
o
    2. $ g++ binomial_heap.cpp

Ejecución:

$ ./a.out

El programa se puede ejecutar de dos formas:

1. Forma normal: Se crea el heap correspondiente y se insertan valores de ejemplo. Luego se realizan algunas operaciones y se imprime el resultado.
   
2. Análisis experimental: Se varía el tamaño de entrada (100, 1000, ..., 100000000). 
Se insertan esos n elementos. 
Luego se realiza una prueba (tomando el tiempo) insertando un elemento, luego se remueve y finalmente se realiza la unión con el mismo heap.

Para ejecutar el análisis experimental, proporcionar cualquier argumento adicional, por ejemplo:

$ ./a.out a
