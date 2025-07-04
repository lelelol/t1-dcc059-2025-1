#include "Grafo.h"
#include <algorithm>
#include <limits>
#include <map>
#include <queue>
#include <vector>

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
    vector<char> fecho;
    map<char, bool> visitados;

    if (!getNoPorId(id_no))
    {
        cout << "Erro: O no '" << id_no << "' nao existe no grafo." << endl;
        return fecho;
    }

    vector<char> pilha;
    pilha.push_back(id_no);

    while (!pilha.empty())
    {
        char atual = pilha.back();
        pilha.pop_back();

        if (!visitados[atual])
        {
            visitados[atual] = true;
            if (atual != id_no)
                fecho.push_back(atual);

            for (Aresta *aresta : this->arestas)
            {
                if (aresta->id_no_origem == atual)
                {
                    if (!visitados[aresta->id_no_alvo])
                    {
                        pilha.push_back(aresta->id_no_alvo);
                    }
                }
            }
        }
    }

    return fecho;
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
    // Mapa para armazenar as distâncias mínimas do nó de origem para qualquer outro nó.
    map<char, int> distancias;

    // Mapa para armazenar o predecessor de cada nó no caminho mais curto.
    map<char, char> predecessores;

    // Fila de prioridade para selecionar o nó com a menor distância que ainda não foi visitado.
    // O par armazena a distância e o ID do nó. A fila é de prioridade mínima.
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

    // Inicialização das distâncias de todos os nós como "infinito", exceto o nó de partida.
    for (No *no : this->lista_adj)
    {
        distancias[no->id] = numeric_limits<int>::max();
        predecessores[no->id] = 0; // 0 representa nenhum predecessor
    }

    // A distância do nó de partida para ele mesmo é 0.
    distancias[id_no_a] = 0;
    pq.push({0, id_no_a});

    while (!pq.empty())
    {
        char u_id = pq.top().second;
        pq.pop();

        No *u_no = getNoPorId(u_id);
        if (!u_no)
            continue; // Se o nó não for encontrado, continue

        // Para cada aresta que sai do nó 'u'
        for (Aresta *aresta : this->arestas)
        {
            if (aresta->id_no_origem == u_id)
            {
                char v_id = aresta->id_no_alvo;
                int peso = this->in_ponderado_aresta ? aresta->peso : 1;

                // Relaxamento da aresta
                if (distancias[u_id] != numeric_limits<int>::max() && distancias[u_id] + peso < distancias[v_id])
                {
                    distancias[v_id] = distancias[u_id] + peso;
                    predecessores[v_id] = u_id;
                    pq.push({distancias[v_id], v_id});
                }
            }
            // Se o grafo não for direcionado, considerar a aresta no sentido inverso também
            else if (!this->in_direcionado && aresta->id_no_alvo == u_id)
            {
                char v_id = aresta->id_no_origem;
                int peso = this->in_ponderado_aresta ? aresta->peso : 1;

                if (distancias[u_id] != numeric_limits<int>::max() && distancias[u_id] + peso < distancias[v_id])
                {
                    distancias[v_id] = distancias[u_id] + peso;
                    predecessores[v_id] = u_id;
                    pq.push({distancias[v_id], v_id});
                }
            }
        }
    }

    // Reconstrução do caminho do nó de destino de volta para o nó de origem.
    vector<char> caminho;
    char no_atual = id_no_b;

    // Se não houver predecessor para o nó de destino (exceto se for o nó de origem), não há caminho.
    if (predecessores[no_atual] == 0 && no_atual != id_no_a)
    {
        return caminho; // Retorna um vetor vazio
    }

    while (no_atual != 0)
    {
        caminho.push_back(no_atual);
        no_atual = predecessores[no_atual];
    }

    // O caminho foi construído de trás para frente, então é preciso invertê-lo.
    reverse(caminho.begin(), caminho.end());

    // Se o caminho não começar com o nó de origem, significa que não há caminho.
    if (caminho.front() != id_no_a)
    {
        return {}; // Retorna um vetor vazio
    }

    return caminho;
}

vector<char> Grafo::caminho_minimo_floyd(char id_no_a, char id_no_b)
{
    int numVertices = this->ordem;
    vector<char> caminho;

    if (numVertices == 0)
    {
        return caminho;
    }

    vector<vector<int>> dist(numVertices, vector<int>(numVertices, numeric_limits<int>::max()));
    vector<vector<int>> prox(numVertices, vector<int>(numVertices, -1));

    // Mapa para converter char ID para índice da matriz
    map<char, int> id_para_indice;
    for (int i = 0; i < numVertices; ++i)
    {
        id_para_indice[this->lista_adj[i]->id] = i;
    }

    for (int i = 0; i < numVertices; ++i)
    {
        dist[i][i] = 0;
        prox[i][i] = i;
    }

    for (Aresta *aresta : this->arestas)
    {
        int u = id_para_indice[aresta->id_no_origem];
        int v = id_para_indice[aresta->id_no_alvo];
        int peso = this->in_ponderado_aresta ? aresta->peso : 1;

        if (dist[u][v] > peso)
        {
            dist[u][v] = peso;
            prox[u][v] = v;
        }
        if (!this->in_direcionado && dist[v][u] > peso)
        {
            dist[v][u] = peso;
            prox[v][u] = u;
        }
    }

    // Implementação do algoritmo
    for (int k = 0; k < numVertices; k++)
    {
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (dist[i][k] != numeric_limits<int>::max() && dist[k][j] != numeric_limits<int>::max() && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    prox[i][j] = prox[i][k]; 
                }
            }
        }
    }

    int u = id_para_indice[id_no_a];
    int v = id_para_indice[id_no_b];

    if (prox[u][v] == -1)
        return caminho;

    caminho.push_back(this->lista_adj[u]->id);
    while (u != v)
    {
        u = prox[u][v];
        caminho.push_back(this->lista_adj[u]->id);
    }

    return caminho;
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

    // Filtrar arestas do subgrafo induzido
    vector<Aresta *> arestasInduzidas;
    for (Aresta *aresta : this->arestas)
    {
        bool origem_in = find(ids_nos.begin(), ids_nos.end(), aresta->id_no_origem) != ids_nos.end();
        bool alvo_in = find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end();

        if (origem_in && alvo_in)
        {
            arestasInduzidas.push_back(aresta);
        }
    }

    // Ordenar arestas por peso
    sort(arestasInduzidas.begin(), arestasInduzidas.end(),
         [](Aresta *a, Aresta *b) { return a->peso < b->peso; });

    // Inicializar estrutura de conjuntos
    map<char, char> pai;
    for (char id : ids_nos)
        pai[id] = id;

    function<char(char)> find = [&](char x) {
        if (pai[x] != x)
            pai[x] = find(pai[x]);
        return pai[x];
    };

    auto unir = [&](char a, char b) {
        pai[find(a)] = find(b);
    };

    Grafo *agm = new Grafo();
    agm->in_direcionado = false;
    agm->in_ponderado_aresta = true;

    for (char id : ids_nos)
        agm->lista_adj.push_back(this->getNoPorId(id));

    for (Aresta *aresta : arestasInduzidas)
    {
        char u = aresta->id_no_origem;
        char v = aresta->id_no_alvo;

        if (find(u) != find(v))
        {
            unir(u, v);
            agm->arestas.push_back(aresta);
        }
    }

    return agm;
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
