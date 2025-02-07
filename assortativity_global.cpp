/*Il codice calcola l'assortatività del grafo globale di tutte le transazioni di una Stablecoin. 
Il grafo come sappiamo è diretto e pesato dove i pesi sono il timestamp e il valore trasferito. Ricordiamo che sono presenti i multiarchi nel grafo globale. 
Detto ciò ricordiamo anche che l'assortatività misura quanto i nodi con valori simili di un certo attributo tendono a connettersi tra loro. 
Questo è stato utile per comprendere se nodi con alto grado e forza si connettono con nodi con alto grado e forza o meno, quoindi possiamo studiare in parte il fenomeno "The rich get richer and the poor get poorer".
Quindi gli attributi utilizzati per il calcolo sono appunto il grado e la forza in ingresso e in uscita di ogni nodo nel grafo globale, calcolati precedentemente.*/

#include <igraph.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "graph_utils.hpp"

using namespace std;

// qui leggo dal file CSV dei gradi e delle forze precedentemente calcolati, e memorizzo ogni valore in un vettore diverso di tipo igraph_vector_t.
bool load_degrees_and_strengths(const char* filename, igraph_vector_t* in_degree, igraph_vector_t* out_degree, igraph_vector_t* in_strength, igraph_vector_t* out_strength) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
        return false;
    }

    std::string line;
    getline(file, line); 
    igraph_vector_init(in_degree, 0);
    igraph_vector_init(out_degree, 0);
    igraph_vector_init(in_strength, 0);
    igraph_vector_init(out_strength, 0);
    while (getline(file, line)) {
        std::istringstream iss(line);
        int node;
        double degree_in, degree_out, strength_in, strength_out;
        char comma; 

        if (!(iss >> node >> comma >> degree_in >> comma >> degree_out >> comma >> strength_in >> comma >> strength_out)) {
            std::cerr << "Errore nella lettura della linea: " << line << std::endl;
            return false;
        }

        igraph_vector_push_back(in_degree, degree_in);
        igraph_vector_push_back(out_degree, degree_out);
        igraph_vector_push_back(in_strength, strength_in);
        igraph_vector_push_back(out_strength, strength_out);
    }
    file.close();
    return true;
}

// qui calcolo l'assortatività basata sul grado e la forza utilizzando una delle funzioni offerte da Igraph.
void calculate_and_save_assortativity(const igraph_t* graph, const igraph_vector_t* in_degree, const igraph_vector_t* out_degree, const igraph_vector_t* in_strength, const igraph_vector_t* out_strength, const char* output_file) {
    igraph_real_t assortativity_in_degree, assortativity_out_degree;
    igraph_real_t assortativity_in_strength, assortativity_out_strength;

    // Assortatività basata sui gradi
    igraph_assortativity(graph, in_degree, NULL, &assortativity_in_degree, true, true);
    igraph_assortativity(graph, out_degree, NULL, &assortativity_out_degree, true, true);

    // Assortatività basata sulla forza
    igraph_assortativity(graph, in_strength, NULL, &assortativity_in_strength, true, true);
    igraph_assortativity(graph, out_strength, NULL, &assortativity_out_strength, true, true);

    ofstream assortativity_outfile(output_file);
    if (!assortativity_outfile.is_open()) {
        cerr << "Errore: impossibile aprire il file di output per l'assortatività: " << output_file << endl;
        return;
    }
    assortativity_outfile << "InDegreeAssortativity,OutDegreeAssortativity,InStrengthAssortativity,OutStrengthAssortativity\n";
    assortativity_outfile << assortativity_in_degree << "," << assortativity_out_degree << ","
                          << assortativity_in_strength << "," << assortativity_out_strength << "\n";
    assortativity_outfile.close();
}

int main(int argc, char** argv) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <graph_file> <degree_strength_file> <output_assortativity_file>\n";
        return 1;
    }
    igraph_t graph;
    igraph_vector_t empty_weights;
    igraph_vector_init(&empty_weights, 0);
    // read_weighted_graph: carica il grafo globale pesato. E' una funzione che ho costruito io e ce ne ho fatto una libreria: "graph_utils.hpp"
    if (!read_weighted_graph(argv[1], &graph, &empty_weights, nullptr)) {
        return 1;
    }
    igraph_vector_destroy(&empty_weights);
    igraph_vector_t in_degree, out_degree, in_strength, out_strength;
    if (!load_degrees_and_strengths(argv[2], &in_degree, &out_degree, &in_strength, &out_strength)) {
        igraph_destroy(&graph);
        return 1;
    }
    calculate_and_save_assortativity(&graph, &in_degree, &out_degree, &in_strength, &out_strength, argv[3]);
    igraph_vector_destroy(&in_degree);
    igraph_vector_destroy(&out_degree);
    igraph_vector_destroy(&in_strength);
    igraph_vector_destroy(&out_strength);
    igraph_destroy(&graph);

    return 0;
}
