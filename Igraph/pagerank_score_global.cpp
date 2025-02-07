/*Il codice elabora il grafo globale per calcolare score di centralità come PageRank, Authority e Hub Score. 
In più c'è il rilevamento della comunità e il calcolo della microvelociy (non corretto).

Utilizzo le funzioni offerte da Igraph per il calcolo di questi score*/

#include <igraph.h>
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <cstring>
#include <cmath>
#include "utils/graph_utils.hpp"

using namespace std;

void compute_pagerank(const igraph_t* graph, igraph_vector_t* weights, vector<double>& pagerank_result) {
    igraph_vector_t pagerank_vector;
    igraph_real_t damping = 0.85;

    if (igraph_vcount(graph) == 0 || igraph_ecount(graph) == 0) {
        cerr << "Errore: il grafo è vuoto, impossibile calcolare il PageRank.\n";
        return;
    }

    igraph_vector_init(&pagerank_vector, igraph_vcount(graph));
    igraph_vs_t all_vertices;
    igraph_vs_all(&all_vertices);
    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);

    int status = igraph_personalized_pagerank(graph, IGRAPH_PAGERANK_ALGO_PRPACK, &pagerank_vector, nullptr, all_vertices, IGRAPH_DIRECTED, damping, nullptr, weights, &options);
    if (status != IGRAPH_SUCCESS) {
        cerr << "Errore nel calcolo del PageRank.\n";
        igraph_vector_destroy(&pagerank_vector);
        igraph_vs_destroy(&all_vertices);
        return;
    }

    for (int i = 0; i < igraph_vcount(graph); i++) {
        pagerank_result.push_back(VECTOR(pagerank_vector)[i]);
    }

    igraph_vector_destroy(&pagerank_vector);
    igraph_vs_destroy(&all_vertices);
}

void write_pagerank_to_csv(const char* output_file, const igraph_t* graph, const vector<double>& pagerank) {
    if (strcmp(output_file, "null") != 0) {
        ofstream output(output_file);
        output << "NodeId,PageRank\n";
        for (int i = 0; i < igraph_vcount(graph); i++) {
            output << i << "," << pagerank[i] << "\n";
        }
        output.close();
    }
}

// Funzione per calcolare Hub e Authority con somma logaritmica
void compute_hub_and_authority_log_sum(const igraph_t* graph, igraph_vector_t* weights1, igraph_vector_t* weights2, vector<double>& hub_combined, vector<double>& authority_combined) {
    igraph_vector_t hub_scores1, authority_scores1, hub_scores2, authority_scores2;
    igraph_real_t eigenvalue;
    igraph_vector_init(&hub_scores1, igraph_vcount(graph));
    igraph_vector_init(&authority_scores1, igraph_vcount(graph));
    igraph_vector_init(&hub_scores2, igraph_vcount(graph));
    igraph_vector_init(&authority_scores2, igraph_vcount(graph));

    igraph_arpack_options_t options;
    igraph_arpack_options_init(&options);

    igraph_hub_and_authority_scores(graph, &hub_scores1, &authority_scores1, &eigenvalue, 1, weights1, &options);
    igraph_hub_and_authority_scores(graph, &hub_scores2, &authority_scores2, &eigenvalue, 1, weights2, &options);

    for (int i = 0; i < igraph_vcount(graph); i++) {
        hub_combined.push_back(log(1 + VECTOR(hub_scores1)[i]) + log(1 + VECTOR(hub_scores2)[i]));
        authority_combined.push_back(log(1 + VECTOR(authority_scores1)[i]) + log(1 + VECTOR(authority_scores2)[i]));
    }

    igraph_vector_destroy(&hub_scores1);
    igraph_vector_destroy(&authority_scores1);
    igraph_vector_destroy(&hub_scores2);
    igraph_vector_destroy(&authority_scores2);
}

// Funzione per rilevare le comunità usando Louvain
/*void compute_community_louvain_unweighted(const igraph_t* graph, vector<int>& membership_result) {
    igraph_t undirected_graph;
    igraph_copy(&undirected_graph, graph);
    igraph_to_undirected(&undirected_graph, IGRAPH_TO_UNDIRECTED_COLLAPSE, nullptr);

    igraph_vector_int_t membership;
    igraph_vector_int_init(&membership, igraph_vcount(&undirected_graph));

    igraph_community_multilevel(&undirected_graph, nullptr, 1.0, &membership, nullptr, nullptr);

    for (int i = 0; i < igraph_vcount(&undirected_graph); i++) {
        membership_result.push_back(VECTOR(membership)[i]);
    }

    igraph_vector_int_destroy(&membership);
    igraph_destroy(&undirected_graph);
}*/

