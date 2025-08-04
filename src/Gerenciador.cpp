#include "Gerenciador.h"
#include <fstream>
#include <limits>
#include <vector>
#include <iostream>

void Gerenciador::comandos(Grafo *grafo)
{
    cout << "Digite uma das opcoes abaixo e pressione enter:" << endl
         << endl;
    cout << "(a) Fecho transitivo direto de um no;" << endl;
    cout << "(b) Fecho transitivo indireto de um no;" << endl;
    cout << "(c) Caminho minimo (Djikstra);" << endl;
    cout << "(d) Caminho minimo (Floyd);" << endl;
    cout << "(e) Arvore Geradora Minima (Algoritmo de Prim);" << endl;
    cout << "(f) Arvore Geradora Minima (Algoritmo de Kruskal);" << endl;
    cout << "(g) Arvore de caminhamento em profundidade;" << endl;
    cout << "(h) Raio, diametro, centro e periferia do grafo;" << endl;
    cout << "(i) Encontrar um Conjunto 2-Dominante (heuristica gulosa);" << endl; // <-- NOVA OPÇÃO

    cout << "(0) Sair;" << endl
         << endl;

    char resp;
    cin >> resp;
    switch (resp)
    {
    case 'a':
    {
        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
        cout << "Fecho transitivo direto: ";
        for (char v : fecho_transitivo_direto)
        {
            cout << v << " ";
        }
        cout << endl
             << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_dir.txt"))
        {
            ofstream out("fecho_trans_dir.txt");
            for (char v : fecho_transitivo_direto)
            {
                out << v << " ";
            }
            out << endl;
            out.close();
            cout << "Fecho salvo em fecho_trans_dir.txt\n\n";
        }
        break;
    }

    case 'b':
    {
        char id_no = get_id_entrada();
        vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
        cout << "Fecho transitivo indireto: ";
        for (char v : fecho_transitivo_indireto)
        {
            cout << v << " ";
        }
        cout << endl
             << endl;

        if (pergunta_imprimir_arquivo("fecho_trans_indir.txt"))
        {
            ofstream out("fecho_trans_indir.txt");
            for (char v : fecho_transitivo_indireto)
            {
                out << v << " ";
            }
            out << endl;
            out.close();
            cout << "Fecho salvo em fecho_trans_indir.txt\n\n";
        }
        break;
    }

    case 'c':
    {
        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);

        if (caminho_minimo.empty())
        {
            cout << "Nao existe caminho entre " << id_no_1 << " e " << id_no_2 << "." << endl
                 << endl;
        }
        else
        {
            cout << "Caminho minimo (Dijkstra): ";
            for (size_t i = 0; i < caminho_minimo.size(); ++i)
            {
                cout << caminho_minimo[i] << (i < caminho_minimo.size() - 1 ? " -> " : "");
            }
            cout << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt"))
        {
            if (!caminho_minimo.empty())
            {
                ofstream out("caminho_minimo_dijkstra.txt");
                for (size_t i = 0; i < caminho_minimo.size(); ++i)
                {
                    out << caminho_minimo[i] << (i < caminho_minimo.size() - 1 ? " -> " : "");
                }
                out << endl;
                out.close();
                cout << "Caminho salvo em caminho_minimo_dijkstra.txt\n\n";
            }
            else
            {
                cout << "Nenhum caminho para salvar no arquivo." << endl
                     << endl;
            }
        }
        break;
    }

    case 'd':
    {
        char id_no_1 = get_id_entrada();
        char id_no_2 = get_id_entrada();
        vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1, id_no_2);

        if (caminho_minimo_floyd.empty())
        {
            cout << "Nao existe caminho entre " << id_no_1 << " e " << id_no_2 << "." << endl
                 << endl;
        }
        else
        {
            cout << "Caminho minimo (Floyd): ";
            for (size_t i = 0; i < caminho_minimo_floyd.size(); ++i)
            {
                cout << caminho_minimo_floyd[i] << (i < caminho_minimo_floyd.size() - 1 ? " -> " : "");
            }
            cout << endl
                 << endl;
        }

        if (pergunta_imprimir_arquivo("caminho_minimo_floyd.txt"))
        {
            if (!caminho_minimo_floyd.empty())
            {
                ofstream out("caminho_minimo_floyd.txt");
                for (size_t i = 0; i < caminho_minimo_floyd.size(); ++i)
                {
                    out << caminho_minimo_floyd[i] << (i < caminho_minimo_floyd.size() - 1 ? " -> " : "");
                }
                out << endl;
                out.close();
                cout << "Caminho salvo em caminho_minimo_floyd.txt\n\n";
            }
            else
            {
                cout << "Nenhum caminho para salvar no arquivo." << endl
                     << endl;
            }
        }
        break;
    }

    case 'e':
    {
        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {
            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
            if (arvore_geradora_minima_prim != nullptr)
            {
                arvore_geradora_minima_prim->imprimirListaAdjacencias();
                cout << endl;

                if (pergunta_imprimir_arquivo("agm_prim.txt"))
                {
                    ofstream out("agm_prim.txt");
                    for (No *no_origem : arvore_geradora_minima_prim->lista_adj)
                    {
                        out << no_origem->id << " -> ";
                        for (Aresta *aresta : arvore_geradora_minima_prim->arestas)
                        {
                            if (aresta->id_no_origem == no_origem->id)
                            {
                                out << aresta->id_no_alvo;
                                if (grafo->in_ponderado_aresta)
                                {
                                    out << "(" << aresta->peso << ")";
                                }
                                out << " ";
                            }
                            else if (!grafo->in_direcionado && aresta->id_no_alvo == no_origem->id)
                            {
                                out << aresta->id_no_origem;
                                if (grafo->in_ponderado_aresta)
                                {
                                    out << "(" << aresta->peso << ")";
                                }
                                out << " ";
                            }
                        }
                        out << endl;
                    }
                    out.close();
                    cout << "AGM (Prim) salva em agm_prim.txt\n\n";
                }
                delete arvore_geradora_minima_prim;
            }
        }
        else
        {
            cout << "Valor invalido" << endl;
        }
        break;
    }

    case 'f':
    {
        int tam;
        cout << "Digite o tamanho do subconjunto: ";
        cin >> tam;

        if (tam > 0 && tam <= grafo->ordem)
        {
            vector<char> ids = get_conjunto_ids(grafo, tam);
            Grafo *arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
            if (arvore_geradora_minima_kruskal != nullptr)
            {
                arvore_geradora_minima_kruskal->imprimirListaAdjacencias();
                cout << endl;

                if (pergunta_imprimir_arquivo("agm_kruskal.txt"))
                {
                    ofstream out("agm_kruskal.txt");
                    for (No *no_origem : arvore_geradora_minima_kruskal->lista_adj)
                    {
                        out << no_origem->id << " -> ";
                        for (Aresta *aresta : arvore_geradora_minima_kruskal->arestas)
                        {
                            if (aresta->id_no_origem == no_origem->id)
                            {
                                out << aresta->id_no_alvo;
                                if (grafo->in_ponderado_aresta)
                                {
                                    out << "(" << aresta->peso << ")";
                                }
                                out << " ";
                            }
                            else if (!grafo->in_direcionado && aresta->id_no_alvo == no_origem->id)
                            {
                                out << aresta->id_no_origem;
                                if (grafo->in_ponderado_aresta)
                                {
                                    out << "(" << aresta->peso << ")";
                                }
                                out << " ";
                            }
                        }
                        out << endl;
                    }
                    out.close();
                    cout << "AGM (Kruskal) salva em agm_kruskal.txt\n\n";
                }
                delete arvore_geradora_minima_kruskal;
            }
        }
        else
        {
            cout << "Valor invalido" << endl;
        }
        break;
    }

    case 'g':
    {
        char id_no = get_id_entrada();
        Grafo *arvore = grafo->arvore_caminhamento_profundidade(id_no);

        if (arvore != nullptr)
        {
            cout << "\nArvore de caminhamento em profundidade gerada:" << endl;
            arvore->imprimirListaAdjacencias();
            cout << endl;

            if (pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt"))
            {
                ofstream out("arvore_caminhamento_profundidade.txt");
                if (!out.is_open())
                {
                    cout << "Erro ao abrir o arquivo para escrita." << endl;
                }
                else
                {
                    for (No *no_origem : arvore->lista_adj)
                    {
                        out << no_origem->id << " -> ";
                        for (Aresta *aresta : arvore->arestas)
                        {
                            if (aresta->id_no_origem == no_origem->id)
                            {
                                out << aresta->id_no_alvo;
                                if (arvore->in_ponderado_aresta)
                                    out << "(" << aresta->peso << ")";
                                out << " ";
                            }
                            else if (!arvore->in_direcionado && aresta->id_no_alvo == no_origem->id)
                            {
                                out << aresta->id_no_origem;
                                if (arvore->in_ponderado_aresta)
                                    out << "(" << aresta->peso << ")";
                                out << " ";
                            }
                        }
                        out << endl;
                    }
                    cout << "Arvore salva em arvore_caminhamento_profundidade.txt\n\n";
                }
                out.close();
            }
            delete arvore;
        }
        break;
    }

    case 'h':
    {
        int r = grafo->raio();
        int d = grafo->diametro();
        vector<char> c = grafo->centro();
        vector<char> p = grafo->periferia();

        if (r != -1)
        {
            cout << "Raio do grafo: " << (r == numeric_limits<int>::max() ? "Infinito" : to_string(r)) << endl;
        }
        if (d != -1)
        {
            cout << "Diametro do grafo: " << (d == numeric_limits<int>::max() ? "Infinito" : to_string(d)) << endl;
        }

        cout << "Centro do grafo: { ";
        for (char id : c)
        {
            cout << id << " ";
        }
        cout << "}" << endl;

        cout << "Periferia do grafo: { ";
        for (char id : p)
        {
            cout << id << " ";
        }
        cout << "}" << endl
             << endl;

        if (pergunta_imprimir_arquivo("metricas.txt"))
        {
            ofstream out("metricas.txt");
            if (r != -1)
                out << "Raio: " << (r == numeric_limits<int>::max() ? "Infinito" : to_string(r)) << endl;
            if (d != -1)
                out << "Diametro: " << (d == numeric_limits<int>::max() ? "Infinito" : to_string(d)) << endl;

            out << "Centro: { ";
            for (char id : c)
                out << id << " ";
            out << "}" << endl;

            out << "Periferia: { ";
            for (char id : p)
                out << id << " ";
            out << "}" << endl;

            out.close();
            cout << "Metricas salvas em metricas.txt\n\n";
        }
        break;
    }

    case 'i':
    {
        vector<char> c2d = grafo->conjunto_2_dominante_guloso();
        cout << "Conjunto 2-Dominante (heuristica gulosa): { ";
        if (c2d.empty())
        {
            cout << "Nenhum";
        }
        else
        {
            for (char id : c2d)
            {
                cout << id << " ";
            }
        }
        cout << "}" << endl
             << endl;

        if (pergunta_imprimir_arquivo("conjunto_2_dominante.txt"))
        {
            ofstream out("conjunto_2_dominante.txt");
            out << "Conjunto 2-Dominante (heuristica gulosa): { ";
            if (c2d.empty())
            {
                out << "Nenhum";
            }
            else
            {
                for (char id : c2d)
                {
                    out << id << " ";
                }
            }
            out << "}" << endl;
            out.close();
            cout << "Conjunto 2-Dominante salvo em conjunto_2_dominante.txt\n\n";
        }
        break;
    }

    case '0':
    {
        exit(0);
    }
    default:
    {
        cout << "Opção inválida" << endl;
    }
    }

    comandos(grafo);
}

