/*Questo codice è il codice principale per creare il grafo globale di tutte le transazioni. 
I nodi rappresentano gli indirizzi Ethereum; gli archi diretti sono le transazioni tra indirizzi; gli archi sono pesati con il valore della transazione e il timestamp di blocco associato alla transazione.
Colasso i multiachi tra la stessa coppia di nodi, conteggiando il numero di transazioni e sommando i valori.
Ovviamente qui creiamo sia il grafo globale non collassato, che il grafo globale collassato con la stessa logica dei grafi temporali, quindi la cardinalità degli archi tra una coppia di nodi e la somma totale del valore trasferito, utilizzato il alcune metriche come il pagerank.

Il codice costruisce quindi un grafo globale pesato a partire da transazioni di stableocin. Assegna ID univoci agli indirizzi per semplificare la gestione dei nodi.
Collassa gli archi ripetuti sommando il numero di transazioni e il valore totale. Ordina gli archi ripetuti sommando il numero di transazioni e il valore totale. 
Orina le transazioni temporalmente, permettendo future analisi temporali.
Infine otterrò:
1) grafo globale non collassato con timestamp e valore sull'arco
2) grafo globale collassato
3) lista di archi coppia (IdNodoMittente,IdNodoDestinatario)
*/


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

struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto h1 = hash<T1>{}(p.first);
        auto h2 = hash<T2>{}(p.second);
        return h1 ^ (h2 << 1); 
    }
};
typedef unordered_map<int, string> block_timestamp_map_t; 
typedef unordered_map<int, int> node_map_t; 
typedef vector<tuple<int, int, double, string>> edges_t; 
using collapsed_edges_t = unordered_map<pair<int, int>,pair<int, double>, pair_hash>; 


// Funzione per leggere i timestamp dei blocchi
void load_block_timestamps(const char* timestamp_file, block_timestamp_map_t& block_timestamps) {
    ifstream input_file(timestamp_file);
    string line;
    while (getline(input_file, line)) {
        stringstream ss(line);
        string block_id_str, timestamp_str;
        if (getline(ss, block_id_str, ',') && getline(ss, timestamp_str, ',')) {
            block_timestamps[stoi(block_id_str)] = timestamp_str; 
        }
    }
}

// Funzione per ottenere o creare un nuovo ID per un indirizzo
inline int get_or_create_id(node_map_t& map, int address) {
    static int next_id = 0;
    auto it = map.find(address);
    if (it != map.end()) {
        return it->second;
    }
    int id = next_id++;
    map[address] = id;
    return id;
}

// Funzione per elaborare una riga del file di trasferimenti e associare il valore e il timestamp
void process_transfer_line(const string& line, const string& value, const block_timestamp_map_t& block_timestamps, node_map_t& nodes, edges_t& edges) {
    stringstream ss(line);
    string block_id_str, contract_id_str, from_str, to_str;
    int block_id, from, to;
    double value_in_DAI = stod(value);

    if (getline(ss, block_id_str, ',') && getline(ss, contract_id_str, ',') && getline(ss, from_str, ',') && getline(ss, to_str, ',')) {
        block_id = stoi(block_id_str);
        from = get_or_create_id(nodes, stoi(from_str));
        to = get_or_create_id(nodes, stoi(to_str));

        auto timestamp_it = block_timestamps.find(block_id);
        if (timestamp_it != block_timestamps.end()) {
            edges.emplace_back(from, to, value_in_DAI, timestamp_it->second);
        }
    }
}

