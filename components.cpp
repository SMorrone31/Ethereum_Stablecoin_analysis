/*Questo codice calcola la componente gigante del grafo per ogni chunk temporale. 
Individua la SCC e la WCC. Inoltre sulla componente gigante calcola anche il numero di nodi e archi, la transitività e la densità.  */

#include <igraph.h>
#include <iostream>
#include <fstream>
#include <filesystem>  
#include <omp.h>

using namespace std;

// estrae il nome del chunk dal percorso: utlizziamo la lista di archi
string extract_chunk_name(const string& file_path) {
    filesystem::path path(file_path);
    string stem = path.stem().string(); 
    string prefix = "edgelist_";  
    if (stem.find(prefix) == 0) {
        return stem.substr(prefix.length());  
    }
    return stem;  
}

// funzione che trova la SCC e WCC.
// trova le componenti connesse con igraph_clusters()
// determina la più grande con OpenMP (reduction(max:largest_size))
// crea un vettore dei nodi appartenenti alla componente gigante
// estrae il sottografo corrispondente con igraph_induced_subgraph()
void calculate_giant_component_and_coverage(igraph_t* graph, igraph_connectedness_t mode, int &largest_size, double &coverage, igraph_t* largest_component) {
    igraph_vector_int_t membership, csize;
    igraph_integer_t num_components;
    igraph_vector_int_init(&membership, 0);
    igraph_vector_int_init(&csize, 0);

    if (igraph_clusters(graph, &membership, &csize, &num_components, mode) != IGRAPH_SUCCESS) {
        cerr << "Errore nel calcolo delle componenti\n";
        igraph_vector_int_destroy(&membership);
        igraph_vector_int_destroy(&csize);
        return;
    }

    largest_size = 0;
    int largest_comp_id = -1;

    #pragma omp parallel for reduction(max:largest_size)
    for (int i = 0; i < num_components; i++) {
        if (VECTOR(csize)[i] > largest_size) {
            largest_size = VECTOR(csize)[i];
            largest_comp_id = i;
        }
    }

    coverage = (double)largest_size / igraph_vcount(graph);
    igraph_vector_int_t comp_vertices;
    igraph_vector_int_init(&comp_vertices, largest_size);
    int idx = 0;

    #pragma omp parallel for shared(idx)
    for (int i = 0; i < igraph_vcount(graph); i++) {
        if (VECTOR(membership)[i] == largest_comp_id) {
            #pragma omp critical
            VECTOR(comp_vertices)[idx++] = i;
        }
    }

    igraph_vs_t vs;
    igraph_vs_vector(&vs, &comp_vertices);
    igraph_induced_subgraph(graph, largest_component, vs, IGRAPH_SUBGRAPH_AUTO);
    igraph_vector_int_destroy(&membership);
    igraph_vector_int_destroy(&csize);
    igraph_vector_int_destroy(&comp_vertices);

}

// calcola le statistiche sulla componente gigante
void calculate_giant_statistics(igraph_t* largest_component, int &num_nodes, int &num_edges, double &transitivity, double &density) {
    num_nodes = igraph_vcount(largest_component);
    num_edges = igraph_ecount(largest_component);
    igraph_transitivity_undirected(largest_component, &transitivity, IGRAPH_TRANSITIVITY_NAN);
    igraph_density(largest_component, &density, 0);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <graph_file> <giant_output_file> <giant_stats_file>\n";
        return 1;
    }

    igraph_t graph;
    FILE* input_file = fopen(argv[1], "r");

    if (!input_file) {
        cerr << "Errore nell'apertura del file del grafo\n";
        return 1;
    }
    if (igraph_read_graph_edgelist(&graph, input_file, 0, IGRAPH_DIRECTED) != IGRAPH_SUCCESS) {
        cerr << "Errore nella lettura del grafo\n";
        fclose(input_file);
        return 1;
    }

    fclose(input_file);

    int largest_scc_size = 0, largest_wcc_size = 0;
    double coverage_scc = 0.0, coverage_wcc = 0.0;
    igraph_t largest_component;
    calculate_giant_component_and_coverage(&graph, IGRAPH_STRONG, largest_scc_size, coverage_scc, &largest_component);
    int num_nodes_scc, num_edges_scc;
    double transitivity_scc, density_scc;
    calculate_giant_statistics(&largest_component, num_nodes_scc, num_edges_scc, transitivity_scc, density_scc);
    igraph_destroy(&largest_component);
    calculate_giant_component_and_coverage(&graph, IGRAPH_WEAK, largest_wcc_size, coverage_wcc, &largest_component);
    int num_nodes_wcc, num_edges_wcc;
    double transitivity_wcc, density_wcc;
    calculate_giant_statistics(&largest_component, num_nodes_wcc, num_edges_wcc, transitivity_wcc, density_wcc);
    igraph_destroy(&largest_component);
    string chunk_name = extract_chunk_name(argv[1]);
    ofstream giant_file(argv[2], ios::app);

    if (!giant_file.is_open()) {
        cerr << "Errore nell'apertura del file delle Giant Components\n";
        igraph_destroy(&graph);
        return 1;
    }

    giant_file.seekp(0, ios::end);
    if (giant_file.tellp() == 0) {
        giant_file << "chunk,LargestSizeSCC,LargestSizeWCC,CoverageSCC,CoverageWCC\n";
    }

    giant_file << chunk_name << "," << largest_scc_size << "," << largest_wcc_size << "," << coverage_scc << "," << coverage_wcc << "\n";
    giant_file.close();

    ofstream stats_file(argv[3], ios::app);
    if (!stats_file.is_open()) {
        cerr << "Errore nell'apertura del file delle statistiche della Giant Component\n";
        igraph_destroy(&graph);
        return 1;
    }

    stats_file.seekp(0, ios::end);
    if (stats_file.tellp() == 0) {
        stats_file << "chunk,SccNodes,WccNodes,SccEdges,WccEdges,SccCC,WccCC,SccDensity,WccDensity\n";
    }

    stats_file << chunk_name << "," 
               << num_nodes_scc << "," << num_nodes_wcc << ","
               << num_edges_scc << "," << num_edges_wcc << ","
               << transitivity_scc << "," << transitivity_wcc << ","
               << density_scc << "," << density_wcc << "\n";

    stats_file.close();
    
    igraph_destroy(&graph);

    return 0;
}