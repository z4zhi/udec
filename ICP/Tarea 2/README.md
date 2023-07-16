# README

Código en C++ que utiliza CUDA para encontrar el valor máximo en n arreglos de números enteros.

Cada arreglo se procesa en su propio stream de CUDA para habilitar la ejecución en paralelo. El kernel `reduce` utiliza cooperative groups para realizar la reducción en paralelo y encontrar el valor máximo en cada arreglo. El resultado se guarda en un arreglo `maximos`.

## Instrucciones

1.  Para compilar

    `nvcc reduccion_max.cu -o ejecutable`

2.  Para ejecutar

    `./ejecutable -n <n> -k <k> -p -b <bloques>`

Donde:

- `<n>` es el numero de arreglos
- `<k>` es el tamaño de los arreglos.
- `-p` imprimir los arreglos generados.
- `<bloques>` numero de bloques

3. Ejemplo:

`./ejecutable -n 3 -k 5 -p -b 2`
