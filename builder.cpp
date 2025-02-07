/*Questo codice si occupa di costruitre il grafo temporale collassato e pesato a partire da un file di transazioni assegnando ID univoci ai nodi e aggregando i pesi degli archi per molteplicità e valore totale delle transazioni.
I file delle transazioni sono gli n file ricavati dagli n chunk individuati durante la suddivisione per chunk mensile. */

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem> 

using namespace std;
using namespace std::chrono;

typedef unordered_map<int, int> node_map_t; // mapa un indirizzo Ethereum a un ID nodo univoco
typedef unordered_map<pair<int, int>, pair<int, double>, hash_pair> aggregated_edges_t; // mappa (From,To) alla coppia (conteggio transazioni, valore aggregato delle transazioni)

// struttura che fornisce una funzione di hashing personalizzata per permettere l'uso di undordered_map<pair<int, int>>, consentendo una ricerca efficiente degli archi.
// questa struttura permette di usare unordered_map<pair<int, int>> ottimizzando la gestine degli archi con accesso O(1) invece di O(log n). 
// infatti lo XOR "^" combina gli hash di from e to per distribuirli meglio, riducendo collisioni, mentre lo shift *<<1* assicura che (a,b) e (b,a) abbiano hash diversi, preservando la direzionalità dell'arco.
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1); 
    }
};


// funzione che verifica se un noso esiste già nella mappa
inline int contains_id(node_map_t& map, int address) {
    auto it = map.find(address);
    return ((it != map.end()) ? it->second : -1);
}

// assegna un ID univoco ai nodi, se l'indirizzo è già presente, restituisce il suo ID
inline int get_or_create_id(node_map_t& map, int address) {
    static int next_id = 0;
    int id = contains_id(map, address);
    if (id == -1) {
        id = next_id;
        map[address] = id;
        next_id++;
    }
    return id;
}

// estare il nome del chunk dal percorso del file di output, rimuovendo il prefisso se necessario
string extract_chunk_name(const string& file_path) {
    filesystem::path path(file_path);
    string stem = path.stem().string();  
    string prefix = "graph_";           
    if (stem.find(prefix) == 0) {
        return stem.substr(prefix.length()); 
    }
    return stem; 
}

// funzione che processa una riga del file delle transazioni e aggiorna la mappa degli archi aggregati. 
// quindi legge i campi, assegna un ID univoco ai nodi mittente e destinatario, incrementa il numero di transazioni e aggiorna la mappa in base al conteggio e la somma dei valori delle transazioni.
void process_line(const string& line, node_map_t& nodes, aggregated_edges_t& aggregated_edges, int& total_transactions) {
    stringstream ss(line);
    string block_id_str, contract_id_str, from_str, to_str, value_str, timestamp_str;
    int block_id, contract_id, from, to;
    double value_in_DAI;

    try {
        if (getline(ss, block_id_str, ',') && 
            getline(ss, contract_id_str, ',') && 
            getline(ss, from_str, ',') && 
            getline(ss, to_str, ',') && 
            getline(ss, value_str, ',') && 
            getline(ss, timestamp_str, ',')) {
            block_id = stoi(block_id_str);
            contract_id = stoi(contract_id_str);
            from = get_or_create_id(nodes, stoi(from_str));
            to = get_or_create_id(nodes, stoi(to_str));
            value_in_DAI = stod(value_str);
            total_transactions++;
            auto edge_key = make_pair(from, to);
            aggregated_edges[edge_key].first += 1;          
            aggregated_edges[edge_key].second += value_in_DAI;     
        } else {
            throw runtime_error("Errore nel parsing dei dati.");
        }
    } catch (const exception& e) {
        cerr << "Errore nel formato della riga dei trasferimenti: " << line << " - " << e.what() << "\n";
    }
}


int main(int argc, char** argv) {
    if (argc < 5) {
        cerr << "Usage: " << argv[0] << " <transfers_file> <graph_file> <mapping_file> <edgelist_file> <summary_file>\n";
        return 1;
    }
    auto start = high_resolution_clock::now();
    ifstream input_file_t(argv[1]);
    if (!input_file_t.is_open()) {
        cerr << "Error: could not open transfers file!\n";
        return 1;
    }
    ofstream graph_file(argv[2]);
    if (!graph_file.is_open()) {
        cerr << "Error: could not open output graph file!\n";
        return 1;
    }
    ofstream mapping_file(argv[3]);
    if (!mapping_file.is_open()) {
        cerr << "Error: could not open output mapping file!\n";
        return 1;
    }
    ofstream edgelist_file(argv[4]);
    if (!edgelist_file.is_open()) {
        cerr << "Error: could not open edgelist file!\n";
        return 1;
    }
    ofstream summary_file(argv[5], ios::app);
    if (!summary_file.is_open()) {
        cerr << "Error: could not open summary file!\n";
        return 1;
    }

    node_map_t nodes;
    aggregated_edges_t aggregated_edges;
    int total_transactions = 0;
    string line;
    if (!getline(input_file_t, line)) {
        cerr << "Error: failed to read the header line from the transfers file!\n";
        return 1;
    }
    // itera su ogni riga del file delle transazioni, processando i dati. Costruisce inoltre le strutture dati dei nodi e degli archi aggregati
    while (getline(input_file_t, line)) {
        process_line(line, nodes, aggregated_edges, total_transactions);
    }

    // ogni elemento del vettore è una tupla (from, to , molteplicità, valore totale)
    vector<tuple<int, int, int, double>> edges;
    for (const auto& it : aggregated_edges) {
        int from = it.first.first;
        int to = it.first.second;
        int count = it.second.first;
        double total_value = it.second.second;
        edges.emplace_back(from, to, count, total_value);
    }

    // ordina gli archi per nodo sorgente, e in caso di pareggio, per nodo destinazione
    sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        if (get<0>(a) == get<0>(b)) {
            return get<1>(a) < get<1>(b);
        }
        return get<0>(a) < get<0>(b);
    });

    int num_nodes = nodes.size();
    int num_edges = edges.size();

    // scrive il grafo pesato, la lista di archi e la mappa (indirizzo, Id nodo)
    for (const auto& edge : edges) {
        int from = get<0>(edge);
        int to = get<1>(edge);
        int count = get<2>(edge);
        double total_value = get<3>(edge);
        graph_file << from << "\t" << to << "\t" << count << "\t" << total_value << "\n";
        edgelist_file << from << " " << to << "\n";
    }
    
    mapping_file << "address,node_id\n";
    for (const auto& i : nodes) {
        mapping_file << i.first << "," << i.second << "\n";
    }

    input_file_t.close();
    graph_file.close();
    mapping_file.close();
    edgelist_file.close();

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start);
    string chunk_name = extract_chunk_name(argv[2]);

    if (summary_file.tellp() == 0) {
        summary_file << "chunk,nodes,aggregated_edges,total_transactions\n";
    }

    summary_file << chunk_name << "," << num_nodes << "," << num_edges << "," << total_transactions << "\n";
    summary_file.close();
    cout << "Graph creation time (ns): " << duration.count() << '\n';
    
    return 0;
}



