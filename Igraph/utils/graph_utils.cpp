/*Libreria per la lettura dei grafi in modi diversi come: grafo diretto pesato, grafo diretto non pesato, grafo globale con timestamp ecc.*/

#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <igraph.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>  

bool read_weighted_graph(const char* filename, igraph_t* graph, igraph_vector_t* num_trans_weights, igraph_vector_t* value_weights) {

    std::ifstream graph_file(filename);
    if (!graph_file.is_open()) {
        std::cerr << "Errore nell'apertura del file del grafo pesato\n";
        return false;
    }

    // vettori di archi e pesi
    igraph_vector_int_t edges;
    igraph_vector_int_init(&edges, 0);
    igraph_vector_init(num_trans_weights, 0);
    if (value_weights != nullptr) {
        igraph_vector_init(value_weights, 0);  // se value_weights non è nullptr
    }

    double from_d, to_d;
    double numTrans, valueTrans;
    std::string line;

    while (getline(graph_file, line)) {
        std::istringstream iss(line);

        // arrontondo quando leggo
        if (!(iss >> from_d >> to_d >> numTrans >> valueTrans)) {
            std::cerr << "Errore nel formato del file del grafo nella riga: " << line << std::endl;
            igraph_vector_int_destroy(&edges);
            igraph_vector_destroy(num_trans_weights);
            if (value_weights != nullptr) {
                igraph_vector_destroy(value_weights);
            }
            return false;
        }

        int from = static_cast<int>(std::round(from_d));
        int to = static_cast<int>(std::round(to_d));

        // archi e pesi ai rispettivi vettori
        igraph_vector_int_push_back(&edges, from);
        igraph_vector_int_push_back(&edges, to);
        igraph_vector_push_back(num_trans_weights, numTrans);
        if (value_weights != nullptr) {
            igraph_vector_push_back(value_weights, valueTrans);
        }
    }

    graph_file.close();

    // grafo diretto dai vettori degli archi
    if (igraph_create(graph, &edges, 0, IGRAPH_DIRECTED) != IGRAPH_SUCCESS) {
        std::cerr << "Errore nella creazione del grafo\n";
        igraph_vector_int_destroy(&edges);
        igraph_vector_destroy(num_trans_weights);
        if (value_weights != nullptr) {
            igraph_vector_destroy(value_weights);
        }
        return false;
    }

    // Verifica la direzionalità del grafo
    if (!igraph_is_directed(graph)) {
        std::cerr << "Errore: il grafo non è diretto!" << std::endl;
        igraph_destroy(graph);
        igraph_vector_int_destroy(&edges);
        igraph_vector_destroy(num_trans_weights);
        if (value_weights != nullptr) {
            igraph_vector_destroy(value_weights);
        }
        return false;
    }

    igraph_vector_int_destroy(&edges);

    return true;
}

bool read_global_graph(const char* filename, igraph_t* graph, igraph_vector_t* weights) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;
    std::vector<double> weight_values;

    while (getline(file, line)) {
        std::istringstream iss(line);
        int from, to;
        double value;
        if (!(iss >> from >> to >> value)) {
            std::cerr << "Errore nella lettura della linea: " << line << std::endl;
            return false;
        }
        edges.emplace_back(from, to);
        weight_values.push_back(value);
    }

    igraph_vector_int_t edge_vector;
    igraph_vector_int_init(&edge_vector, edges.size() * 2);

    for (size_t i = 0; i < edges.size(); ++i) {
        VECTOR(edge_vector)[2 * i] = edges[i].first;
        VECTOR(edge_vector)[2 * i + 1] = edges[i].second;
    }

    igraph_create(graph, &edge_vector, 0, IGRAPH_DIRECTED);
    igraph_vector_int_destroy(&edge_vector);

    igraph_vector_init(weights, weight_values.size());
    for (size_t i = 0; i < weight_values.size(); ++i) {
        VECTOR(*weights)[i] = weight_values[i];
    }

    return true;
}

bool read_unweighted_graph(const std::string& filename, igraph_t* graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;

    while (getline(file, line)) {
        std::istringstream iss(line);
        int from, to;
        if (!(iss >> from >> to)) {
            std::cerr << "Errore nella lettura della linea: " << line << std::endl;
            return false;
        }
        edges.emplace_back(from, to);
    }

    igraph_vector_int_t edge_vector;
    igraph_vector_int_init(&edge_vector, edges.size() * 2);

    for (size_t i = 0; i < edges.size(); ++i) {
        VECTOR(edge_vector)[2 * i] = edges[i].first;
        VECTOR(edge_vector)[2 * i + 1] = edges[i].second;
    }

    igraph_create(graph, &edge_vector, 0, IGRAPH_DIRECTED);
    igraph_vector_int_destroy(&edge_vector);

    return true;
}

// Funzione per leggere il grafo con valore delle transazioni e timestamp
bool read_global_graph_with_timestamp(const std::string& filename, igraph_t* graph, igraph_vector_t* weights, std::vector<std::string>& timestamps) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Errore nell'apertura del file: " << filename << std::endl;
        return false;
    }

    std::string line;
    std::vector<std::pair<int, int>> edges;
    std::vector<double> weight_values;
    timestamps.clear();  // Pulisce i timestamp

    while (getline(file, line)) {
        std::istringstream iss(line);
        int from, to;
        double value;
        std::string timestamp;

        // Controlla se il parsing è riuscito
        if (!(iss >> from >> to >> value >> timestamp)) {
            std::cerr << "Errore nella lettura della linea: " << line << std::endl;
            return false;
        }

        edges.emplace_back(from, to);
        weight_values.push_back(value);
        timestamps.push_back(timestamp);
    }

    // Controlla se sono stati letti dati validi
    if (edges.empty() || weight_values.empty() || timestamps.empty()) {
        std::cerr << "Errore: il file sembra essere vuoto o mal formattato." << std::endl;
        return false;
    }

    // Inizializza il grafo con i nodi e gli archi
    igraph_vector_int_t edge_vector;
    igraph_vector_int_init(&edge_vector, edges.size() * 2);  // Due valori per arco: da -> a

    for (size_t i = 0; i < edges.size(); ++i) {
        VECTOR(edge_vector)[2 * i] = edges[i].first;
        VECTOR(edge_vector)[2 * i + 1] = edges[i].second;
    }

    // Crea il grafo diretto
    if (igraph_create(graph, &edge_vector, 0, IGRAPH_DIRECTED) != IGRAPH_SUCCESS) {
        std::cerr << "Errore nella creazione del grafo." << std::endl;
        igraph_vector_int_destroy(&edge_vector);
        return false;
    }
    igraph_vector_int_destroy(&edge_vector);

    // Inizializza il vettore dei pesi
    igraph_vector_init(weights, weight_values.size());
    for (size_t i = 0; i < weight_values.size(); ++i) {
        VECTOR(*weights)[i] = weight_values[i];
    }

    return true;
}


#endif
