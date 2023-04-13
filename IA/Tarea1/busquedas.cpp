#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>

using namespace std;

/**
 * @brief struct para un nodo
 * Contiene el nombre del nodo, su heuristica y 2 vectores:
 * Uno que guarda sus vecinos/aristas y el otro, el costo de sus aristas.
 * Ademas contiene un entero que cuenta las veces que fue visitado y 2 funciones:
 * Uno para desmarcar los nodos visitados "reiniciar" los nodos y otro para borrar los nodos.
 */
struct nodo
{
    string nombre;
    int h;            // heuristica
    vector<nodo *> e; // vector que contiene los vecinos/aristas del nodo
    vector<int> c;    // costos
    int visited = 0;  // si el nodo ha sido visitado
    void clearVisited()
    {
        visited = 0;
        if (!e.empty())
            for (auto *arista : e)
                arista->clearVisited();
    }
    ~nodo()
    {
        if (!e.empty())
            for (auto *arista : e)
                arista->~nodo(), delete arista;
    }
};

string init, goal;
int optimo;

// leer el problema (grafo) desde un archivo
void read(string archivo, nodo *v)
{
    ifstream file(archivo);
    string line;

    // nodo inicial
    getline(file, line);
    init = v->nombre = line.substr(6); // Init: <nodo_inicial>

    // nodo goal
    getline(file, line);
    goal = line.substr(6); // Goal: <nodo_objetivo>

    // leer nodos y aristas
    vector<nodo *> tmp; // almacenar los nodos que se van leyendo
    tmp.push_back(v);
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        size_t coma = line.find(","); // buscar si hay una coma (arista)
        if (coma == string::npos)     // si no hay coma, es heuristica
        {
            size_t space = line.find(" ");          // buscar espacio en blaco
            string nombre = line.substr(0, space);  // inicio hasta espacio
            int heu = stoi(line.substr(space + 1)); // heuristica es el numero despues del espacio

            if (nombre == init)
                v->h = heu;
            else
            {
                // crear el nodo y almacenarlo en el vector
                nodo *node = new nodo;
                node->nombre = nombre, node->h = heu;
                tmp.push_back(node);
            }
        }
        else // con coma, arista
        {
            size_t space = line.find(" ");
            string nombre1 = line.substr(0, space - 1);
            line = line.substr(space + 1);

            space = line.find(" ");
            string nombre2 = line.substr(0, space - 1);
            line = line.substr(space + 1);

            int costo = stoi(line.substr(0, space));

            // buscar posicion del nodo1 y nodo2 en el vector tmp
            int pos1 = -1, pos2 = -1;
            for (int i = 0; i < tmp.size(); i++)
            {
                if (tmp[i]->nombre == nombre1)
                    pos1 = i;
                if (tmp[i]->nombre == nombre2)
                    pos2 = i;
                if (pos1 != -1 && pos2 != -1)
                    break;
            }
            tmp[pos1]->e.push_back(tmp[pos2]), tmp[pos1]->c.push_back(costo);
            //  tmp[pos2]->e.push_back(tmp[pos1]), tmp[pos2]->c.push_back(costo); // no dirigido
        }
    }
    v = tmp[0];
}

/**
 * @brief Funcion recursiva para recorrer todos los nodos e imprimirlos
 * Imprime y luego recorre los vecinos del nodo, usando la funcion recursiva
 * @param actual nodo que sera visitado
 */
void printAll(nodo *actual)
{
    if (!actual->visited)
    {
        actual->visited = 1;
        cout << "Nodo: " << actual->nombre << " " << actual->h << endl;
        cout << " Aristas: ";
        for (int i = 0; i < actual->e.size(); i++)
            cout << actual->e[i]->nombre << " (costo: " << actual->c[i] << ") ";
        cout << endl;
    }
    if (!actual->e.empty())
        for (auto *arista : actual->e)
            printAll(arista);
}

/**
 * @brief Funcion para imprimir los resultados.
 * El camino encontrado y su costo (y si es la solución óptima, que ud. puede calcular a mano)
 * Cantidad de nodos expandidos (veces por nodo y en total)
 * @param camino camino almacenado en un string
 * @param expandidos nodos visitados/expandidos y numero de veces
 * @param costo almacenado en un int
 */
