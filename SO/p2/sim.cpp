#include <vector>
#include <queue>
#include <random>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
minstd_rand rng;

int N = 20;       // procesos maximos, rango [a, b]
int M = 3;        // cpus
int a = 1, b = 5; // [a, b]

vector<vector<queue<int>>> runqueue(2); // 2 runqueues, rango (0, colas-1)
vector<int> tiempo;
int colas = 10;
int N2 = 10; // "pid" actual maximo

int activa = 0, expirada = 1, esperando = 0; 
int quantum = 1;

vector<vector<mutex>> locks(2);
mutex mE, mG, mPrint;
condition_variable cvE, cvG;

void imprimir(){
    cout << endl << "Cola" << endl;
    cout << "Activa" << " Proceso(Tiempo)" << endl;
    for (int i = 0; i < colas; i++){
        int aux = runqueue[activa][i].size();
        !aux ? cout << " " << i << "     -"
             : cout << " " << i << "    ";

        while (aux--){
            int aux2 = runqueue[activa][i].front();
            cout << " " << aux2 << "(" << tiempo[aux2] << ")";
            runqueue[activa][i].push(aux2);
            runqueue[activa][i].pop();
        }
        cout << endl;
    }
    cout << endl;
}

int defPrioridad(int prioridadActual, int nuevoTiempo){
    return nuevoTiempo / quantum > 3 ? min(9, prioridadActual + 1) :  max(0, prioridadActual - 1);
}

int hebra_E(int h){
    int cola = 0;
    while (true){
        locks[activa][cola].lock();
        // quedan procesos en la cola de la runqueue activa
        if (!runqueue[activa][cola].empty()){
            int proceso = runqueue[activa][cola].front();
            runqueue[activa][cola].pop();
            locks[activa][cola].unlock();

			mPrint.lock();
            cout << "CPU " << h << ", Proceso " << proceso << " ejecutandose" << endl;
			mPrint.unlock();
			
            int tiempoEjecucion = min(tiempo[proceso], quantum); // planificador cuenta con un periodo de tiempo (similar a RR) 
            sleep(tiempoEjecucion);

            // cuando un proceso no termina en el tiempo establecido, es agregado a la runqueue expirada con una prioridad inferior 
            // y con un nuevo tiempo de ejecución
            if (tiempo[proceso] > quantum){ 
				tiempo[proceso] = a + rng() % b; // si un proceso tiene tiempo de ejecución inferior no significa que ese tiempo "sobrante" se acumule. 
				int nuevaPrioridad = defPrioridad(cola, tiempo[proceso]);
				
            	locks[expirada][nuevaPrioridad].lock();
                runqueue[expirada][nuevaPrioridad].push(proceso);
            	locks[expirada][nuevaPrioridad].unlock();
            }
            else{
				mPrint.lock();
                cout << "CPU " << h << ", Proceso " << proceso << " finalizado" << endl;
				mPrint.unlock();
			}

        }else{
            locks[activa][cola].unlock();

            if (cola + 1 == colas){
                unique_lock<mutex> m(mE);

                esperando++;
                if (esperando == M) // todas las CPUs con colas vacias esperando
                    cvG.notify_one(); // se activa la hebra G
                cvE.wait(m);
            }

            cola = (cola + 1) % colas; // arreglo circular
        }
    }

    return 0;
}

int hebra_G()
{
    while (true){
        unique_lock<mutex> m(mG);
        cvG.wait(m);

        unique_lock<mutex> m2(mE);
        esperando = 0;
        swap(activa, expirada); // 1 0 -> 0 1, para los procesos de la runqueue expirada

        int procesosActuales = 0; // procesos en la runqueue expirada/activa
        for (int i = 0; i < colas; i++)
            procesosActuales += runqueue[activa][i].size();

        int nuevosProc = N - procesosActuales;

        for (int i = 0; i < nuevosProc; i++){
            runqueue[activa][colas / 2].push(N2 + i);
            tiempo.push_back(a + rng() % b);
        }
        N2 += nuevosProc;

		imprimir();

        cvE.notify_all();
    }

    return 0;
}

int main()
{
    rng.seed(time(NULL));
    cout << "N M a b" << endl;
    cin >> N >> M >> a >> b;

    runqueue[0] = vector<queue<int>>(colas);
    runqueue[1] = vector<queue<int>>(colas);
    locks[0] = vector<mutex>(colas);
    locks[1] = vector<mutex>(colas);

    for (int i = 0; i < N; i++){
        runqueue[activa][colas / 2].push(i);
        tiempo.push_back(a + rng() % b);
    }

	imprimir();

    vector<thread> t;
    for (int i = 0; i < M; i++){
        thread aux(hebra_E, i);
        t.push_back(move(aux));
    }
    thread aux(hebra_G);
    t.push_back(move(aux));

    for (int i = 0; i < t.size(); i++)
        t[i].join();

    return 0;
}