// Funzione per collassare i multiarchi
void collapse_multi_edges(const edges_t& edges, collapsed_edges_t& collapsed_edges) {
    for (const auto& edge : edges) {
        int from = get<0>(edge);
        int to = get<1>(edge);
        double value = get<2>(edge);
        
        auto key = make_pair(from, to);
        
        if (collapsed_edges.find(key) == collapsed_edges.end()) {
            collapsed_edges[key] = make_pair(1, value);
        } else {
            collapsed_edges[key].first += 1; 
            collapsed_edges[key].second += value; 
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 8) {
        cerr << "Usage: " << argv[0] << " <transfers_file> <values_file> <timestamps_file> <graph_output_file> <mapping_file> <edgelist_file> <collapsed_output_file>\n";
        return 1;
    }

    block_timestamp_map_t block_timestamps;
    load_block_timestamps(argv[3], block_timestamps);
    ifstream transfers_file(argv[1]);
    ifstream values_file(argv[2]);

    if (!transfers_file.is_open() || !values_file.is_open()) {
        cerr << "Error: could not open transfers or values file!\n";
        return 1;
    }

    ofstream graph_output_file, mapping_file, edgelist_file, collapsed_output_file;
    if (strcmp(argv[4], "null") != 0) {
        graph_output_file.open(argv[4]);
        if (!graph_output_file.is_open()) {
            cerr << "Error: could not open graph output file!\n";
            return 1;
        }
    }

    if (strcmp(argv[5], "null") != 0) {
        mapping_file.open(argv[5]);
        if (!mapping_file.is_open()) {
            cerr << "Error: could not open mapping file!\n";
            return 1;
        }
    }

    if (strcmp(argv[6], "null") != 0) {
        edgelist_file.open(argv[6]);
        if (!edgelist_file.is_open()) {
            cerr << "Error: could not open edgelist file!\n";
            return 1;
        }
    }

    if (strcmp(argv[7], "null") != 0) {
        collapsed_output_file.open(argv[7]);
        if (!collapsed_output_file.is_open()) {
            cerr << "Error: could not open collapsed output file!\n";
            return 1;
        }
    }

    node_map_t nodes;  
    edges_t edges;  

    string transfer_line, value_line;
    getline(transfers_file, transfer_line);
    getline(values_file, value_line);
    while (getline(transfers_file, transfer_line) && getline(values_file, value_line)) {
        process_transfer_line(transfer_line, value_line, block_timestamps, nodes, edges);
    }
    sort(edges.begin(), edges.end(), [](const auto& a, const auto& b) {
        return get<3>(a) < get<3>(b);
    });

    if (graph_output_file.is_open()) {
        for (const auto& edge : edges) {
            int from = get<0>(edge);
            int to = get<1>(edge);
            double total_value = get<2>(edge);
            string timestamp = get<3>(edge);
            graph_output_file << from << " " << to << " " << total_value << " " << timestamp << "\n";
        }
    }

    if (edgelist_file.is_open()) {
        for (const auto& edge : edges) {
            int from = get<0>(edge);
            int to = get<1>(edge);
            edgelist_file << from << " " << to << "\n";
        }
    }

    collapsed_edges_t collapsed_edges;
    collapse_multi_edges(edges, collapsed_edges);

    if (collapsed_output_file.is_open()) {
        for (const auto& collapsed_edge : collapsed_edges) {
            int from = collapsed_edge.first.first;
            int to = collapsed_edge.first.second;
            int num_transactions = collapsed_edge.second.first;
            double total_value = collapsed_edge.second.second;
            collapsed_output_file << from << " " << to << " " << num_transactions << " " << total_value << "\n";
        }
    }

    if (mapping_file.is_open()) {
        mapping_file << "address,node_id\n";
        for (const auto& i : nodes) {
            mapping_file << i.first << "," << i.second << "\n";
        }
    }

    transfers_file.close();
    values_file.close();
    if (graph_output_file.is_open()) graph_output_file.close();
    if (mapping_file.is_open()) mapping_file.close();
    if (edgelist_file.is_open()) edgelist_file.close();
    if (collapsed_output_file.is_open()) collapsed_output_file.close();

    cout << "Graph, mapping, and edgelist generation completed and saved to the respective files." << endl;
    if (collapsed_output_file.is_open()) {
        cout << "Collapsed graph saved to " << argv[7] << endl;
    }

    return 0;
}