void print(const vector<string> &camino, const vector<pair<string, int>> &expandidos, int costo)
{
    for (int i = 0; i < camino.size(); i++)
    {
        cout << camino[i];
        if (i < camino.size() - 1)
            cout << " -> ";
    }
    cout << endl
         << "Costo: " << costo;
    costo != optimo ? cout << ", No Optimo" : cout << ", Optimo";
    cout << endl;

    int total = 0;
    for (int i = 0; i < expandidos.size(); i++)
    {
        total += expandidos[i].second;
        cout << expandidos[i].first << ": " << expandidos[i].second << endl;
    }
    cout << "Total: " << total << endl;
}

/**
 * @brief Busqueda en profundidad (escogiendo un sucesor al azar)
 * El algoritmo funciona con un bucle while infinito, donde:
 * 1. Se comprueba si es el nodo goal o si no tiene vecinos. Si es verdadero, termina el bucle.
 * 2. Caso contrario, se almacena el nodo en el vector camino y se marca el nodo como visitado.
 * 3. Se escoge un vecino al azar y se actualiza el nodo. En la siguiente iteracion, se ocupa el nuevo nodo.
 * @param actual
 */
void p(nodo *actual)
{
    vector<string> camino;                // almacenar el camino
    vector<pair<string, int>> expandidos; // nodos y cuantas veces fue expandido
    int costo = 0;                        // costo total del camino
    mt19937 gen(time(NULL));

    while (actual->nombre != goal || !actual->e.empty())
    {
        // revisar si se ha expandido el nodo y aumentar el contador , si no agregarlo al vector
        int idx = -1;
        for (int i = 0; i < expandidos.size(); i++)
            if (expandidos[i].first == actual->nombre)
            {
                idx = i;
                break;
            }
        if (idx != -1)
            expandidos[idx].second++;
        else
            expandidos.push_back(pair(actual->nombre, 1));

        camino.push_back(actual->nombre), actual->visited++;

        // escoger un sucesor al azar
        uniform_int_distribution<> dis(0, actual->e.size() - 1);
        int random = dis(gen);

        costo += actual->c[random], actual = actual->e[random];
    }
    camino.push_back(actual->nombre), actual->visited++;
    expandidos.push_back(pair(actual->nombre, 1));
    print(camino, expandidos, costo);
}

/**
 * @brief Busqueda greedy
 * El algoritmo funciona con un bucle while infinito, donde:
 * 1. Se comprueba si es el nodo goal o si no tiene vecinos. Si es verdadero, termina el bucle.
 * 2. Caso contrario, se almacena el nodo en el vector camino y se marca el nodo como visitado.
 * 3. Luego, se crean 2 enteros, uno que almacena la heuristica menor y el otro la posicion del vecino que tiene esa heuristica.
 * 4. Al terminar el for, se actualiza el nodo con el minimo encontrado.
 * @param actual
 */
void g(nodo *actual)
{
    vector<string> camino;                // almacenar el camino
    vector<pair<string, int>> expandidos; // nodos y cuantas veces fue expandido
    int costo = 0;                        // costo total del camino

    while (actual->nombre != goal || !actual->e.empty())
    {
        // revisar si se ha expandido el nodo y aumentar el contador , si no agregarlo al vector
        int idx = -1;
        for (int i = 0; i < expandidos.size(); i++)
            if (expandidos[i].first == actual->nombre)
            {
                idx = i;
                break;
            }
        if (idx != -1)
            expandidos[idx].second++;
        else
            expandidos.push_back(pair(actual->nombre, 1));

        camino.push_back(actual->nombre), actual->visited++;

        int min = 99, posmin;
        for (int i = 0; i < actual->e.size(); i++) // buscar el min de los vecinos
            if (actual->e[i]->h < min)
                min = actual->e[i]->h, posmin = i;

        costo += actual->c[posmin], actual = actual->e[posmin];
    }
    camino.push_back(actual->nombre), actual->visited++;
    expandidos.push_back(pair(actual->nombre, 1));
    print(camino, expandidos, costo);
}

