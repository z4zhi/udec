#include <vector>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <set>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <signal.h>

using namespace std;
int flagSIGINT = 0;

void signal_handler(int sig);
void ejecutarComandos(vector<vector<string>> &com);
vector<string> medirRecursos();

int main(){
	signal(SIGINT, signal_handler);

	string nombreLog;
	int flagRecursos = 0;
	vector<vector<string>> statsRecursos;
	ofstream myfile;

	while(true){	
		cout << "mini-shell:$ ";
		string linea;
		getline(cin, linea);

		if(flagSIGINT){
			if(linea == "y" || linea == "Y") exit(0);
			flagSIGINT = 0;
			continue;
		}
		if(linea.empty()) continue;

		// procesar linea
		string buffer;
		stringstream ss(linea);
		vector<vector<string>> com(1);
		while(ss >> buffer){
			if(buffer == "|") com.push_back( vector<string>() );
			else com.back().push_back(buffer);
		}

		// comandos propios
		if(com[0][0] == "exit") exit(0);

		else if(com[0].size() == 3 && com[0][0] == "usorecursos" && com[0][1] == "start"){
			if(flagRecursos) cout << "Comando ejecutandose" << endl;
			else{
				flagRecursos = 1;
				nombreLog = com[0][2];
				myfile.open(nombreLog);
				
				string c = "comando";
				myfile << c.append(30 - c.size(), ' ');
				vector<string> vaux = {"tuser", "tsys", "maxrss"};
				for(string stat: vaux) myfile << stat.append(15 - stat.size(), ' ');
				myfile << "\n";

				myfile.close();
			}
			continue;

		}else if(com[0].size() == 2 && com[0][0] == "usorecursos" && com[0][1] == "stop"){
			if(!flagRecursos) cout << "Comando no iniciado" << endl;
			else flagRecursos = 0;
			continue;

		}else if(com[0].size() == 3 && com[0][0] == "sig"){
			try{
				int sig = stoi(com[0][1]), pid = stoi(com[0][2]);
				if(sig == 1) kill(pid, SIGUSR1);
				else if(sig == 2) kill(pid, SIGUSR2);
			}catch(const invalid_argument &ia){
				cout << "Argumentos incorrectos" << endl;
			}
			continue;
		}

		if(fork() == 0){
			ejecutarComandos(com);
			if(flagRecursos){
				myfile.open(nombreLog, fstream::app);

				if(30 - (int)linea.size() > 5) myfile << linea.append(30 - linea.size(), ' ');
				else myfile << linea.append(5, ' ');
				vector<string> vaux = medirRecursos();
				for(string stat: vaux) myfile << stat.append(15 - stat.size(), ' ');
				myfile << "\n";

				myfile.close();
			}
			exit(0);

		}else wait(NULL);
	}

	return 0;
}

void ejecutarComandos(vector<vector<string>> &com){
	// crear pipes
	int nPipes = com.size()-1;
	int pipes[nPipes*2];
	for(int i=0; i<nPipes; i++) pipe(pipes + 2*i);

	// ejecutar comandos
	set<pid_t> pids;
	for(int i=0; i<com.size(); i++){
		// procesar argmentos
		char *myargs[ com[i].size()+1 ];
		for(int j=0; j<com[i].size(); j++) myargs[j] = strdup( com[i][j].c_str() );
		myargs[ com[i].size() ] = NULL;

		// crear procesos
		pid_t pid = fork();
		if(pid == 0){
			if(i != 0) dup2(pipes[ (i-1)*2 ], 0);				// si no es el primero, lee el anterior
			if(i != com.size()-1) dup2(pipes[i*2 + 1], 1);		// si no es el ultimo, escribe en el siguiente

			for(int j=0; j<nPipes; j++){	// cerrar pipes
				close(pipes[j]);
				close(pipes[j+1]);
			}

			execvp(myargs[0], myargs);	// si retorna es porque ocurrio una exepcion
			cout << "Comando no encontrado" << endl;
			exit(0);
		}else pids.insert(pid);
	}
	
	for(int i=0; i<nPipes; i++){	// cerrar pipes
		close(pipes[i]);
		close(pipes[i+1]);
	}

	// esperar hijos
	int error = 0;
	for(int i=0; i<com.size(); i++){
		pids.erase( wait(&error) ); 

		if(error){	// si uno falla, matar a los vivos y esperarlos
			for(pid_t p: pids) kill(p, SIGKILL);
			while( wait(NULL) != -1 );
			break;
		}
	}
}

void signal_handler(int sig){
	if(sig == SIGINT){
		flagSIGINT = 1;
		cout << endl << "¿Desea salir de la shell? (Y/N)" << endl;
	}
}

vector<string> medirRecursos(){
	struct rusage r_usage;
	getrusage(RUSAGE_CHILDREN, &r_usage);

	string tuser;
	if(r_usage.ru_utime.tv_sec >= 1 ) tuser = to_string(r_usage.ru_utime.tv_sec) + "s";
	else{
		string aux = to_string( (r_usage.ru_utime.tv_usec)/1000.0 );
		aux.resize(5);
		tuser = aux + "ms";
	}

	string tsys;
	if(r_usage.ru_stime.tv_sec >= 1 ) tsys = to_string(r_usage.ru_stime.tv_sec) + "s";
	else{
		string aux = to_string( (r_usage.ru_stime.tv_usec)/1000.0 );
		aux.resize(5);
		tsys = aux + "ms";
	}

	string maxrss = to_string(r_usage.ru_maxrss) + "kB";

	return vector<string>{tuser, tsys, maxrss};
}
