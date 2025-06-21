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
    while (getline(arquivo, linha))
    {
        // lê a linha do arquivo e verifica se é ponderado e direcionado.
        if (linha_num == 0)
        {
            bool direcionado;
            bool ponderado_aresta;
            bool ponderado_vertice;
            stringstream ss(linha);
            ss >> direcionado >> ponderado_aresta >> ponderado_vertice;
            grafo->in_direcionado = direcionado;
            grafo->in_ponderado_aresta = ponderado_aresta;
            grafo->in_ponderado_vertice = ponderado_vertice;
            linha_num++;
        }
        // lê a linha do arquivo e verifica o grau.
        else if (linha_num == 1)
        {
            int ordem;
            stringstream ss(linha);
            ss >> ordem;
            grafo->ordem = ordem;
            linha_num++;
        }
        }

    arquivo.close();
    Gerenciador::comandos(grafo);

    return 0;
}