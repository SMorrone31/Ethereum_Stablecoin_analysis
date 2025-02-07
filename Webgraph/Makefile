# Variabili per compilazione ed esecuzione
JAVAC = javac
JAVA = java
SRC = src
BIN = out
CLASSPATH = $(BIN):$(LIBS)/*
LIBS = libs
CONVENTION = -Xms16G -Xmx256G

STABLECOINS = tether wrapped_ether usdc dai
RESULTS_BASE = /data/morroneT/webgraph_results

CHUNK_DIR = /data/morroneT/data/chunks
CHUNK_DIR_IGRAPH =/data/morroneT/data/chunks_igraph

DATA = /data/morroneT/data
RESULTS = /data/morroneT/webgraph_results/results
GRAPHS = /data/morroneT/webgraph_results/graphs

GRAPHS_INPUT = $(GRAPHS)/graphs_input
GRAPHS_WEBGRAPH = $(GRAPHS)/graphs_webgraph
MAPPING_FILES = $(GRAPHS)/mapping_file

# Sorgenti principali
MAIN_EDGE_BUILDER = builder.EdgeListBuilder
MAIN_GRAPH_BUILDER = builder.GraphBuilder
MAIN_SPLIT_TRANSFERS = builder.TemporalChunkDivider
MAIN_EXTRACT_TRANSFERS = builder.ContractExtractor
MAIN_BUILDER = builder.GraphBuilder
MAIN_CC = analyses.ClusteringCoefficient
MAIN_CENTRALITY = analyses.Centrality
MAIN_DIAMETER = analyses.CalculateDirectedDiameter

# File di input
TX = $(DATA)/erc20_transfers.csv
VAL_FILE = $(DATA)/erc20_values_dec.csv
TIMESTAMP_FILE = $(DATA)/block_timestamps_0-14999999.csv
CONTRACT_FILE = $(DATA)/erc20_contracts.csv
ADDRESSES = $(DATA)/erc20_addresses.csv

# directory
TX_EXTRACT_DIR = $(DATA)/transfer_extract

# Nome fisso per i file WebGraph
GRAPH_OUTPUT_PREFIX = graph_output

# Directory per i risultati delle statistiche
MAPPING_FILES = $(GRAPHS)/mapping_files

# Regola per creare le directory di output per ogni stablecoin
$(BIN):
	@mkdir -p $(BIN)

$(RESULTS):
	@mkdir -p $(RESULTS)

$(CHUNK_DIR):
	@mkdir -p $(CHUNK_DIR)

# Creazione delle directory di output per i grafi
$(GRAPHS):
	@mkdir -p $(GRAPHS_INPUT)
	@mkdir -p $(GRAPHS_WEBGRAPH)
	@mkdir -p $(MAPPING_FILES)
	@for chunk in $(STABLECOINS); do \
		mkdir -p $(GRAPHS_WEBGRAPH)/$$chunk; \
	done

create_directories:
	@for coin in $(STABLECOINS); do \
		mkdir -p $(RESULTS_BASE)/$$coin/graphs/graphs_input; \
		mkdir -p $(RESULTS_BASE)/$$coin/graphs/graphs_webgraph; \
		mkdir -p $(RESULTS_BASE)/$$coin/graphs/mapping_file; \
		mkdir -p $(RESULTS_BASE)/$$coin/results/centrality; \
		mkdir -p $(RESULTS_BASE)/$$coin/results/clustering; \
		mkdir -p $(RESULTS_BASE)/$$coin/results/diameters; \
		mkdir -p $(RESULTS_BASE)/$$coin/results/avg_spl; \
		mkdir -p $(RESULTS_BASE)/$$coin/results/weighted_avg_spl; \
		mkdir -p $(TX_EXTRACT_DIR)/transfers; \
		mkdir -p $(TX_EXTRACT_DIR)/values; \
	done

# Compila i file Java solo se ci sono modifiche
compile: $(BIN)
	@echo "Compilando il codice..."
	$(JAVAC) -d $(BIN) -cp "$(CLASSPATH)" $(SRC)/**/*.java
	@echo "Compilazione completata."

# Obiettivo all: compila solo se ci sono modifiche
all: compile create_directories
	@echo "Compilazione completata. File .out aggiornati se necessario."

# Regola per eseguire l'estrazione dei trasferimenti per ogni stablecoin
run_extractor: compile
	@for coin in $(STABLECOINS); do \
		echo "Eseguendo l'estrazione per $$coin..."; \
		hash=""; \
		if [ "$$coin" = "tether" ]; then \
			hash="dac17f958d2ee523a2206206994597c13d831ec7"; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			hash="c02aaa39b223fe8d0a0e5c4f27ead9083c756cc2"; \
		elif [ "$$coin" = "usdc" ]; then \
			hash="a0b86991c6218b36c1d19d4a2e9eb0ce3606eb48"; \
		elif [ "$$coin" = "dai" ]; then \
			hash="6b175474e89094c44da98b954eedeac495271d0f"; \
		fi; \
		transfers_output=$(TX_EXTRACT_DIR)/transfers/$$coin.csv; \
		values_output=$(TX_EXTRACT_DIR)/values/$$coin.csv; \
		$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_EXTRACT_TRANSFERS) $(TX) $(VAL_FILE) $$hash $$transfers_output $$values_output; \
		echo "Estrazione completata per $$coin."; \
	done


