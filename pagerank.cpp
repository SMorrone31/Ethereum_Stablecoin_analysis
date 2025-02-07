/*Questo codice calcola il PageRank, l'Hub e Authority score per ogni grafo dei chunk temporali. 
Per una migliore precisione, vengono combinati gli hub score e gli authority score tra di loro con la somma logaritmica in quanto vengono calcolati per i due pesi ottenuti: motleplicità e somma dei valori delle transazioni.*/

#include <igraph.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "utils/graph_utils.hpp"
#include <cmath> 

using namespace std;

// Funzione per calcolare PageRank: utilizza la funzione di igraph con damping factor = 0.85.
// il peso degli archi guida la distribuzione del PageRank. 
void compute_pagerank(const igraph_t* graph, igraph_vector_t* weights, vector<double>& pagerank_result) {
    igraph_vector_t pagerank_vector;
    igraph_real_t damping = 0.85;
    igraph_vector_init(&pagerank_vector, igraph_vcount(graph));
    igraph_real_t value = 0;
    igraph_pagerank_algo_t algo = IGRAPH_PAGERANK_ALGO_PRPACK;
    igraph_vs_t all_vertices;
    igraph_vs_all(&all_vertices); 
    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);
    igraph_personalized_pagerank(graph, algo, &pagerank_vector, &value, all_vertices, IGRAPH_DIRECTED, damping, nullptr, weights, &options);

    for (int i = 0; i < igraph_vcount(graph); i++) {
        pagerank_result.push_back(VECTOR(pagerank_vector)[i]);
    }

    igraph_vector_destroy(&pagerank_vector);
    igraph_vs_destroy(&all_vertices);
}

// Calcola due insiemi di punteggi Hub e Authority con igraph_hub_and_authority_scores(), usando i due pesi individuati. Infine calcola la somma logaritmica che enfatizza i nodi con valori elevati in entrambi gli aspetti, senza dominanza di un solo peso
void compute_hub_and_authority_log_sum(const igraph_t* graph, igraph_vector_t* weights1, igraph_vector_t* weights2, vector<double>& hub_combined, vector<double>& authority_combined) {
    igraph_vector_t hub_scores1, authority_scores1, hub_scores2, authority_scores2;
    igraph_real_t eigenvalue;
    igraph_vector_init(&hub_scores1, igraph_vcount(graph));
    igraph_vector_init(&authority_scores1, igraph_vcount(graph));
    igraph_vector_init(&hub_scores2, igraph_vcount(graph));
    igraph_vector_init(&authority_scores2, igraph_vcount(graph));

    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);
    igraph_hub_and_authority_scores(graph, &hub_scores1, &authority_scores1, &eigenvalue, 1, weights1, NULL);
    igraph_hub_and_authority_scores(graph, &hub_scores2, &authority_scores2, &eigenvalue, 1, weights2, NULL);

    for (int i = 0; i < igraph_vcount(graph); i++) {
        double hub_mult = VECTOR(hub_scores1)[i];
        double hub_value = VECTOR(hub_scores2)[i];
        double auth_mult = VECTOR(authority_scores1)[i];
        double auth_value = VECTOR(authority_scores2)[i];
        hub_combined.push_back(log(1 + hub_mult) + log(1 + hub_value));
        authority_combined.push_back(log(1 + auth_mult) + log(1 + auth_value));
    }

    igraph_vector_destroy(&hub_scores1);
    igraph_vector_destroy(&authority_scores1);
    igraph_vector_destroy(&hub_scores2);
    igraph_vector_destroy(&authority_scores2);
}


// Funzione per scrivere il PageRank in un file CSV (solo nodeId, PageRank)
void write_pagerank_to_csv(const char* output_file, const igraph_t* graph, const vector<double>& pagerank) {
    ofstream output(output_file);
    if (!output.is_open()) {
        cerr << "Error opening output file: " << output_file << endl;
        return;
    }

    output << "NodeId,PageRank\n";
    for (int i = 0; i < igraph_vcount(graph); i++) {
        output << i << "," << pagerank[i] << "\n";
    }

    output.close();
}

void write_hub_and_authority_log_sum_to_csv(const char* output_file, const igraph_t* graph, const vector<double>& hub_combined, const vector<double>& authority_combined) {
    ofstream output(output_file);
    if (!output.is_open()) {
        cerr << "Error opening output file: " << output_file << endl;
        return;
    }

    output << "NodeId,Hub_Combined,Authority_Combined\n";
    for (int i = 0; i < igraph_vcount(graph); i++) {
        output << i << "," << hub_combined[i] << "," << authority_combined[i] << "\n";
    }

    output.close();
}


int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <input_graph> <output_pagerank_file> <output_hub_auth_file>\n";
        return 1;
    }

    const char* input_file = argv[1];
    const char* output_pagerank_file = argv[2];
    const char* output_hub_auth_file = argv[3];

    igraph_t graph;
    igraph_vector_t weights1, weights2;  
    igraph_vector_init(&weights1, 0);
    igraph_vector_init(&weights2, 0);
    if (!read_weighted_graph(input_file, &graph, &weights1, &weights2)) {
        cerr << "Errore nella lettura del grafo pesato.\n";
        return 1;
    }

    vector<double> pagerank_weight1;
    vector<double> hub_combined, authority_combined;
    compute_pagerank(&graph, &weights1, pagerank_weight1);
    write_pagerank_to_csv(output_pagerank_file, &graph, pagerank_weight1);
    compute_hub_and_authority_log_sum(&graph, &weights1, &weights2, hub_combined, authority_combined);
    write_hub_and_authority_log_sum_to_csv(output_hub_auth_file, &graph, hub_combined, authority_combined);
    igraph_vector_destroy(&weights1);
    igraph_vector_destroy(&weights2);
    igraph_destroy(&graph);

    cout << "Analysis completed. Results saved in " << output_pagerank_file << " and " << output_hub_auth_file << endl;
    return 0;
}
