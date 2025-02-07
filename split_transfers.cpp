/*Il codice prende in input un file di timestamp dei blocchi ed associa ogni ID di blocco ad un timestamp. Poi prende in input il file delle transazioni contenente le informazioni delle transazioni. 
Un file dei valori trasferiti (series) e il nome della stalbecoin analizzata.

Il codice associa ogni transazione al mese corrispondente usando il suo timestamp. Divide le transazioni in chunk temporali mensili. Scrive i chunk nei file <MeseAnno>.csv, creando una directory per la stablecoin in analisi se non esistente.*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <map>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdlib>

using namespace std;

// converto un timestamp UNIX in un idnetificatore di periodo mensile "MeseAnno", es: "Dec22". 
string timestamp_to_period(time_t timestamp) {
    tm* time_info = gmtime(&timestamp); 
    char buffer[8];
    strftime(buffer, sizeof(buffer), "%b%y", time_info);
    return string(buffer);
}

// questa funzione legge un file CSV contenente una mappatura tra ID di blocco e timestamp UNIX, e la memorizza in una struttura undordered per un accesso rapido
unordered_map<int, time_t> read_block_timestamps(const string& filename) {
    ifstream file(filename);
    unordered_map<int, time_t> block_timestamps;
    int block_id;
    time_t timestamp;

    if (!file.is_open()) {
        cerr << "Errore nell'apertura del file dei timestamp dei blocchi: " << filename << "\n";
        return block_timestamps;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string block_id_str, timestamp_str;
        if (getline(iss, block_id_str, ',') && getline(iss, timestamp_str, ',')) {
            try {
                block_id = stoi(block_id_str);
                timestamp = static_cast<time_t>(stoll(timestamp_str));
                block_timestamps[block_id] = timestamp;
            } catch (const exception& e) {
                cerr << "Errore nella conversione dei dati: " << e.what() << " (linea: " << line << ")\n";
            }
        } else {
            cerr << "Errore nel formato del CSV dei timestamp: " << line << "\n";
        }
    }

    file.close();
    return block_timestamps;
}

// funzione che legge un file contenente i valori numerici delle transazioni e li memorizza in un vettore ordinato
vector<double> read_transfer_values(const string& filename) {
    ifstream file(filename);
    vector<double> transfer_values;
    double value;

    if (!file.is_open()) {
        cerr << "Errore nell'apertura del file dei valori dei trasferimenti: " << filename << "\n";
        return transfer_values;
    }

    string line;
    while (getline(file, line)) {
        try {
            value = stod(line);
            transfer_values.push_back(value);
        } catch (const exception& e) {
            cerr << "Errore nella conversione del valore: " << line << " -> " << e.what() << "\n";
        }
    }

    file.close();
    if (transfer_values.empty()) {
        cerr << "Errore: Nessun valore di trasferimento caricato.\n";
    }
    return transfer_values;
}

// funzione che legge il file delle transazioni e suddivide le transazioni in chunk temporali mensili sulla base del loro timestamp
void process_transfers_by_time_chunks(const string& transfers_file, const vector<double>& transfer_values, const unordered_map<int, time_t>& block_timestamps, const string& stablecoin, const string& base_dir) {
    ifstream file(transfers_file);
    map<string, vector<tuple<int, int, int, int, double, time_t>>> transfers_by_month;
    int block_id, contract_id, from, to;
    size_t transfer_index = 0;

    if (!file.is_open()) {
        cerr << "Errore nell'apertura del file dei trasferimenti: " << transfers_file << "\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string block_id_str, contract_id_str, from_str, to_str;
        if (getline(iss, block_id_str, ',') && getline(iss, contract_id_str, ',') && 
            getline(iss, from_str, ',') && getline(iss, to_str, ',')) {
            try {
                block_id = stoi(block_id_str);
                contract_id = stoi(contract_id_str);
                from = stoi(from_str);
                to = stoi(to_str);
                if (transfer_index < transfer_values.size() && block_timestamps.count(block_id)) 
                    double value = transfer_values[transfer_index++];
                    time_t timestamp = block_timestamps.at(block_id);
                    string period = timestamp_to_period(timestamp);
                    transfers_by_month[period].emplace_back(block_id, contract_id, from, to, value, timestamp);
                
            } catch (const exception& e) {
                cerr << "Errore nella conversione dei dati della riga: " << line << " -> " << e.what() << "\n";
            }
        } else {
            cerr << "Errore nel formato della riga dei trasferimenti: " << line << "\n";
        }
    }

    file.close();

    string stablecoin_dir = base_dir + "/" + stablecoin;
    struct stat info;
    if (stat(stablecoin_dir.c_str(), &info) != 0) {
        if (system(("mkdir -p " + stablecoin_dir).c_str()) != 0) {
            cerr << "Errore nella creazione della directory: " << stablecoin_dir << "\n";
            return;
        }
    }
    for (const auto& [period, transfers] : transfers_by_month) {
        string filename = stablecoin_dir + "/" + period + ".csv";
        ofstream output_file(filename);
        if (!output_file.is_open()) {
            cerr << "Errore nell'apertura del file di output: " << filename << "\n";
            continue;
        }

        output_file << "BlockID,ContractID,From,To,Value,Timestamp\n";

        for (const auto& [block_id, contract_id, from, to, value, timestamp] : transfers) {
            output_file << block_id << "," << contract_id << "," << from << "," << to << "," << value << "," << timestamp << "\n";
        }

        output_file.close();
        cout << "Creato il file: " << filename << " con " << transfers.size() << " trasferimenti.\n";
    }
}

int main(int argc, char** argv) {
    if (argc < 6) {  
        cerr << "Usage: " << argv[0] << " <block_timestamps_file> <transfers_file> <values_file> <stablecoin> <output_directory>\n";
        return 1;
    }
    auto block_timestamps = read_block_timestamps(argv[1]);
    auto transfer_values = read_transfer_values(argv[3]);
    process_transfers_by_time_chunks(argv[2], transfer_values, block_timestamps, argv[4], argv[5]);

    return 0;
}
