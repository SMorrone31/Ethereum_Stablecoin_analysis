/*Questo codice calcola il coefficiente di clustering e la densità per ogni chunk temporale. 
Non tenendo conto dei pesi, i grafi sono letti come liste di archi, rimuovendo quindi i self-loop e i multiarchi. */

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <igraph.h>
#include <vector>

using namespace std;
using namespace std::chrono;

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <graph_dir> <clustering_output_file> <density_output_file> <chunk_files...>\n";
        return 1;
    }
    auto start = high_resolution_clock::now();
    
    FILE *clustering_output_file = fopen(argv[2], "w");
    if (!clustering_output_file) {
        cerr << "Error: could not open clustering output file!\n";
        return 1;
    }
    fprintf(clustering_output_file, "chunk,clustering_coefficient\n");

    FILE *density_output_file = fopen(argv[3], "w");
    if (!density_output_file) {
        cerr << "Error: could not open density output file!\n";
        fclose(clustering_output_file);
        return 1;
    }

    fprintf(density_output_file, "chunk,density\n");

    for (int i = 4; i < argc; i++) {
        FILE *input_file = fopen(argv[i], "r");

        if (!input_file) {
            cerr << "Error: could not open graph file " << argv[i] << "!\n";
            continue;
        }

        igraph_t graph;
        // legge la lista di archi
        igraph_read_graph_edgelist(&graph, input_file, 0, 0);
        fclose(input_file);
        igraph_simplify(&graph, 1, 1, NULL);
        igraph_real_t clustering_coefficient;
        // calcolo del coefficiente di clustering con la funzione di Igraph
        igraph_transitivity_undirected(&graph, &clustering_coefficient, IGRAPH_TRANSITIVITY_ZERO);
        igraph_real_t density;
        // calcola la densità ignorando i self-loops
        igraph_density(&graph, &density, IGRAPH_NO_LOOPS);

        // estrazione del nome del chunk e salvataggio dei risultati
        string chunk_name = argv[i];
        size_t pos = chunk_name.find_last_of("/\\");

        if (pos != string::npos) {
            chunk_name = chunk_name.substr(pos + 1); 
        }

        chunk_name = chunk_name.substr(0, chunk_name.find(".txt"));
        string prefix = "edgelist_";

        if (chunk_name.find(prefix) == 0) {
            chunk_name = chunk_name.substr(prefix.length()); 
        }

        fprintf(clustering_output_file, "%s,%.6f\n", chunk_name.c_str(), clustering_coefficient);
        fprintf(density_output_file, "%s,%.6f\n", chunk_name.c_str(), density);
        igraph_destroy(&graph);

    }

    fclose(clustering_output_file);
    fclose(density_output_file);
    auto end = high_resolution_clock::now();
    auto elapsed = duration_cast<nanoseconds>(end - start);
    cout << "Execution completed in " << elapsed.count() << " ns\n";

    return 0;
}