/*void compute_microvelocity(const igraph_t* graph, const vector<string>& timestamps, 
                           const igraph_vector_t* values, 
                           map<int, double>& microvelocity_in, map<int, double>& microvelocity_out) {
    map<int, vector<pair<double, double>>> holding_times_in, holding_times_out;

    // Calcola il tempo di holding e associa i valori
    for (int i = 0; i < igraph_ecount(graph); ++i) {
        int from = IGRAPH_FROM(graph, i);
        int to = IGRAPH_TO(graph, i);
        double time_from = stod(timestamps[from]);
        double time_to = stod(timestamps[to]);
        double holding_time = time_to - time_from;
        double value = VECTOR(*values)[i];

        if (holding_time > 0) {
            holding_times_in[to].emplace_back(holding_time, value);
            holding_times_out[from].emplace_back(holding_time, value);
        }
    }

    // Calcola la MicroVelocity in ingresso per ogni nodo
    for (const auto& kv : holding_times_in) {
        double total_weighted_time = 0, total_value = 0;
        for (const auto& pair : kv.second) {
            total_weighted_time += pair.second / pair.first;  // value / holding_time
            total_value += pair.second;
        }
        microvelocity_in[kv.first] = total_value > 0 ? total_weighted_time / total_value : 0.0;
    }

    // Calcola la MicroVelocity in uscita per ogni nodo
    for (const auto& kv : holding_times_out) {
        double total_weighted_time = 0, total_value = 0;
        for (const auto& pair : kv.second) {
            total_weighted_time += pair.second / pair.first;  // value / holding_time
            total_value += pair.second;
        }
        microvelocity_out[kv.first] = total_value > 0 ? total_weighted_time / total_value : 0.0;
    }
}*/


void write_hub_and_authority_log_sum_to_csv(const char* output_file, const igraph_t* graph, const vector<double>& hub_combined, const vector<double>& authority_combined) {
    if (strcmp(output_file, "null") != 0) {
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
}

/*void write_microvelocity_and_community_to_csv(const char* output_file, const igraph_t* graph, const vector<int>& community, const map<int, double>& microvelocity_in, const map<int, double>& microvelocity_out) {
    if (strcmp(output_file, "null") != 0) {
        ofstream output(output_file);
        if (!output.is_open()) {
            cerr << "Error opening output file: " << output_file << endl;
            return;
        }
        output << "NodeId,Community,MicroVelocityIn,MicroVelocityOut\n";
        for (int i = 0; i < igraph_vcount(graph); i++) {
            double mv_in = microvelocity_in.count(i) ? microvelocity_in.at(i) : 0.0;
            double mv_out = microvelocity_out.count(i) ? microvelocity_out.at(i) : 0.0;
            output << i << "," << community[i] << "," << mv_in << "," << mv_out << "\n";
        }
        output.close();
    }
}*/


int main(int argc, char** argv) {
    if (argc < 7) {
        cerr << "Usage: " << argv[0] << " <input_weighted_pagerank_graph> <input_weighted_global_graph> <input_unweighted_graph> <output_pagerank_file> <output_hub_auth_file> <output_microvelocity_community_file>\n";
        return 1;
    }

    const char* input_weighted_pagerank_file = argv[1];
    const char* input_weighted_global_file = argv[2];
    const char* input_unweighted_file = argv[3];
    const char* output_pagerank_file = argv[4];
    const char* output_hub_auth_file = argv[5];
    const char* output_microvelocity_community_file = argv[6];

    igraph_t pagerank_graph, weighted_global_graph, unweighted_graph;
    igraph_vector_t pagerank_weights, weights1, weights2;
    vector<string> timestamps;
    map<int, double> microvelocity_in, microvelocity_out;

    // PageRank: Carica grafo collassato (peso molteplicità)
    if (strcmp(input_weighted_pagerank_file, "null") != 0 && read_weighted_graph(input_weighted_pagerank_file, &pagerank_graph, &pagerank_weights, nullptr)) {
        vector<double> pagerank;
        compute_pagerank(&pagerank_graph, &pagerank_weights, pagerank);
        write_pagerank_to_csv(output_pagerank_file, &pagerank_graph, pagerank);
        igraph_destroy(&pagerank_graph);
        igraph_vector_destroy(&pagerank_weights);
    }

    // Hub e Authority: Carica grafo collassato con due pesi
    // quindi questi due score vengono calcolati per entrambi i pesi: Somma delle transazioni e molteplicità
    if (strcmp(input_weighted_global_file, "null") != 0 && read_weighted_graph(input_weighted_global_file, &weighted_global_graph, &weights1, &weights2)) {
        vector<double> hub_combined, authority_combined;
        compute_hub_and_authority_log_sum(&weighted_global_graph, &weights1, &weights2, hub_combined, authority_combined);
        write_hub_and_authority_log_sum_to_csv(output_hub_auth_file, &weighted_global_graph, hub_combined, authority_combined);
        igraph_destroy(&weighted_global_graph);
        igraph_vector_destroy(&weights1);
        igraph_vector_destroy(&weights2);
    }

    if (strcmp(input_weighted_global_file, "null") != 0 && 
        read_global_graph_with_timestamp(input_weighted_global_file, &weighted_global_graph, &weights1, timestamps)) {
        compute_microvelocity(&weighted_global_graph, timestamps, &weights1, microvelocity_in, microvelocity_out);
        write_microvelocity_and_community_to_csv(output_microvelocity_community_file, 
                                                &weighted_global_graph, 
                                                vector<int>(igraph_vcount(&weighted_global_graph), -1),  // Dummy community
                                                microvelocity_in, microvelocity_out);
        igraph_destroy(&weighted_global_graph);
        igraph_vector_destroy(&weights1);
    }


    // Community: Carica grafo non pesato multiarchi
    if (strcmp(input_unweighted_file, "null") != 0 && read_unweighted_graph(input_unweighted_file, &unweighted_graph)) {
        vector<int> community_membership;
        compute_community_louvain_unweighted(&unweighted_graph, community_membership);
        write_microvelocity_and_community_to_csv(output_microvelocity_community_file, &unweighted_graph, community_membership, microvelocity_in, microvelocity_out);
        igraph_destroy(&unweighted_graph);
    }

    cout << "Operazioni completate." << endl;
    return 0;
}
