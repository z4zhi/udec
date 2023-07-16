#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
using namespace chrono;

typedef vector<vector<int>> vii;


void imprimir(vii &M, string nombre){
	cout << "Matriz " << nombre << endl;
	for(int i=0; i<M.size(); i++){
		for(int j=0; j<M.size(); j++){
			cout << setw(4) << M[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}


// 1. Matrices tradicionales
vii mat_trad_sec(vii &A, vii &B){
	int n = A.size();
	vector<vector<int>> C(n, vector<int>(n));

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			for(int k=0; k<n; k++) C[i][j] += A[i][k] * B[k][j];
		}
	}

	return C;
}

vii mat_trad_par(vii &A, vii &B){
	int n = A.size();
	vector<vector<int>> C(n, vector<int>(n));

	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				for(int k=0; k<n; k++) 
					C[i][j] += A[i][k] * B[k][j];
			}
		}

	return C;
}


// 2. Matrices amigables con el cache
vii mat_amigable_sec(vii &A, vii &B){
	int n = A.size();
	vector<vector<int>> C(n, vector<int>(n));

	vector<vector<int>> BT(n, vector<int>(n));
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++) BT[i][j] = B[j][i];
	}

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			int suma = 0;
			for(int k=0; k<n; k++){
				suma += A[i][k] * BT[j][k];
			}
			C[i][j] = suma;
		}
	}

	return C;
}

vii mat_amigable_par(vii &A, vii &B){
	int n = A.size();
	vector<vector<int>> C(n, vector<int>(n));

	vector<vector<int>> BT(n, vector<int>(n));
	
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++) BT[i][j] = B[j][i];
		}
		
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				int suma = 0;
				for(int k=0; k<n; k++){
					suma += A[i][k] * BT[j][k];
				}
				C[i][j] = suma;
			}
		}

	return C;
}


// 3. 3a y strassen secuencial
vii suma(vii &M1, vii &M2){
	int n = M1.size();
	vii M(n, vector<int>(n));

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			M[i][j] = M1[i][j] + M2[i][j];
		}
	}

	return M;
}

vii resta(vii &M1, vii &M2){
	int n = M1.size();
	vii M(n, vector<int>(n));

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			M[i][j] = M1[i][j] - M2[i][j];
		}
	}

	return M;
}

vii bloques_sec(vii A, vii B){
	if(A.size() <= 2 << 5) return mat_amigable_sec(A, B);

	int n = A.size()/2;

	vii A11(n, vector<int>(n)), A12(n, vector<int>(n));
	vii A21(n, vector<int>(n)), A22(n, vector<int>(n));
	vii B11(n, vector<int>(n)), B12(n, vector<int>(n));
	vii B21(n, vector<int>(n)), B22(n, vector<int>(n));

	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			A11[i][j] = A[i][j];
			A12[i][j] = A[i][j+n];
			A21[i][j] = A[i+n][j];
			A22[i][j] = A[i+n][j+n];
			B11[i][j] = B[i][j];
			B12[i][j] = B[i][j+n];
			B21[i][j] = B[i+n][j];
			B22[i][j] = B[i+n][j+n];
		}
	}

	vii aux1 = bloques_sec(A11, B11);
	vii aux2 = bloques_sec(A12, B21);
	vii C1 = suma(aux1, aux2);
	
	aux1 = bloques_sec(A11, B12);
	aux2 = bloques_sec(A12, B22);	
	vii C2 = suma(aux1, aux2);

	aux1 = bloques_sec(A21, B11);
	aux2 = bloques_sec(A22, B21);	
	vii C3 = suma(aux1, aux2);

	aux1 = bloques_sec(A22, B12);
	aux2 = bloques_sec(A22, B22);	
	vii C4 = suma(aux1, aux2);


	vii C(n*2, vector<int>(n*2));
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			C[i][j] = C1[i][j];
			C[i][j+n] = C2[i][j];
			C[i+n][j] = C3[i][j];
			C[i+n][j+n] = C4[i][j];
		}
	}

	return C;
}

