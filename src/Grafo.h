//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>

using namespace std;
class Grafo
{
public:
    Grafo();
    ~Grafo();

    vector<char> fecho_transitivo_direto(char id_no);                 // a
    vector<char> fecho_transitivo_indireto(char id_no);               // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b);      // d
    Grafo *arvore_geradora_minima_prim(vector<char> ids_nos);         // e
    Grafo *arvore_geradora_minima_kruskal(vector<char> ids_nos);      // f
    Grafo *arvore_caminhamento_profundidade(char id_no);              // g
    int raio();                                                       // h 1
    int diametro();                                                   // h 2
    vector<char> centro();                                            // h 3
    vector<char> periferia();                                         // h 4
    vector<char> conjunto_dominante_distancia2_guloso();              // i
    vector<char> conjunto_dominante_distancia2_guloso_randomizado(float alpha); //j
    vector<char> conjunto_dominante_grasp_reativo(int max_iteracoes, int tamanho_bloco_atualizacao); //k
    void imprimirListaAdjacencias();
    No *getNoPorId(char id);
    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No *> lista_adj;
    vector<Aresta *> arestas;

private:
    void dfs_inverso(char id_atual, map<char, bool> &visitados);
    map<char, map<char, int>> calcularTodasDistancias(); // auxiliar para diametro e raio
    int calcular_score_distancia2(char id_candidato, const set<char>& nao_cobertos) const;
    std::set<char> obter_cobertura_distancia2(char id_vertice) const;
    bool is_solucao_valida(const vector<char>& solucao) const;
    void refinar_solucao_localmente(vector<char>& solucao) const;
};

#endif // GRAFO_H
