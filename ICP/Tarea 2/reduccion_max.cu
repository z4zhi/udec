#include <iostream>
#include <vector>
#include <math.h>
#include <cooperative_groups.h>
#include <algorithm>
#include <random>
#include <cuda_runtime.h>

using namespace std;
using namespace cooperative_groups;


// Kernel para encontrar el maximo
__device__ int reduce_max(thread_group g, int *temp, int val) {
	int tid = g.thread_rank();

	// toma una mitad para compararla con la otra, luego la mitad de la primera mitad, y asi
	for (int i = blockDim.x/2; i>0; i /= 2){
		// cada hilo toma un valor, se espera a que todos terminen
		temp[tid] = val;	
		g.sync();

		// cada hilo compara su valor con el de la otra mitad respectivo y guarda el maximo, se espera a que todos terminen
		if(tid<i) val = max(val, temp[tid + i]);
		g.sync();
	}

	return val;
}


// Kernel inicio, cooperative groups
__global__ void reduce(int *maximos, int *input){
	// memoria compartida del stream
    extern __shared__ int temp[];

	// id del thread
    int id = blockIdx.x*blockDim.x + threadIdx.x;		

	// grupo cooperativo
    thread_group g = this_thread_block();

	// llamada al reduce 
    int block_max = reduce_max(g, temp, input[id]);

	// se guarda el valor maximo, la primera hebra es la encargada en este caso
    if (threadIdx.x == 0) atomicMax(maximos, block_max);
}


int main(int argc, char *argv[]) {
	// arreglos, tamaño
	int n = 3, k1 = 5, p = 0, bloques = -1;
	for(int i=0; i<argc; i++){
		if( !strcmp(argv[i], "-n" ) ) n = atoi(argv[i+1]);
		if( !strcmp(argv[i], "-k" ) ) k1 = atoi(argv[i+1]);
		if( !strcmp(argv[i], "-p" ) ) p = 1;
		if( !strcmp(argv[i], "-b" ) ) bloques = atoi(argv[i+1]);
	}


	int k = pow(2, ceil(log(k1)/log(2)));

	// memoria
	vector<int*> arreglosDst(n), arreglosSrc(n), maximos(n);
    for (int i = 0; i < n; i++) {
        cudaMallocHost(&arreglosSrc[i], k * sizeof(int));
        cudaMalloc(&arreglosDst[i], k * sizeof(int));
        cudaMallocHost(&maximos[i], sizeof(int));
    }
	

	// creacion arreglos y relleno de ceros para que sea potencia de 2
	for(int i=0; i<n; i++){
		for(int j=0; j<k; j++){
			if(j < k1) arreglosSrc[i][j] = j;
			else arreglosSrc[i][j] = 0;
		}
		shuffle(arreglosSrc[i], arreglosSrc[i] + k1, mt19937{random_device{}()});
	}

	// print arreglos
	if(p){
		for(int i=0; i<n; i++){
			for(int j=0; j<k; j++){
				cout << arreglosSrc[i][j] << " ";
			}
			cout << endl;
		}
	}

	// streams
	vector<cudaStream_t> stream(n);
	for (int i=0; i<n; i++) cudaStreamCreate(&stream[i]);

	// calculo para no superar el limite de hebras del bloque
	if(bloques == -1) bloques = 2 + k/1024;
	int hebras = (k + bloques - 1) / bloques;
	int sharedBytes = hebras * sizeof(int);

	cout << "Bloques:    " << bloques << endl;
	cout << "Hebras:     " << hebras << endl;
	cout << "sharedbits: " << sharedBytes << endl;
	
	for (int i=0; i<n; i++) {
		// copia arreglo host a gpu
		cudaMemcpyAsync(arreglosDst[i], arreglosSrc[i], k * sizeof(int), cudaMemcpyHostToDevice, stream[i]);
		
		// kernel
		reduce<<<bloques, hebras, sharedBytes, stream[i]>>>(maximos[i], arreglosDst[i]);
	}
	
	// Sincronizacion de los streams
	cudaDeviceSynchronize();	

	// print de los maximos
    for (int i = 0; i < n; i++) cout << maximos[i][0] << " ";

	// liberacion memoria y strems
	for(int i=0; i<n; i++){
		cudaFree(arreglosDst[i]);
		cudaFreeHost(arreglosSrc[i]);
		cudaStreamDestroy(stream[i]);
	}
	
	return 0;
}

