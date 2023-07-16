Compilar usando g++ matriz.cpp -o matriz.exe -fopenmp


Para ejecutar: matriz.exe 
	-n <numero>: matriz tamano 2^numero x 2^numero
	-p         : opcion para imprimir las matrices A, B, C
	-a <numero>: eleccion de algoritmo, default todos.
		0: Multiplicacion tradicional secuencial
		1: Multiplicacion tradicional paralela
		2: Multiplicacion amigable con el cache secuencial
		3: Multiplicacion amigable con el cache paralela
		4: Multiplicacion Strassen secuencial
		5: Multiplicacion Strassen paralela
		6: multiplicacion bloques secuencial
		7: multiplicacion bloques paralela
	
	-r         : para mostrar el resultado solo en numeros