run_split_transfers: $(CHUNK_DIR)
	@for coin in $(STABLECOINS); do \
		echo "Eseguendo la suddivisione dei trasferimenti per $$coin in chunk temporali..."; \
		transfers_file=$(TX_EXTRACT_DIR)/transfers/$$coin.csv; \
		values_file=$(TX_EXTRACT_DIR)/values/$$coin.csv; \
		output_dir=$(CHUNK_DIR)/$$coin; \
		mkdir -p $$output_dir; \
		$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_SPLIT_TRANSFERS) $(TIMESTAMP_FILE) $$transfers_file $$values_file $$output_dir; \
		echo "Suddivisione completata per $$coin."; \
	done

# Regola per generare i file di input (.txt) dei grafi per ogni stablecoin
run_graph_input: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Generazione dei file di input per WebGraph per $$coin..."; \
		for file in $(CHUNK_DIR_IGRAPH)/$$coin/*.csv; do \
			if [ -f "$$file" ]; then \
				base_name=$$(basename $$file .csv); \
				number_file=$(RESULTS_BASE)/$$coin/graphs/graphs_input/$${base_name}_number.txt; \
				value_file=$(RESULTS_BASE)/$$coin/graphs/graphs_input/$${base_name}_value.txt; \
				$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" builder.EdgeListBuilder $$file $$number_file $$value_file $(RESULTS_BASE)/$$coin/graphs/mapping_file/$$coin_mapping.txt; \
				echo "Creati i file di input: $$number_file e $$value_file per $$coin"; \
			else \
				echo "Nessun file CSV trovato per $$coin nella cartella chunks."; \
			fi; \
		done; \
	done


run_builder: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Costruzione dei grafi per $$coin..."; \
		for chunk in $(RESULTS_BASE)/$$coin/graphs/graphs_input/*_number.txt; do \
			base_name=$$(basename $$chunk _number.txt); \
			output_dir=$(RESULTS_BASE)/$$coin/graphs/graphs_webgraph/$$base_name; \
			mkdir -p $$output_dir; \
			$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_BUILDER) $$chunk $$output_dir $(GRAPH_OUTPUT_PREFIX); \
			echo "Grafi costruiti per $$coin nella cartella $$output_dir"; \
		done; \
	done

run_global_graphs: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Creazione del grafo globale per $$coin..."; \
		input_file=/data/morroneT/igraph_results/graphs/global/$$coin/edgelistClean_$$coin.txt \
		output_dir=/data/morroneT/webgraph_results/$$coin/graphs/global_graph; \
		mkdir -p $$output_dir; \
		$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_BUILDER) $$input_file $$output_dir $(GRAPH_OUTPUT_PREFIX); \
		echo "Grafo globale creato per $$coin."; \
	done


run_centrality: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Calcolando la centralità per $$coin..."; \
		for chunk_dir in $(RESULTS_BASE)/$$coin/graphs/graphs_webgraph/*; do \
			graph_prefix=$$chunk_dir/graph_output; \
			if [ -f "$$graph_prefix.graph" ]; then \
				output_dir=$(RESULTS_BASE)/$$coin/results/centrality; \
				echo "Processing chunk: $$chunk_dir"; \
				$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_CENTRALITY) $$chunk_dir $$output_dir; \
				echo "Centralità calcolata per il chunk $$chunk_dir."; \
			else \
				echo "Errore: Il file $$graph_prefix.graph non esiste, saltando il chunk $$chunk_dir."; \
			fi; \
		done; \
	done

run_global_centrality: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Calcolando la centralità armonica per $$coin..."; \
		input_graph=$(RESULTS_BASE)/$$coin/graphs/global_graph; \
		graph_prefix=$$input_graph/graph_output; \
		if [ -f "$$graph_prefix.graph" ]; then \
		    output_file=$(RESULTS_BASE)/$$coin/results; \
			$(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_CENTRALITY) $$input_graph $$output_file; \
			echo "Centralità armonica calcolata per $$coin."; \
		else \
			echo "Errore: Grafo non trovato per $$coin."; \
		fi; \
	done

# Regola per calcolare il diametro per ogni stablecoin
run_diameters: create_directories
	@for coin in $(STABLECOINS); do \
		echo "Calcolando il diametro per $$coin..."; \
		output_csv=$(RESULTS_BASE)/$$coin/results/diameters/diameters.csv; \
		echo "ChunkName,Diameter" > $$output_csv; \
		for chunk_dir in $(RESULTS_BASE)/$$coin/graphs/graphs_webgraph/*; do \
			base_name=$$(basename $$chunk_dir); \
			graph_prefix=$(RESULTS_BASE)/$$coin/graphs/graphs_webgraph/$$base_name/$(GRAPH_OUTPUT_PREFIX); \
			if [ -f "$$graph_prefix.graph" ]; then \
				echo "Calcolando il diametro per il chunk: $$base_name..."; \
				diameter_output=$$( $(JAVA) $(CONVENTION) -cp "$(CLASSPATH)" $(MAIN_DIAMETER) $$graph_prefix ); \
				diameter=$$(echo "$$diameter_output" | grep 'Diametro del grafo' | awk '{print $$4}'); \
				if [ "$$diameter" ]; then \
					echo "$$base_name,$$diameter" >> $$output_csv; \
					echo "Diametro per il chunk $$base_name: $$diameter"; \
				else \
					echo "Errore: Impossibile calcolare il diametro per il chunk: $$base_name"; \
				fi; \
			else \
				echo "Errore: Grafo non trovato per il chunk: $$base_name"; \
			fi; \
		done; \
		echo "Diametri calcolati per $$coin."; \
	done


clean:
	@echo "Pulizia delle directory di output e di compilazione..."
	rm -rf $(BIN)
	rm -rf $(RESULTS_BASE)
	rm -rf $(CHUNK_DIR)
	@echo "Pulizia completata."
