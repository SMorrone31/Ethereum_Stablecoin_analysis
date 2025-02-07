/*Questo codice calcola i gradi e le forze dei nodi della componente WCC più grande*/

/*#include <igraph.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "graph_utils.hpp"

using namespace std;

int extract_giant_component(igraph_t* graph, igraph_t* largest_component, igraph_vector_int_t* node_mapping) {
    igraph_vector_int_t membership, csize;
    int res = IGRAPH_SUCCESS;
    if (igraph_vector_int_init(&membership, 0) != IGRAPH_SUCCESS ||
        igraph_vector_int_init(&csize, 0) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'inizializzazione dei vettori di membership o csize\n";
        igraph_vector_int_destroy(&membership);
        igraph_vector_int_destroy(&csize);
        return IGRAPH_FAILURE;
    }
    res = igraph_connected_components(graph, &membership, &csize, nullptr, IGRAPH_WEAK);
    if (res != IGRAPH_SUCCESS) {
        cerr << "Errore nel calcolo delle componenti connesse\n";
        igraph_vector_int_destroy(&membership);
        igraph_vector_int_destroy(&csize);
        return res;
    }

    int largest_comp_id = igraph_vector_int_which_max(&csize);
    if (igraph_vector_int_init(node_mapping, 0) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'inizializzazione di node_mapping\n";
        igraph_vector_int_destroy(&membership);
        igraph_vector_int_destroy(&csize);
        return IGRAPH_FAILURE;
    }

    for (int i = 0; i < igraph_vcount(graph); ++i) {
        if (VECTOR(membership)[i] == largest_comp_id) {
            igraph_vector_int_push_back(node_mapping, i);  
        }
    }

    igraph_vs_t vs;
    igraph_vs_vector(&vs, node_mapping);
    res = igraph_induced_subgraph(graph, largest_component, vs, IGRAPH_SUBGRAPH_AUTO);
    igraph_vs_destroy(&vs);
    igraph_vector_int_destroy(&membership);
    igraph_vector_int_destroy(&csize);

    return res;
}
int extract_subgraph_weights(const igraph_t* graph, const igraph_t* subgraph,
                             const igraph_vector_t* original_weights_num_trans, const igraph_vector_t* original_weights_sum_values,
                             igraph_vector_t* subgraph_weights_num_trans, igraph_vector_t* subgraph_weights_sum_values,
                             const igraph_vector_int_t* node_mapping) {

    if (igraph_vector_init(subgraph_weights_num_trans, igraph_ecount(subgraph)) != IGRAPH_SUCCESS ||
        igraph_vector_init(subgraph_weights_sum_values, igraph_ecount(subgraph)) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'inizializzazione dei vettori per i pesi del sottografo\n";
        return IGRAPH_FAILURE;
    }

    for (int i = 0; i < igraph_ecount(subgraph); ++i) {
        igraph_integer_t from_sub, to_sub;
        igraph_edge(subgraph, i, &from_sub, &to_sub);

        igraph_integer_t from_orig = VECTOR(*node_mapping)[from_sub];
        igraph_integer_t to_orig = VECTOR(*node_mapping)[to_sub];

        igraph_integer_t edge_id;
        int res = igraph_get_eid(graph, &edge_id, from_orig, to_orig, IGRAPH_DIRECTED, 0);

        if (res == IGRAPH_SUCCESS) {
            double num_trans = VECTOR(*original_weights_num_trans)[edge_id];
            double value_trans = VECTOR(*original_weights_sum_values)[edge_id];

            VECTOR(*subgraph_weights_num_trans)[i] = (abs(num_trans) < 1e-10) ? 0.0 : num_trans;
            VECTOR(*subgraph_weights_sum_values)[i] = (abs(value_trans) < 1e-10) ? 0.0 : value_trans;

        } else {
            cerr << "Errore: arco (" << from_orig << ", " << to_orig << ") non trovato nel grafo originale.\n";
            VECTOR(*subgraph_weights_num_trans)[i] = 0.0;
            VECTOR(*subgraph_weights_sum_values)[i] = 0.0;
        }
    }

    return IGRAPH_SUCCESS;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <graph_file> <output_file>\n";
        return 1;
    }

    igraph_t graph, largest_component;
    igraph_vector_t weights_num_trans, weights_sum_values;
    igraph_vector_int_t node_mapping;

    if (igraph_vector_init(&weights_num_trans, 0) != IGRAPH_SUCCESS ||
        igraph_vector_init(&weights_sum_values, 0) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'inizializzazione dei vettori dei pesi\n";
        return 1;
    }

    if (!read_weighted_graph(argv[1], &graph, &weights_num_trans, &weights_sum_values)) {
        cerr << "Errore nella lettura del grafo pesato\n";
        igraph_vector_destroy(&weights_num_trans);
        igraph_vector_destroy(&weights_sum_values);
        return 1;
    }

    if (extract_giant_component(&graph, &largest_component, &node_mapping) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'estrazione della WCC più grande\n";
        igraph_vector_destroy(&weights_num_trans);
        igraph_vector_destroy(&weights_sum_values);
        igraph_destroy(&graph);
        return 1;
    }

    igraph_vector_t subgraph_weights_num_trans, subgraph_weights_sum_values;
    if (extract_subgraph_weights(&graph, &largest_component, &weights_num_trans, &weights_sum_values, 
                             &subgraph_weights_num_trans, &subgraph_weights_sum_values, &node_mapping) != IGRAPH_SUCCESS) {
        cerr << "Errore nell'estrazione dei pesi del sottografo\n";
        igraph_vector_destroy(&weights_num_trans);
        igraph_vector_destroy(&weights_sum_values);
        igraph_vector_int_destroy(&node_mapping);
        igraph_destroy(&largest_component);
        igraph_destroy(&graph);
        return 1;
    }


    int num_nodes = igraph_vcount(&largest_component);
    vector<double> in_degree_trans(num_nodes, 0.0), out_degree_trans(num_nodes, 0.0);
    vector<double> in_strength_value(num_nodes, 0.0), out_strength_value(num_nodes, 0.0);

    for (int i = 0; i < igraph_ecount(&largest_component); ++i) {
        igraph_integer_t from, to;
        igraph_edge(&largest_component, i, &from, &to);

        double num_trans = VECTOR(subgraph_weights_num_trans)[i];
        double value_trans = VECTOR(subgraph_weights_sum_values)[i];

        out_degree_trans[from] += num_trans;
        in_degree_trans[to] += num_trans;
        out_strength_value[from] += value_trans;
        in_strength_value[to] += value_trans;
    }

    ofstream output_file(argv[2]);
    if (!output_file.is_open()) {
        cerr << "Errore nell'apertura del file CSV di output\n";
        igraph_vector_destroy(&weights_num_trans);
        igraph_vector_destroy(&weights_sum_values);
        igraph_vector_destroy(&subgraph_weights_num_trans);
        igraph_vector_destroy(&subgraph_weights_sum_values);
        igraph_vector_int_destroy(&node_mapping);
        igraph_destroy(&largest_component);
        igraph_destroy(&graph);
        return 1;
    }

    output_file << "Node,In-Degree,Out-Degree,In-Strength,Out-Strength\n";
    for (int i = 0; i < num_nodes; ++i) {
        output_file << i << "," << in_degree_trans[i] << "," << out_degree_trans[i] << ","
                    << in_strength_value[i] << "," << out_strength_value[i] << "\n";
    }
    output_file.close();

    igraph_vector_destroy(&weights_num_trans);
    igraph_vector_destroy(&weights_sum_values);
    igraph_vector_destroy(&subgraph_weights_num_trans);
    igraph_vector_destroy(&subgraph_weights_sum_values);
    igraph_vector_int_destroy(&node_mapping);
    igraph_destroy(&largest_component);
    igraph_destroy(&graph);

    cout << "Analisi completata. Risultati salvati in " << argv[2] << endl;
    return 0;
}
*/