/**
 * @brief struct auxiliar para almacenar caminos.
 * El vector almacena los nombres de los nodos expandidos.
 * En costo se va acumulando el costo total.
 * sig es el siguiente nodo que sera visitado.
 * f se utiliza para A* y es el costo (acumulado actual) + la heuristica del vecino.
 */
struct caminos
{
    vector<string> camino;
    int costo = 0;
    nodo *sig;
    int f = 0;
};

/**
 * @brief Busqueda por costo uniforme
 * El algoritmo funciona con un bucle que se detiene cuando el costo para expandir al proximo nodo es
 * mayor o igual al costo minimo encontrado de camino hacia goal.
 * 1. Se comprueba que el costo guardado en min (costo al siguiente nodo a expandir) es menor al de mingoal.
 * 2. Si lo es, entra al bucle y se marca como visitado.
 * 3. Si el nodo actual es goal, es un camino hacia goal y se comprueba si es el camino minimo, actualizando mingoal y cgoal.
 * 4. Si no, entra a un for, en donde para cada vecino del nodo actual, se crean caminos nuevos con estos vecinos,
 * con la informacion anterior (del nodo actual) + la informacion nueva (actualizar costos, camino, nodo).
 * 5. Estos caminos nuevos se almacenan en path y se borra el camino actual
 * 6. Para todos los caminos en path, se busca el que tenga menor costo y,
 * se actualiza el min, n (indice que apunta al camino de path minimo) y el nodo que se visitara en la siguiente iteracion.
 * @param actual
 */
void cu(nodo *actual)
{
    vector<caminos> paths;                    // caminos
    vector<pair<string, int>> expandidos;     // nodos y cuantas veces fue expandido
    int n = 0;                                // indice
    int min = 90, mingoal = 99; // costo min del sig nodo y del camino hacia goal
    caminos cgoal;                            // camino que llega a goal
    caminos root;                             // camino inicial
    root.camino.push_back(actual->nombre), paths.push_back(root), cgoal.costo = mingoal;

    while (min < mingoal) // si el costo del proximo nodo a expandir es mayor al costo encontrado para goal, termina
    {
        actual->visited++;

        // revisar si se ha expandido el nodo y aumentar el contador , si no agregarlo al vector
        int idx = -1;
        for (int i = 0; i < expandidos.size(); i++)
            if (expandidos[i].first == actual->nombre)
            {
                idx = i;
                break;
            }
        if (idx != -1)
            expandidos[idx].second++;
        else
            expandidos.push_back(pair(actual->nombre, 1));

        // si el nodo actual es goal, se almacena en goals
        if (actual->nombre == goal)
        {
            // si tiene menor costo, es mas optimo
            if (mingoal > paths[n].costo)
                mingoal = paths[n].costo, cgoal = paths[n];
        }
        // si no es goal, expandir a vecinos
        else
        {
            // expandir nodo actual y crear nuevos posibles caminos, dependiendo de cuantos vecinos tenga
            for (int i = 0; i < actual->e.size(); i++)
            {
                caminos aux = paths[n]; // crear un nuevo camino, con la informacion anterior
                // se actualiza la informacion con la nueva
                aux.camino.push_back(actual->e[i]->nombre), aux.costo += actual->c[i], aux.sig = actual->e[i];
                paths.push_back(aux);
            }
        }
        // elimino al camino (ya fue visitado y no es necesario)
        paths.erase(paths.begin() + n);

        // busco entre los caminos nuevos posibles y los caminos anteriores el que tenga minimo costo
        min = 99;
        for (int i = 0; i < paths.size(); i++)
            if (paths[i].costo < min)
                min = paths[i].costo, n = i;
        actual = paths[n].sig;
    }
    print(cgoal.camino, expandidos, cgoal.costo);
}