char Gerenciador::get_id_entrada()
{
    cout << "Digite o id de um no: ";
    char id;
    cin >> id;
    cout << endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam)
{
    vector<char> ids = {};
    cout << "Digite os " << tam << " ids do subconjunto:" << endl;
    while ((int)ids.size() < tam)
    {
        char id_no = get_id_entrada();
        bool existe = false;
        for (No *no : grafo->lista_adj)
        {
            if (no->id == id_no)
            {
                existe = true;
                break;
            }
        }

        if (!existe)
        {
            cout << "Vertice nao existe" << endl;
        }
        else
        {
            bool repetido = find(ids.begin(), ids.end(), id_no) != ids.end();
            if (repetido)
            {
                cout << "Valor repetido" << endl;
            }
            else
            {
                ids.push_back(id_no);
            }
        }
    }
    return ids;
}

bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo)
{
    cout << "Imprimir em arquivo externo? (" << nome_arquivo << ")" << endl;
    cout << "(1) Sim;" << endl;
    cout << "(2) Nao." << endl;
    int resp;
    cin >> resp;

    while (cin.fail())
    {
        cout << "Entrada invalida. Por favor, digite 1 para Sim ou 2 para Nao." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin >> resp;
    }

    cout << endl;

    switch (resp)
    {
    case 1:
        return true;
    case 2:
        return false;
    default:
        cout << "Resposta invalida" << endl;
        return pergunta_imprimir_arquivo(nome_arquivo);
    }
}