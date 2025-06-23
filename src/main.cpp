#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Grafo.h"
#include "Gerenciador.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Erro: Forneca o nome do arquivo como argumento." << endl;
        cout << "Uso: ./programa <nome_do_arquivo.txt>" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro na leitura do arquivo '" << nomeArquivo << "'" << endl;
        return 1;
    }

    Grafo *grafo = new Grafo();
    string linha;

    getline(arquivo, linha);
    stringstream(linha) >> grafo->in_direcionado >> grafo->in_ponderado_aresta >> grafo->in_ponderado_vertice;

    getline(arquivo, linha);
    stringstream(linha) >> grafo->ordem;

    for (int i = 0; i < grafo->ordem; ++i)
    {
        if (getline(arquivo, linha))
        {
            stringstream ss(linha);
            No *no = new No();
            ss >> no->id;
            if (grafo->in_ponderado_vertice)
            {
                ss >> no->peso;
            }
            else
            {
                no->peso = 0;
            }
            grafo->lista_adj.push_back(no);
        }
    }

    while (getline(arquivo, linha))
    {
        if (linha.empty())
            continue;

        stringstream ss(linha);
        Aresta *aresta = new Aresta();
        ss >> aresta->id_no_origem >> aresta->id_no_alvo;
        if (grafo->in_ponderado_aresta)
        {
            ss >> aresta->peso;
        }
        else
        {
            aresta->peso = 0;
        }
        grafo->arestas.push_back(aresta);
    }

    arquivo.close();

    Gerenciador::comandos(grafo);

    return 0;
}