vii strassen_sec(vii A, vii B){
	// limite recursion
	if(A.size() <= 2 << 5) return mat_amigable_sec(A, B);

	int n = A.size()/2;

	// Submatrices de A y B
	vii A11(n, vector<int>(n)), A12(n, vector<int>(n));
	vii A21(n, vector<int>(n)), A22(n, vector<int>(n));
	vii B11(n, vector<int>(n)), B12(n, vector<int>(n));
	vii B21(n, vector<int>(n)), B22(n, vector<int>(n));

	// Particionar las matrices A y B en 4 submatrices cada una
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			A11[i][j] = A[i][j];
			A12[i][j] = A[i][j+n];
			A21[i][j] = A[i+n][j];
			A22[i][j] = A[i+n][j+n];
			B11[i][j] = B[i][j];
			B12[i][j] = B[i][j+n];
			B21[i][j] = B[i+n][j];
			B22[i][j] = B[i+n][j+n];
		}
	}

	vii M1 = strassen_sec( suma(A11, A22), suma(B11, B22) );
	vii M2 = strassen_sec( suma(A21, A22), B11 );
	vii M3 = strassen_sec( A11, resta(B12, B22) );
	vii M4 = strassen_sec( A22, resta(B21, B11) );
	vii M5 = strassen_sec( suma(A11, A12), B22 );
	vii M6 = strassen_sec( resta(A21, A11), suma(B11, B12) );
	vii M7 = strassen_sec( resta(A12, A22), suma(B21, B22) );

	vii C(n*2, vector<int>(n*2));
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
			C[i][j+n] = M3[i][j] + M5[i][j];
			C[i+n][j] = M2[i][j] + M4[i][j];
			C[i+n][j+n] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
		}
	}

	return C;
}


// 4. 3a y strassen paralelo
vii sumap(vii &M1, vii &M2){
    int n = M1.size();
    vii M(n, vector<int>(n));

    #pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				M[i][j] = M1[i][j] + M2[i][j];
			}
		}

    return M;
}

vii restap(vii &M1, vii &M2){
    int n = M1.size();
    vii M(n, vector<int>(n));

    #pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				M[i][j] = M1[i][j] - M2[i][j];
			}
		}

    return M;
}

vii bloques_par(vii A, vii B){
	if(A.size() <= 2 << 5) return mat_amigable_sec(A, B);

	int n = A.size()/2;

	vii A11(n, vector<int>(n)), A12(n, vector<int>(n));
	vii A21(n, vector<int>(n)), A22(n, vector<int>(n));
	vii B11(n, vector<int>(n)), B12(n, vector<int>(n));
	vii B21(n, vector<int>(n)), B22(n, vector<int>(n));
	
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				A11[i][j] = A[i][j];
				A12[i][j] = A[i][j+n];
				A21[i][j] = A[i+n][j];
				A22[i][j] = A[i+n][j+n];
				B11[i][j] = B[i][j];
				B12[i][j] = B[i][j+n];
				B21[i][j] = B[i+n][j];
				B22[i][j] = B[i+n][j+n];
			}
		}

	vii aux1, aux2, C1, C2, C3, C4;

	#pragma omp parallel sections 
	{
		#pragma omp section
		{
			aux1 = bloques_par(A11, B11);
			aux2 = bloques_par(A12, B21);
			C1 = sumap(aux1, aux2);
		}
		#pragma omp section
		{
			aux1 = bloques_par(A11, B12);
			aux2 = bloques_par(A12, B22);    
			C2 = sumap(aux1, aux2);
		}
		#pragma omp section
		{
			aux1 = bloques_par(A21, B11);
			aux2 = bloques_par(A22, B21);    
			C3 = sumap(aux1, aux2);
		}
		#pragma omp section
		{
			aux1 = bloques_par(A22, B12);
			aux2 = bloques_par(A22, B22);    
			C4 = sumap(aux1, aux2);
		}
	}

	vii C(n*2, vector<int>(n*2));
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				C[i][j] = C1[i][j];
				C[i][j+n] = C2[i][j];
				C[i+n][j] = C3[i][j];
				C[i+n][j+n] = C4[i][j];
			}
		}

	return C;
}

