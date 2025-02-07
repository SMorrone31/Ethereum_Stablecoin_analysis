/*Questo codice calcola la reciprocità del grafo diretto pesato del chunk temporale. 
Ricordiamo che la reciprocità misura la probabilità che, dato un arco (u->v), esista anche l'arco opposto (v->u).

Utilizziamo la lista di archi per un accesso e lettura più veloce, dato che abbiamo bisogno solo di sapere se esiste un arco tra due nodi.  */

#include <igraph.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <graph_file> <output_file>" << endl;
        return 1;
    }

    const char* graph_file = argv[1];
    const char* output_file = argv[2];

    igraph_t graph;
    igraph_real_t reciprocity;
    FILE* file = fopen(graph_file, "r");
    if (file == nullptr) {
        cerr << "Error opening file " << graph_file << endl;
        return 1;
    }

    igraph_read_graph_edgelist(&graph, file, 0, IGRAPH_DIRECTED);

    fclose(file);

    igraph_reciprocity(&graph, &reciprocity, 0, IGRAPH_RECIPROCITY_DEFAULT);

    ofstream out(output_file, ios::app);
    if (!out.is_open()) {
        cerr << "Error opening output file " << output_file << endl;
        return 1;
    }
    out << reciprocity << endl;
    out.close();
    igraph_destroy(&graph);

    return 0;
}
