#include "No.h"
#include "Aresta.h"
#include <cstdlib> 

No** No::getVizinhos(const No** nos, int numNos, int& numVizinhos) {
    No** vizinhos = (No**)malloc(numNos * sizeof(No*));
    numVizinhos = 0;

    for (int i = 0; i < arestas.size(); ++i) {
        Aresta* aresta = arestas[i];
        for (int j = 0; j < numNos; ++j) {
            const No* no = nos[j];
            if (no->id == aresta->id_no_alvo) {
                vizinhos[numVizinhos++] == no; 
                break;
            }
        }
    }

    vizinhos = (No**)realloc(vizinhos, numVizinhos * sizeof(No*));
    return vizinhos;
}