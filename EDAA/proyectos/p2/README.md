# README Proyecto 2: Conteo de patrones

## Descripción

Este proyecto implementa 3 algoritmos para la búsqueda y conteo de patrones dentro de una cadena de texto. El proyecto consta de 4 archivos principales: `kmp.cpp`, `fmindex.cpp`, `sa.cpp` y `main.cpp`.

- `kmp.cpp` implementa el algoritmo de búsqueda KMP como una clase, que incluye el arreglo LPS y almacena el patrón. Para la función de conteo, se requiere pasar la cadena de texto `T`.
- `sa.cpp` implementa el Suffix Array como una clase. La implementación del Suffix Array no es propia y fue obtenida de "GeeksforGeeks". La implementación del algoritmo de búsqueda es propia y requiere pasar el patrón `p`.
- `fmindex.cpp` implementa el FM-Index como una clase. Contiene la Transformación de Burrows-Wheeler (`BWT`), la tabla `C`, la tabla de ocurrencias y las funciones `LF(i)` y `count(p)`.
- Cada implementación también incluye la función `space()`, que devuelve el tamaño de la estructura de datos (sin contar los strings almacenados).
- `main.cpp` implementa la ejecución principal con el análisis experimental y un programa que lee una cadena de texto y un patrón.

## Compilación y Ejecución

### Para compilar:

```bash
g++ main.cpp
```

### Ejecución:

El programa puede ser ejecutado de las siguientes maneras:

1. `./a.out` sin proporcionar argumentos adicionales. Ejecutará el modo de análisis experimental.

   - Dos tipos de análisis experimental:
     - Tamaño de la cadena variable (10000, 20000, 30000, ..., 100000) con un patrón constante (1000).
     - Patrón variable (10000, 20000, 30000, ..., 100000) con una cadena de texto constante (100000).

2. `./a.out T p`. Al proporcionar 2 argumentos (la cadena seguida del patrón), los 3 algoritmos buscarán el patrón en la cadena y mostrarán los resultados.
