#include <iostream>
#include <vector>

int left(int i) {
    return 2 * i;
}

int right(int i) {
    return 2 * i + 1;
}

int parent(int i) {
    return i / 2;
}

void max_heapify(std::vector<int>& a, int heap_size, int i) {

    //Completar con el método max heapify

}

void build_max_heap(std::vector<int>& a) {
    int heap_size = a.size();

    for (int i = heap_size / 2; i >= 1; --i) {
        max_heapify(a, heap_size, i);
    }
}

int main() {
    // hay un -1 al inicio del arreglo, ya que se cuenta desde el 1
    std::vector<int> a = {-1, 0, 5, 20, 6, 12, 65, 1, 4, 9, 3, 89, 22, 25, 28, 10};
    build_max_heap(a);
    std::cout << "Heap:";
    for (size_t i = 1; i < a.size(); ++i) {
        std::cout << " " << a[i];
    }
    std::cout << std::endl;

    return 0;
}
