#ifndef NO_H
#define NO_H

#include <string>
#include <vector>
#include "Aresta.h"


using namespace std;
class No {
public:
    No();
    ~No();

    char id;
    int peso;
    vector<Aresta*> arestas;

    // FUNÇÕES IMPLEMENTADAS
    No** getVizinhos(const No** nos, int numNos, int& numVizinhos);
};



#endif //NO_H