/**
 * @brief A*
 * Similar a costo uniforme. La diferencia es que usa f para decir el proximo vecino a expandir.
 * El algoritmo funciona con un bucle que se detiene cuando
 * el costo para expandir al proximo nodo es mayor o igual al costo minimo encontrado de camino hacia goal.
 * 1. Se comprueba que el costo guardado en min (costo al siguiente nodo a expandir) es menor al de mingoal.
 * 2. Si lo es, entra al bucle y se marca como visitado.
 * 3. Si el nodo actual es goal, es un camino hacia goal y se comprueba si es el camino minimo, actualizando mingoal y cgoal.
 * 4. Si no, entra a un for, en donde para cada vecino del nodo actual, se crean caminos nuevos posibles,
 * con la informacion anterior (del nodo actual) + la informacion nueva (actualizar costos, camino, nodo).
 * Adicionalmente, esta f que guarda la suma del costo acumulado + la heuristica del vecino.
 * 5. Estos caminos nuevos posibles se almacenan en path y se borra el camino actual.
 * 6. Para todos los caminos en path, se busca el que tenga menor f y,
 * se actualiza el min, n (indice que apunta al camino de path minimo) y el nodo que se visitara en la siguiente iteracion.
 * @param actual
 */
void a(nodo *actual)
{
    vector<caminos> paths;                    // caminos
    vector<pair<string, int>> expandidos;     // nodos y cuantas veces fue expandido
    int n = 0;                                // indice
    int min = 90, mingoal = 99; // costo min del sig nodo y del camino hacia goal
    caminos cgoal;                            // camino que llega a goal
    caminos root;                             // camino inicial
    root.camino.push_back(actual->nombre), paths.push_back(root), cgoal.costo = mingoal;

    while (min < mingoal) // si el costo del proximo nodo a expandir es mayor al costo encontrado para goal, termina
    {
        actual->visited++;

        // revisar si se ha expandido el nodo y aumentar el contador , si no agregarlo al vector
        int idx = -1;
        for (int i = 0; i < expandidos.size(); i++)
            if (expandidos[i].first == actual->nombre)
            {
                idx = i;
                break;
            }

        if (idx != -1)
            expandidos[idx].second++;
        else
            expandidos.push_back(pair(actual->nombre, 1));

        // si el nodo actual es goal, se almacena en goals
        if (actual->nombre == goal)
        {
            // si tiene menor costo, es mas optimo
            if (mingoal > paths[n].costo)
                mingoal = paths[n].costo, cgoal = paths[n];
        }
        // si no es goal, expandir a vecinos
        else
        {
            // expandir nodo actual y crear nuevos posibles caminos, dependiendo de cuantos vecinos tenga
            for (int i = 0; i < actual->e.size(); i++)
            {
                caminos aux = paths[n]; // crear un nuevo camino, con la informacion anterior
                // se actualiza la informacion con la nueva
                aux.camino.push_back(actual->e[i]->nombre), aux.costo += actual->c[i], aux.sig = actual->e[i];
                aux.f = aux.costo + actual->e[i]->h;
                paths.push_back(aux);
            }
        }
        // elimino al camino (ya fue visitado y no es necesario)
        paths.erase(paths.begin() + n);

        // busco entre los caminos nuevos posibles y los caminos anteriores el que tenga minimo costo
        min = 99;
        for (int i = 0; i < paths.size(); i++)
            if (paths[i].f < min)
                min = paths[i].f, n = i;
        actual = paths[n].sig;
    }
    optimo = cgoal.costo;
    print(cgoal.camino, expandidos, cgoal.costo);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "Uso: " << argv[0] << " archivo.txt" << endl;
        return 1;
    }
    nodo *v = new nodo; // grafo, arbol
    read(argv[1], v);
    cout << "Se ha ingresado el archivo: " << argv[1] << ", con el siguiente grafo: " << endl
         << "Init: " << init << endl
         << "Goal: " << goal << endl;
    printAll(v);
    v->clearVisited();

    cout << endl
         << "A*" << endl;
    a(v);
    v->clearVisited();

    cout << endl
         << "Busqueda en profundidad" << endl;
    p(v);
    v->clearVisited();

    cout << endl
         << "Busqueda por costo uniforme" << endl;
    cu(v);
    v->clearVisited();

    cout << endl
         << "Busqueda greedy" << endl;
    g(v);
    v->~nodo(), delete v;
    return 0;
}