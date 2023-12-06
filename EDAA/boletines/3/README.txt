# README

Existen dos códigos: uno que implementa Suffix Array y otro que implementa KMP. Ambos son codigos comentados de GeeksforGeeks.

## Compilación y Ejecución

Para compilar cualquiera de los 2 archivos:

    1. $ g++ sa.cpp
o
    2. $ g++ kmp.cpp

Ejecución:

$ ./a.out

El programa se ejecuta con dos tipos de análisis experimental:

1. Tamaño de string variable (100000, 200000, 300000 , ..., 1000000), patrón a buscar constante.
2. Patrón a buscar variable (100000, 200000, 300000 , ..., 1000000), tamaño de string constante (1000000).

Para ejecutar el análisis experimental 1, proporcionar cualquier argumento adicional, ejemplo:

$ ./a.out a

Para ejecutar el análisis experimental 2, proporcionar cualquier 2 argumentos adicionales, ejemplo:

$ ./a.out a a