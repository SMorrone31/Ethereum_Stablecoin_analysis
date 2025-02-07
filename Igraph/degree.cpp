/*Questo codice calcola il grado e la forza in ingresso e in uscita di ogni nodo nel grafo diretto e pesato, collassato. 
Qui utiilzzo anche OpenMP per parallelizzare il calcolo delle metriche. 
Il grafo viene letto da file utilizzando la funzione read_weighted_graph() definita in graph_utils.hpp.*/

#include <igraph.h>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <vector>
#include "graph_utils.hpp"

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <graph_file> <output_file>\n";
        return 1;
    }

    // qui viene caricato il grafo diretto e pesato di ogni chunk temporale. I pesi degli archi come sappiamo hanno due valori: numero di transaiozni tra due nodi e il valore aggregato delle transazioni.
    igraph_t graph;
    igraph_vector_t weights_num_transazioni;
    igraph_vector_t weights_somma_valori;
    igraph_vector_init(&weights_num_transazioni, 0);
    igraph_vector_init(&weights_somma_valori, 0);

    if (!read_weighted_graph(argv[1], &graph, &weights_num_transazioni, &weights_somma_valori)) {
        cerr << "Errore nella lettura del grafo pesato\n";
        return 1;
    }

    // controllo importante per la direzionalità per il calcolo di queste metriche
    if (!igraph_is_directed(&graph)) {
        cerr << "Errore: il grafo non è diretto come previsto\n";
        igraph_vector_destroy(&weights_num_transazioni);
        igraph_vector_destroy(&weights_somma_valori);
        igraph_destroy(&graph);
        return 1;
    }
    
    int num_nodes = igraph_vcount(&graph);
    cout << "Grafo creato con " << num_nodes << " nodi e " << igraph_ecount(&graph) << " archi." << endl;

    // vettori per contare il numero di archi in ingresso e in uscita e sommare i pesi degli archi in ingresso e in uscita (grado e forza)
    vector<double> weighted_indegree_value(num_nodes, 0.0);
    vector<double> weighted_outdegree_value(num_nodes, 0.0);
    vector<double> indegree_num_trans(num_nodes, 0.0);
    vector<double> outdegree_num_trans(num_nodes, 0.0);

    // calcolo parallelo delle metriche per ogni arco
    #pragma omp parallel for
    for (int i = 0; i < igraph_ecount(&graph); ++i) {
        igraph_integer_t from, to;
        igraph_edge(&graph, i, &from, &to);

        double weight_num_trans = VECTOR(weights_num_transazioni)[i];
        double weight_value = VECTOR(weights_somma_valori)[i];

        #pragma omp atomic
        outdegree_num_trans[from] += weight_num_trans;
        #pragma omp atomic
        indegree_num_trans[to] += weight_num_trans;

        #pragma omp atomic
        weighted_outdegree_value[from] += weight_value;
        #pragma omp atomic
        weighted_indegree_value[to] += weight_value;
    }

    // salva i risultati sul file (uno per ogni chunk temporale)
    ofstream output_file(argv[2]);
    if (!output_file.is_open()) {
        cerr << "Errore nell'apertura del file CSV di output\n";
        return 1;
    }

    output_file << "Node,In-Degree,Out-Degree,In-Strength,Out-Strength\n";
    
    for (int i = 0; i < num_nodes; i++) {
        output_file << i << "," 
                    << indegree_num_trans[i] << "," 
                    << outdegree_num_trans[i] << "," 
                    << weighted_indegree_value[i] << "," 
                    << weighted_outdegree_value[i] << "\n";
    }

    // chiude il file e libera la memoria
    output_file.close();
    cout << "Risultati salvati nel file di output." << endl;
    igraph_vector_destroy(&weights_num_transazioni);
    igraph_vector_destroy(&weights_somma_valori);
    igraph_destroy(&graph);

    return 0;
}
