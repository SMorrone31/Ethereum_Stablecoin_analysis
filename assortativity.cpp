/*Questo codice calcola l'assortatività del grafo temporale del chunk.
Ricordiamo che l'assortatività misura la tendenza dei nodi a connettersi con altri nodi simili in termini in base a delle statistiche comuni. In questo caso in termini di grado e forza. 
Vengono calcolati 4 valori: assortatività per gradi e forze in ingresso e in uscita ovvero:
1) assortatività per in-degree: se i nodi con alto grado in ingresso si connettono tra di loro
2) assortatività per out-degree: se i nodi con alto grado in uscita si connettono tra di loro
3) assortatività per in-strength: se i nodi con elevata somma di pesi in ingresso si connettono tra di loro
4) assortatività per out-strength: se i nodi con elevata somma di pesi in uscita si connettono tra di loro
*/

#include <igraph.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "graph_utils.hpp"

using namespace std;

// calcolo dell'assortatività tramite le funzioni offerte da Igraph 
void calculate_and_save_assortativity(const igraph_t* graph, const igraph_vector_t* in_strength, const igraph_vector_t* out_strength, const char* output_file) {
    igraph_real_t assortativity_in_degree, assortativity_out_degree;
    igraph_real_t assortativity_in_strength, assortativity_out_strength;
    igraph_assortativity_degree(graph, &assortativity_in_degree, IGRAPH_IN);
    igraph_assortativity_degree(graph, &assortativity_out_degree, IGRAPH_OUT);
    igraph_assortativity(graph, in_strength, NULL, &assortativity_in_strength, IGRAPH_IN, 0);
    igraph_assortativity(graph, out_strength, NULL, &assortativity_out_strength, IGRAPH_OUT, 0);
    ofstream assortativity_outfile(output_file);
    if (!assortativity_outfile.is_open()) {
        cerr << "Error: could not open output file for assortativity: " << output_file << endl;
        return;
    }

    assortativity_outfile << "InDegreeAssortativity,OutDegreeAssortativity,InStrengthAssortativity,OutStrengthAssortativity\n";
    assortativity_outfile << assortativity_in_degree << "," << assortativity_out_degree << ","
                          << assortativity_in_strength << "," << assortativity_out_strength << "\n";

    assortativity_outfile.close();
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <graph_file> <output_assortativity_file> <output_degrees_file>\n";
        return 1;
    }

    // lettura del grafo pesato 
    igraph_t graph;
    if (!read_global_graph(argv[1], &graph, &edge_weights)) {
        return 1;
    }

    igraph_vector_init(&in_strength, igraph_vcount(&graph));
    igraph_vector_init(&out_strength, igraph_vcount(&graph));
    calculate_and_save_assortativity(&graph, &in_strength, &out_strength, argv[2]);
    igraph_vector_destroy(&edge_weights);
    igraph_vector_destroy(&in_strength);
    igraph_vector_destroy(&out_strength);
    igraph_destroy(&graph);

    return 0;
}