vii strassen_par(vii A, vii B){
	// limite recursion
	if(A.size() <= 2 << 5) return mat_amigable_sec(A, B);

	int n = A.size()/2;

	// Submatrices de A y B
	vii A11(n, vector<int>(n)), A12(n, vector<int>(n));
	vii A21(n, vector<int>(n)), A22(n, vector<int>(n));
	vii B11(n, vector<int>(n)), B12(n, vector<int>(n));
	vii B21(n, vector<int>(n)), B22(n, vector<int>(n));

	// Particionar las matrices A y B en 4 submatrices cada una
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				A11[i][j] = A[i][j];
				A12[i][j] = A[i][j+n];
				A21[i][j] = A[i+n][j];
				A22[i][j] = A[i+n][j+n];
				B11[i][j] = B[i][j];
				B12[i][j] = B[i][j+n];
				B21[i][j] = B[i+n][j];
				B22[i][j] = B[i+n][j+n];
			}
		}

	vii M1, M2, M3, M4, M5, M6, M7;
	#pragma omp parallel sections
    {
		#pragma omp section
		M1 = strassen_par( sumap(A11, A22), sumap(B11, B22) );
		#pragma omp section
		M2 = strassen_par( sumap(A21, A22), B11 );
		#pragma omp section
		M3 = strassen_par( A11, restap(B12, B22) );
		#pragma omp section
		M4 = strassen_par( A22, restap(B21, B11) );
		#pragma omp section
		M5 = strassen_par( sumap(A11, A12), B22 );
		#pragma omp section
		M6 = strassen_par( restap(A21, A11), sumap(B11, B12) );
		#pragma omp section
		M7 = strassen_par( restap(A12, A22), sumap(B21, B22) );
	}

	vii C(n*2, vector<int>(n*2));
	#pragma omp parallel for
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				C[i][j] = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
				C[i][j+n] = M3[i][j] + M5[i][j];
				C[i+n][j] = M2[i][j] + M4[i][j];
				C[i+n][j+n] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
			}
		}

	return C;
}


int main(int argc, char *argv[]){
	minstd_rand rng;
	rng.seed(time(NULL));

	int n = 2 << 8, printMat = 0, alg = 0, flag = 0, soloResultados = 0;
	for(int i=0; i<argc; i++){
		if( !strcmp(argv[i], "-n" ) ) n = 2 << (atoi(argv[i+1]) - 1);
		if( !strcmp(argv[i], "-p" ) ) printMat = 1;
		if( !strcmp(argv[i], "-r" ) ) soloResultados = 1;
		if( !strcmp(argv[i], "-a" ) ){
			alg = atoi(argv[i+1]);
			flag = 1;
		}
	}

	// Creacion de A y B
	vector<vector<int>> A(n, vector<int>(n)), B(n, vector<int>(n));
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			A[i][j] = rng()%10;
			B[j][i] = rng()%10;
		}
	}
	
	if(printMat){
		imprimir(A, "A");
		imprimir(B, "B");
	}

	while(1){
		vii C;
		auto start = high_resolution_clock::now();

		if(alg == 0) C = mat_trad_sec(A, B);
		else if(alg == 1) C = mat_trad_par(A, B);
		else if(alg == 2) C = mat_amigable_sec(A, B);
		else if(alg == 3) C = mat_amigable_par(A, B);
		else if(alg == 4) C = strassen_sec(A, B);
		else if(alg == 5) C = strassen_par(A, B);
		else if(alg == 6) C = bloques_sec(A, B);
		else if(alg == 7) C = bloques_par(A, B);
		else break;

		auto finish = high_resolution_clock::now();
		auto d = duration_cast<microseconds> (finish - start).count();
		
		if(soloResultados) cout << d/1000.0 << endl;
		else{
			if(alg == 0)      cout << "Trad sec     "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 1) cout << "Trad par     "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 2) cout << "Amig sec     "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 3) cout << "Amig par     "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 4) cout << "Strassen sec "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 5) cout << "Strassen par "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 6) cout << "bloques sec  "<< d/1000.0 << " [ms]" << endl;
			else if(alg == 7) cout << "bloques par  "<< d/1000.0 << " [ms]" << endl;
		}

		if(printMat) imprimir(C, "C");
		if(flag) break;
		alg++;
	}

	return 0;
}