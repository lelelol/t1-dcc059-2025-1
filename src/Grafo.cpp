#include "Grafo.h"
#include <algorithm>
#include <limits>
Grafo::Grafo()
{
}

Grafo::~Grafo()
{
}
void Grafo::imprimirListaAdjacencias()
{
    if (this->lista_adj.empty())
    {
        cout << "O grafo esta vazio." << endl;
        return;
    }

    for (No *no_origem : this->lista_adj)
    {
        cout << no_origem->id << " -> ";

        for (Aresta *aresta : this->arestas)
        {

            if (aresta->id_no_origem == no_origem->id)
            {
                cout << aresta->id_no_alvo;
                if (this->in_ponderado_aresta)
                {
                    cout << "(" << aresta->peso << ")";
                }
                cout << " ";
            }
            else if (!this->in_direcionado && aresta->id_no_alvo == no_origem->id)
            {
                cout << aresta->id_no_origem;
                if (this->in_ponderado_aresta)
                {
                    cout << "(" << aresta->peso << ")";
                }
                cout << " ";
            }
        }
        cout << endl;
    }
}
No *Grafo::getNoPorId(char id)
{
    for (No *no : this->lista_adj)
    {
        if (no->id == id)
        {
            return no;
        }
    }
    return nullptr;
}

vector<char> Grafo::fecho_transitivo_direto(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    vector<char> fecho;
    map<char, bool> visitados;

    if (!this->in_direcionado)
    {
        cout << "Aviso: Fecho transitivo indireto eh um conceito para grafos direcionados." << endl;
        return fecho;
    }

    if (getNoPorId(id_no) == nullptr)
    {
        cout << "Erro: O no '" << id_no << "' nao existe no grafo." << endl;
        return fecho;
    }

    dfs_inverso(id_no, visitados);

    for (auto const &par : visitados)
    {
        if (par.second && par.first != id_no)
        {
            fecho.push_back(par.first);
        }
    }

    return fecho;
}

void Grafo::dfs_inverso(char id_atual, map<char, bool> &visitados)
{
    visitados[id_atual] = true;

    for (Aresta *aresta : this->arestas)
    {
        if (aresta->id_no_alvo == id_atual)
        {
            char id_pai = aresta->id_no_origem;

            if (visitados.find(id_pai) == visitados.end())
            {
                dfs_inverso(id_pai, visitados);
            }
        }
    }
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{

    if (!this->in_ponderado_aresta)
    {
        cout << "O grafo não é ponderado nas arestas." << endl;
        return nullptr;
    }
    if (ids_nos.empty())
    {
        cout << "Subconjunto vazio." << endl;
        return nullptr;
    }

    vector<Aresta *> arestasInduzidas;
    for (Aresta *aresta : this->arestas)
    {
        bool origem_in_X = (find(ids_nos.begin(), ids_nos.end(), aresta->id_no_origem) != ids_nos.end());
        bool alvo_in_X = (find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end());

        if (origem_in_X && alvo_in_X)
        {
            arestasInduzidas.push_back(aresta);
        }
    }

    Grafo *agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;

    map<char, bool> inMST;
    for (char id : ids_nos)
    {
        inMST[id] = false;
        agm->lista_adj.push_back(this->getNoPorId(id));
    }

    char no_inicial = ids_nos[0];
    inMST[no_inicial] = true;
    int arestas_na_agm = 0;

    while (arestas_na_agm < ids_nos.size() - 1)
    {
        Aresta *melhorAresta = nullptr;
        int pesoMinimo = numeric_limits<int>::max();

        for (Aresta *aresta : arestasInduzidas)
        {
            bool ladoA_in = inMST[aresta->id_no_origem];
            bool ladoB_in = inMST[aresta->id_no_alvo];

            if (ladoA_in != ladoB_in)
            {
                if (aresta->peso < pesoMinimo)
                {
                    pesoMinimo = aresta->peso;
                    melhorAresta = aresta;
                }
            }
        }

        if (melhorAresta == nullptr)
        {
            cout << "O subconjunto dado não é conexo, arvore nao gerada." << endl;
            break;
        }

        agm->arestas.push_back(melhorAresta);

        inMST[melhorAresta->id_no_origem] = true;
        inMST[melhorAresta->id_no_alvo] = true;
        arestas_na_agm++;
    }

    return agm;
}

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    cout << "Metodo nao implementado" << endl;
    return nullptr;
}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}
