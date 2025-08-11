#include "Grafo.h"
#include <algorithm>
#include <limits>
#include <map>
#include <queue>
#include <vector>
#include <functional>
#include <set>
#include <numeric>
#include <random>

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
    map<char, int> distancias;

    map<char, char> predecessores;

    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;

    for (No *no : this->lista_adj)
    {
        distancias[no->id] = numeric_limits<int>::max();
        predecessores[no->id] = 0;
    }

    distancias[id_no_a] = 0;
    pq.push({0, id_no_a});

    while (!pq.empty())
    {
        char u_id = pq.top().second;
        pq.pop();

        No *u_no = getNoPorId(u_id);
        if (!u_no)
            continue;

        for (Aresta *aresta : this->arestas)
        {
            if (aresta->id_no_origem == u_id)
            {
                char v_id = aresta->id_no_alvo;
                int peso = this->in_ponderado_aresta ? aresta->peso : 1;

                if (distancias[u_id] != numeric_limits<int>::max() && distancias[u_id] + peso < distancias[v_id])
                {
                    distancias[v_id] = distancias[u_id] + peso;
                    predecessores[v_id] = u_id;
                    pq.push({distancias[v_id], v_id});
                }
            }
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

    vector<char> caminho;
    char no_atual = id_no_b;

    if (predecessores[no_atual] == 0 && no_atual != id_no_a)
    {
        return caminho;
    }

    while (no_atual != 0)
    {
        caminho.push_back(no_atual);
        no_atual = predecessores[no_atual];
    }

    reverse(caminho.begin(), caminho.end());

    if (caminho.front() != id_no_a)
    {
        return {};
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

    map<char, int> idVertice;
    for (int i = 0; i < numVertices; ++i)
    {
        idVertice[this->lista_adj[i]->id] = i;
    }

    for (int i = 0; i < numVertices; ++i)
    {
        dist[i][i] = 0;
        prox[i][i] = i;
    }

    for (Aresta *aresta : this->arestas)
    {
        int u = idVertice[aresta->id_no_origem];
        int v = idVertice[aresta->id_no_alvo];
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

    int u = idVertice[id_no_a];
    int v = idVertice[id_no_b];

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

    sort(arestasInduzidas.begin(), arestasInduzidas.end(),
         [](Aresta *a, Aresta *b)
         { return a->peso < b->peso; });

    map<char, char> pai;
    for (char id : ids_nos)
        pai[id] = id;

    function<char(char)> find_set = [&](char x)
    {
        if (pai[x] != x)
            pai[x] = find_set(pai[x]);
        return pai[x];
    };

    auto unir = [&](char a, char b)
    {
        pai[find_set(a)] = find_set(b);
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

        if (find_set(u) != find_set(v))
        {
            unir(u, v);
            agm->arestas.push_back(aresta);
        }
    }

    return agm;
}

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    if (getNoPorId(id_no) == nullptr)
    {
        cout << "Erro: O no inicial '" << id_no << "' nao existe no grafo." << endl;
        return nullptr;
    }

    Grafo *arvore = new Grafo();
    arvore->in_direcionado = this->in_direcionado;
    arvore->in_ponderado_aresta = this->in_ponderado_aresta;
    arvore->in_ponderado_vertice = this->in_ponderado_vertice;
    arvore->ordem = 0;

    map<char, int> cor;
    for (No *no : this->lista_adj)
    {
        cor[no->id] = 0;
    }

    cout << "Iniciando busca em profundidade a partir de '" << id_no << "'." << endl;
    cout << "Arestas de retorno (nao pertencem a arvore):" << endl;

    function<void(char)> dfs_visit;
    dfs_visit = [&](char u_id)
    {
        cor[u_id] = 1;
        arvore->lista_adj.push_back(this->getNoPorId(u_id));
        arvore->ordem++;

        for (Aresta *aresta : this->arestas)
        {
            char v_id = 0;
            bool eh_vizinho = false;

            if (aresta->id_no_origem == u_id)
            {
                v_id = aresta->id_no_alvo;
                eh_vizinho = true;
            }
            else if (!this->in_direcionado && aresta->id_no_alvo == u_id)
            {
                v_id = aresta->id_no_origem;
                eh_vizinho = true;
            }

            if (eh_vizinho)
            {
                if (cor[v_id] == 0)
                {
                    arvore->arestas.push_back(aresta);
                    dfs_visit(v_id);
                }
                else if (cor[v_id] == 1)
                {
                    cout << "  - Aresta de retorno: " << u_id << " -> " << v_id << endl;
                }
            }
        }
        cor[u_id] = 2;
    };

    dfs_visit(id_no);

    return arvore;
}

map<char, map<char, int>> Grafo::calcularTodasDistancias()
{
    int numVertices = this->ordem;
    map<char, map<char, int>> distancias;

    if (numVertices == 0)
        return distancias;

    map<char, int> idDeAcordoAoIndice;
    vector<char> indiceDeAcordoAoId(numVertices);
    for (int i = 0; i < numVertices; ++i)
    {
        idDeAcordoAoIndice[this->lista_adj[i]->id] = i;
        indiceDeAcordoAoId[i] = this->lista_adj[i]->id;
    }

    vector<vector<int>> dist(numVertices, vector<int>(numVertices, numeric_limits<int>::max()));

    for (int i = 0; i < numVertices; ++i)
    {
        dist[i][i] = 0;
    }

    for (Aresta *aresta : this->arestas)
    {
        int u = idDeAcordoAoIndice[aresta->id_no_origem];
        int v = idDeAcordoAoIndice[aresta->id_no_alvo];
        int peso = this->in_ponderado_aresta ? aresta->peso : 1;

        if (dist[u][v] > peso)
        {
            dist[u][v] = peso;
        }

        if (!this->in_direcionado && dist[v][u] > peso)
        {
            dist[v][u] = peso;
        }
    }

    for (int k = 0; k < numVertices; k++)
    {
        for (int i = 0; i < numVertices; i++)
        {
            for (int j = 0; j < numVertices; j++)
            {
                if (dist[i][k] != numeric_limits<int>::max() && dist[k][j] != numeric_limits<int>::max() && dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            distancias[indiceDeAcordoAoId[i]][indiceDeAcordoAoId[j]] = dist[i][j];
        }
    }

    return distancias;
}
int Grafo::raio()
{
    map<char, map<char, int>> distancias = this->calcularTodasDistancias();
    int raio = numeric_limits<int>::max();

    if (distancias.empty())
    {
        cout << "Grafo vazio ou nao conexo logo o raio nao pode ser calculado." << endl;
        return -1;
    }

    for (auto const &par_origem : distancias)
    {
        int excentricidade = 0;
        for (auto const &par_destino : par_origem.second)
        {
            int dist = par_destino.second;
            if (dist == numeric_limits<int>::max())
            {
                cout << "Grafo e conexo (fortemente)logo o raio e infinito" << endl;
                return numeric_limits<int>::max();
            }
            if (dist > excentricidade)
            {
                excentricidade = dist;
            }
        }
        if (excentricidade < raio)
        {
            raio = excentricidade;
        }
    }

    return raio;
}
int Grafo::diametro()
{
    map<char, map<char, int>> distancias = this->calcularTodasDistancias();
    int diametro = 0;

    if (distancias.empty())
    {
        cout << "Grafo vazio ou nao conexo logo o diametro nao pode ser calculado." << endl;
        return -1;
    }

    for (auto const &par_origem : distancias)
    {
        int excentricidade = 0;
        for (auto const &par_destino : par_origem.second)
        {
            int dist = par_destino.second;
            if (dist == numeric_limits<int>::max())
            {
                cout << "Grafo e conexo (fortemente) logo o diametro e infinito." << endl;
                return numeric_limits<int>::max();
            }
            if (dist > excentricidade)
            {
                excentricidade = dist;
            }
        }
        if (excentricidade > diametro)
        {
            diametro = excentricidade;
        }
    }

    return diametro;
}

vector<char> Grafo::centro()
{
    map<char, map<char, int>> distancias = this->calcularTodasDistancias();
    vector<char> centro_nodes;
    map<char, int> excentricidades;
    int raioGrafo = numeric_limits<int>::max();

    if (distancias.empty())
    {
        cout << "O grafo e um grafo vazio ou nao conexo." << endl;
        return centro_nodes;
    }

    for (auto const &par_origem : distancias)
    {
        char idDaOrigem = par_origem.first;
        int excentricidadeNova = 0;
        for (auto const &par_destino : par_origem.second)
        {
            int dist = par_destino.second;
            if (dist == numeric_limits<int>::max())
            {
                cout << "O grafo tem centro indefinido pois nao é fortemente conexo." << endl;
                return {};
            }
            if (dist > excentricidadeNova)
            {
                excentricidadeNova = dist;
            }
        }
        excentricidades[idDaOrigem] = excentricidadeNova;
        if (excentricidadeNova < raioGrafo)
        {
            raioGrafo = excentricidadeNova;
        }
    }

    for (auto const &par : excentricidades)
    {
        if (par.second == raioGrafo)
        {
            centro_nodes.push_back(par.first);
        }
    }

    return centro_nodes;
}

vector<char> Grafo::periferia()
{
    map<char, map<char, int>> distancias = this->calcularTodasDistancias();
    vector<char> periferia_nodes;
    map<char, int> excentricidades;
    int diametroGrafo = 0;

    if (distancias.empty())
    {
        cout << "Grafo vazio ou nao conexo." << endl;
        return periferia_nodes;
    }

    for (auto const &par_origem : distancias)
    {
        char idDaOrigem = par_origem.first;
        int excentricidadeNova = 0;
        for (auto const &par_destino : par_origem.second)
        {
            int dist = par_destino.second;
            if (dist == numeric_limits<int>::max())
            {
                cout << "Periferia indefinida pois o grafo nao e fortemente conexo." << endl;
                return {};
            }
            if (dist > excentricidadeNova)
            {
                excentricidadeNova = dist;
            }
        }
        excentricidades[idDaOrigem] = excentricidadeNova;
        if (excentricidadeNova > diametroGrafo)
        {
            diametroGrafo = excentricidadeNova;
        }
    }

    for (auto const &par : excentricidades)
    {
        if (par.second == diametroGrafo)
        {
            periferia_nodes.push_back(par.first);
        }
    }

    return periferia_nodes;
}

//---------------------------------------------------------- TRABALHO 2 -------------------------------------------------------

vector<char> Grafo::conjunto_dominante_distancia2_guloso()
{
    vector<char> D;
    if (this->ordem == 0)
    {
        return D;
    }

    std::set<char> nao_cobertos;
    for (No *no : this->lista_adj)
    {
        nao_cobertos.insert(no->id);
    }

    while (!nao_cobertos.empty())
    {
        char melhor_vertice_para_adicionar = 0;
        int max_score = -1;

        for (No *candidato_no : this->lista_adj)
        {
            int score_atual = this->calcular_score_distancia2(candidato_no->id, nao_cobertos);

            if (score_atual > max_score)
            {
                max_score = score_atual;
                melhor_vertice_para_adicionar = candidato_no->id;
            }
        }

        if (melhor_vertice_para_adicionar == 0)
        {
            if (!nao_cobertos.empty())
            {
                melhor_vertice_para_adicionar = *nao_cobertos.begin();
            }
            else
            {
                break;
            }
        }

        D.push_back(melhor_vertice_para_adicionar);

        std::set<char> para_remover = this->obter_cobertura_distancia2(melhor_vertice_para_adicionar);

        for (char id_remover : para_remover)
        {
            nao_cobertos.erase(id_remover);
        }
    }

    sort(D.begin(), D.end());
    return D;
}

vector<char> Grafo::conjunto_dominante_distancia2_guloso_randomizado(float alpha)
{
    if (alpha < 0.0f || alpha > 1.0f)
    {
        throw invalid_argument("O parâmetro alpha deve estar entre 0.0 e 1.0.");
    }

    vector<char> D;
    if (this->ordem == 0)
    {
        return D;
    }

    std::set<char> nao_cobertos;
    for (No *no : this->lista_adj)
    {
        nao_cobertos.insert(no->id);
    }

    mt19937 gerador_aleatorio(random_device{}());

    while (!nao_cobertos.empty())
    {
        vector<pair<char, int>> scores_dos_candidatos;
        int max_score = 0;

        for (No *candidato_no : this->lista_adj)
        {
            int score_atual = this->calcular_score_distancia2(candidato_no->id, nao_cobertos);

            if (score_atual > 0)
            {
                scores_dos_candidatos.push_back({candidato_no->id, score_atual});
            }
            if (score_atual > max_score)
            {
                max_score = score_atual;
            }
        }

        vector<char> rcl;
        char melhor_vertice_para_adicionar = 0;

        if (max_score > 0)
        {
            float limite_score = alpha * max_score;
            for (const auto &par : scores_dos_candidatos)
            {
                if (par.second >= limite_score)
                {
                    rcl.push_back(par.first);
                }
            }
        }

        if (!rcl.empty())
        {
            uniform_int_distribution<> distrib(0, rcl.size() - 1);
            melhor_vertice_para_adicionar = rcl[distrib(gerador_aleatorio)];
        }
        else
        {
            if (!nao_cobertos.empty())
            {
                uniform_int_distribution<> distrib(0, nao_cobertos.size() - 1);
                auto it = nao_cobertos.begin();
                advance(it, distrib(gerador_aleatorio));
                melhor_vertice_para_adicionar = *it;
            }
            else
            {
                break;
            }
        }

        D.push_back(melhor_vertice_para_adicionar);

        std::set<char> para_remover = this->obter_cobertura_distancia2(melhor_vertice_para_adicionar);

        for (char id_remover : para_remover)
        {
            nao_cobertos.erase(id_remover);
        }
    }

    sort(D.begin(), D.end());
    return D;
}

vector<char> Grafo::conjunto_dominante_grasp_reativo(int max_iteracoes, int tamanho_bloco_atualizacao)
{
    vector<float> alphas_disponiveis = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f};
    int num_alphas = alphas_disponiveis.size();

    vector<double> probabilidades(num_alphas, 1.0 / num_alphas);
    vector<double> soma_qualidade_solucoes(num_alphas, 0.0);
    vector<int> contador_usos(num_alphas, 0);

    vector<char> melhor_solucao_global;
    size_t melhor_custo_global = -1; // Equivalente a infinito

    mt19937 gerador_aleatorio(random_device{}());

    for (int i = 0; i < max_iteracoes; ++i)
    {
        // a. Seleciona um alpha com base nas probabilidades atuais
        discrete_distribution<> dist_alphas(probabilidades.begin(), probabilidades.end());
        int indice_alpha_escolhido = dist_alphas(gerador_aleatorio);
        float alpha_atual = alphas_disponiveis[indice_alpha_escolhido];

        // usa o GRASP padrão com o alpha escolhido
        vector<char> solucao_construida = this->conjunto_dominante_distancia2_guloso_randomizado(alpha_atual);

        // refina a solução construída
        this->refinar_solucao_localmente(solucao_construida);

        // Atualiza a melhor solução global encontrada até agora
        if (solucao_construida.size() < melhor_custo_global)
        {
            melhor_custo_global = solucao_construida.size();
            melhor_solucao_global = solucao_construida;
            cout << "Iteração " << i << ": Nova melhor solução encontrada com custo " << melhor_custo_global << " (usando alpha=" << alpha_atual << ")" << endl;
        }

        // Registra o desempenho do alpha utilizado
        soma_qualidade_solucoes[indice_alpha_escolhido] += solucao_construida.size();
        contador_usos[indice_alpha_escolhido]++;

        // Atualização Reativa das Probabilidades (a cada bloco de iterações)
        if ((i + 1) % tamanho_bloco_atualizacao == 0)
        {
            double soma_total_q = 0;
            vector<double> q(num_alphas);

            for (int j = 0; j < num_alphas; ++j)
            {
                if (contador_usos[j] > 0)
                {
                    double media_custo = soma_qualidade_solucoes[j] / contador_usos[j];
                    // A qualidade é o inverso do custo médio (melhor custo -> maior qualidade)
                    q[j] = 1.0 / media_custo;
                }
                else
                {
                    q[j] = 0; // Se nunca foi usado, não tem qualidade
                }
                soma_total_q += q[j];
            }

            if (soma_total_q > 0)
            {
                for (int j = 0; j < num_alphas; ++j)
                {
                    probabilidades[j] = q[j] / soma_total_q;
                }
            }
            else
            {
                // Se nenhum alpha produziu resultado (raro), reseta para uniforme
                for (int j = 0; j < num_alphas; ++j)
                {
                    probabilidades[j] = 1.0 / num_alphas;
                }
            }
        }
    }

    sort(melhor_solucao_global.begin(), melhor_solucao_global.end());
    return melhor_solucao_global;
}

//------------------------------ FUNÇÕES AUXILIARES PARA ALGORITMO GULOSO ----------------------

// Retorna o conjunto de vértices cobertos a uma distância de até 2 de um vértice.
std::set<char> Grafo::obter_cobertura_distancia2(char id_vertice) const
{
    std::set<char> cobertos;
    vector<char> vizinhos_dist1;

    // Distância 0
    cobertos.insert(id_vertice);

    // Distância 1
    for (Aresta *aresta : this->arestas)
    {
        if (aresta->id_no_origem == id_vertice)
        {
            vizinhos_dist1.push_back(aresta->id_no_alvo);
        }
        else if (!this->in_direcionado && aresta->id_no_alvo == id_vertice)
        {
            vizinhos_dist1.push_back(aresta->id_no_origem);
        }
    }
    cobertos.insert(vizinhos_dist1.begin(), vizinhos_dist1.end());

    // Distância 2
    for (char id_vizinho1 : vizinhos_dist1)
    {
        for (Aresta *aresta : this->arestas)
        {
            if (aresta->id_no_origem == id_vizinho1)
            {
                cobertos.insert(aresta->id_no_alvo);
            }
            else if (!this->in_direcionado && aresta->id_no_alvo == id_vizinho1)
            {
                cobertos.insert(aresta->id_no_origem);
            }
        }
    }
    return cobertos;
}

// Calcula quantos vértices AINDA NÃO COBERTOS um candidato consegue cobrir.
int Grafo::calcular_score_distancia2(char id_candidato, const std::set<char> &nao_cobertos) const
{
    // Todos os vértices que o candidato pode cobrir.
    std::set<char> cobertos_pelo_candidato = this->obter_cobertura_distancia2(id_candidato);

    // Conta quantos desses estão na lista de 'nao_cobertos'.
    int score = 0;
    for (char id_coberto : cobertos_pelo_candidato)
    {
        if (nao_cobertos.count(id_coberto))
        {
            score++;
        }
    }
    return score;
}

// Verifica se um dado conjunto de vértices é um conjunto dominante de distância 2 válido.
bool Grafo::is_solucao_valida(const vector<char> &solucao) const
{
    if (this->ordem == 0)
        return true;

    std::set<char> cobertos;
    for (char id_no_solucao : solucao)
    {
        std::set<char> cobertura_parcial = this->obter_cobertura_distancia2(id_no_solucao);
        cobertos.insert(cobertura_parcial.begin(), cobertura_parcial.end());
    }

    // Se o número de nós cobertos for igual à ordem do grafo, a solução é válida.
    return cobertos.size() == this->ordem;
}

// Tenta refinar uma solução removendo vértices redundantes.
void Grafo::refinar_solucao_localmente(vector<char> &solucao) const
{
    // Itera sobre uma cópia dos IDs para poder modificar o vetor original
    vector<char> copia_solucao = solucao;
    bool melhoria_encontrada = true;

    while (melhoria_encontrada)
    {
        melhoria_encontrada = false;
        char no_para_remover = 0;

        for (char id_no : copia_solucao)
        {
            // Cria uma solução temporária sem o nó atual
            vector<char> solucao_temp;
            for (char v : copia_solucao)
            {
                if (v != id_no)
                {
                    solucao_temp.push_back(v);
                }
            }

            // Se a solução temporária ainda for válida, encontramos uma melhoria
            if (is_solucao_valida(solucao_temp))
            {
                no_para_remover = id_no;
                melhoria_encontrada = true;
                break; // Sai do laço interno para aplicar a remoção
            }
        }

        if (melhoria_encontrada)
        {
            // Remove o nó redundante da solução original e da cópia de trabalho
            solucao.erase(remove(solucao.begin(), solucao.end(), no_para_remover), solucao.end());
            copia_solucao = solucao;
        }
    }
}