#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Gerenciador.h"

using namespace std;

int main(int argc, char *argv[])
{
    Grafo *grafo = new Grafo();

    if (argc != 2)
    {
        cout << "arquivo nao encontrado" << endl;
        return 1;
    }

    string nomeArquivo = argv[1];

    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "erro na leitura do arquivo" << endl;
        return 1;
    }

    string linha;
    int linha_num = 0;
    bool direcionado;
    bool ponderado_aresta;
    bool ponderado_vertice;
    int ordem;
    int aux_ordem = 0;
    while (getline(arquivo, linha))
    {
        // lê a linha do arquivo e verifica se é ponderado e direcionado.
        if (linha_num == 0)
        {
            stringstream ss(linha);
            ss >> direcionado >> ponderado_aresta >> ponderado_vertice;
            grafo->in_direcionado = direcionado;
            grafo->in_ponderado_aresta = ponderado_aresta;
            grafo->in_ponderado_vertice = ponderado_vertice;
        }
        // lê a linha do arquivo e verifica o grau.
        else if (linha_num == 1)
        {
            stringstream ss(linha);
            ss >> ordem;
            grafo->ordem = ordem;
        }
        else if (ponderado_vertice && linha_num <= ordem + 2)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            char id_no;
            int peso;
            ss >> id_no >> peso;
            No *no = new No();
            no->id = id_no;
            no->peso = peso;
            grafo->lista_adj.push_back(no);
        }
        else if (!ponderado_vertice && linha_num <= ordem + 2)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            char id_no;
            ss >> id_no;
            No *no = new No();
            no->id = id_no;
            no->peso = 0;
            grafo->lista_adj.push_back(no);
        }
        else if (ponderado_aresta && linha_num < ordem + 2)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            char id_a;
            char id_b;
            int peso;
            ss >> id_a >> id_b >> peso;
            Aresta *aresta = new Aresta();
            aresta->id_no_origem = id_a;
            aresta->id_no_alvo = id_b;
            aresta->peso = peso;
            grafo->arestas.push_back(aresta);
        }
        else if (!ponderado_aresta && linha_num < ordem + 2)
        {
            getline(arquivo, linha);
            stringstream ss(linha);
            char id_a;
            char id_b;
            int peso;
            ss >> id_a >> id_b;
            Aresta *aresta = new Aresta();
            aresta->id_no_origem = id_a;
            aresta->id_no_alvo = id_b;
            aresta->peso = 0;
            grafo->arestas.push_back(aresta);
        }
        linha_num++;
    }

    arquivo.close();
    Gerenciador::comandos(grafo);

    return 0;
}