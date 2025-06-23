#include "Aresta.h"

Aresta::Aresta()
{
}
Aresta::~Aresta()
{
    for (Aresta *aresta : arestas)
    {
        delete aresta;
    }
}