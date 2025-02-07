# Definizione del compilatore e delle opzioni
CXX = g++
CXX_FLAGS = -O3 -std=c++17 -I /data/morroneT/igraph/include/igraph -Iutils
LD_FLAGS = -L /data/morroneT/igraph/lib -ligraph -fopenmp

# Stablecoin per splitting
STABLECOINS = dai tether usdc wrapped_ether

# File di input
TRANSFERS_DAI = /data/morroneT/data/transfer_extract/transfers/dai.csv
VALUES_DAI = /data/morroneT/data/transfer_extract/values/dai.csv

TRANSFERS_TETHER = /data/morroneT/data/transfer_extract/transfers/tether.csv
VALUES_TETHER = /data/morroneT/data/transfer_extract/values/tether.csv

TRANSFERS_USDC = /data/morroneT/data/transfer_extract/transfers/usdc.csv
VALUES_USDC = /data/morroneT/data/transfer_extract/values/usdc.csv

TRANSFERS_WRAPPED_ETHER = /data/morroneT/data/transfer_extract/transfers/wrapped_ether.csv
VALUES_WRAPPED_ETHER = /data/morroneT/data/transfer_extract/values/wrapped_ether.csv

GRAPHS_GLOBAL = /data/morroneT/igraph_results/graphs/global
GLOBAL_DIR_DAI = $(GRAPHS_GLOBAL)/dai
GLOBAL_DIR_TETHER = $(GRAPHS_GLOBAL)/tether
GLOBAL_DIR_USDC = $(GRAPHS_GLOBAL)/usdc
GLOBAL_DIR_WRAPPED_ETHER = $(GRAPHS_GLOBAL)/wrapped_ether

PAGERANK_GRAPH_DAI = $(GRAPHS_GLOBAL)/dai/collapsed_dai.txt
PAGERANK_GRAPH_TETHER = $(GRAPHS_GLOBAL)/tether/collapsed_tether.txt
PAGERANK_GRAPH_USDC = $(GRAPHS_GLOBAL)/usdc/collapsed_usdc.txt
PAGERANK_GRAPH_WRAPPED_ETHER = $(GRAPHS_GLOBAL)/wrapped_ether/collapsed_wrapped_ether.txt

# Timestamp file
TIMESTAMP = /data/morroneT/data/block_timestamps_0-14999999.csv

# Directory base per le stablecoin e i risultati
CHUNKS_DIR = /data/morroneT/data/chunks_igraph
GRAPHS_BASE = /data/morroneT/igraph_results/graphs/chunks
SUMMARY_DIR = /data/morroneT/igraph_results/results/chunks_stats/summary

# Directory per grafi, edgelist, e mapping file suddivisi per stablecoin
GRAPHS_FILES_TETHER = $(GRAPHS_BASE)/graphs_files/tether
GRAPHS_FILES_WRAPPED_ETHER = $(GRAPHS_BASE)/graphs_files/wrapped_ether
GRAPHS_FILES_USDC = $(GRAPHS_BASE)/graphs_files/usdc
GRAPHS_FILES_DAI = $(GRAPHS_BASE)/graphs_files/dai

MAPPING_FILES_TETHER = $(GRAPHS_BASE)/mapping_files/tether
MAPPING_FILES_WRAPPED_ETHER = $(GRAPHS_BASE)/mapping_files/wrapped_ether
MAPPING_FILES_USDC = $(GRAPHS_BASE)/mapping_files/usdc
MAPPING_FILES_DAI = $(GRAPHS_BASE)/mapping_files/dai

EDGELIST_FILES_TETHER = $(GRAPHS_BASE)/edgelist_files/tether
EDGELIST_FILES_WRAPPED_ETHER = $(GRAPHS_BASE)/edgelist_files/wrapped_ether
EDGELIST_FILES_USDC = $(GRAPHS_BASE)/edgelist_files/usdc
EDGELIST_FILES_DAI = $(GRAPHS_BASE)/edgelist_files/dai

# Directory per il file di output dei gradi e della forza per stablecoin
RESULTS_DEGREE_TETHER = /data/morroneT/igraph_results/results/chunks_stats/degree_files/tether
RESULTS_DEGREE_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/degree_files/wrapped_ether
RESULTS_DEGREE_USDC = /data/morroneT/igraph_results/results/chunks_stats/degree_files/usdc
RESULTS_DEGREE_DAI = /data/morroneT/igraph_results/results/chunks_stats/degree_files/dai

RESULTS_CLUSTERING_TETHER = /data/morroneT/igraph_results/results/chunks_stats/clustering_coefficient/tether
RESULTS_CLUSTERING_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/clustering_coefficient/wrapped_ether
RESULTS_CLUSTERING_USDC = /data/morroneT/igraph_results/results/chunks_stats/clustering_coefficient/usdc
RESULTS_CLUSTERING_DAI = /data/morroneT/igraph_results/results/chunks_stats/clustering_coefficient/dai


# Directory per il file di output del PageRank per stablecoin
RESULTS_PAGERANK_TETHER = /data/morroneT/igraph_results/results/chunks_stats/page_rank_chunks_files/tether
RESULTS_PAGERANK_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/page_rank_chunks_files/wrapped_ether
RESULTS_PAGERANK_USDC = /data/morroneT/igraph_results/results/chunks_stats/page_rank_chunks_files/usdc
RESULTS_PAGERANK_DAI = /data/morroneT/igraph_results/results/chunks_stats/page_rank_chunks_files/dai

# Directory per i file di output del Giant WCC Degree per stablecoin
RESULTS_GIANT_WCC_TETHER = /data/morroneT/igraph_results/results/chunks_stats/giant_wcc_degree/tether
RESULTS_GIANT_WCC_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/giant_wcc_degree/wrapped_ether
RESULTS_GIANT_WCC_USDC = /data/morroneT/igraph_results/results/chunks_stats/giant_wcc_degree/usdc
RESULTS_GIANT_WCC_DAI = /data/morroneT/igraph_results/results/chunks_stats/giant_wcc_degree/dai

# Directory per i file di output delle Giant Components e le loro statistiche per ogni stablecoin
RESULTS_COMPONENTS_TETHER = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_components/tether
RESULTS_COMPONENTS_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_components/wrapped_ether
RESULTS_COMPONENTS_USDC = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_components/usdc
RESULTS_COMPONENTS_DAI = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_components/dai

RESULTS_STATS_TETHER = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_stats/tether
RESULTS_STATS_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_stats/wrapped_ether
RESULTS_STATS_USDC = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_stats/usdc
RESULTS_STATS_DAI = /data/morroneT/igraph_results/results/chunks_stats/components_stats/giant_stats/dai

# Directory di output per i risultati della power-law
RESULTS_POWER_LAW_TETHER = /data/morroneT/igraph_results/results/chunks_stats/power_law/tether
RESULTS_POWER_LAW_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/power_law/wrapped_ether
RESULTS_POWER_LAW_USDC = /data/morroneT/igraph_results/results/chunks_stats/power_law/usdc
RESULTS_POWER_LAW_DAI = /data/morroneT/igraph_results/results/chunks_stats/power_law/dai

# Directory di output per i risultati delle densità
RESULTS_DENSITY_DAI = /data/morroneT/igraph_results/results/chunks_stats/density/dai
RESULTS_DENSITY_TETHER = /data/morroneT/igraph_results/results/chunks_stats/density/tether
RESULTS_DENSITY_WRAPPED_ETHER = /data/morroneT/igraph_results/results/chunks_stats/density/wrapped_ether
RESULTS_DENSITY_USDC = /data/morroneT/igraph_results/results/chunks_stats/density/usdc

# Definizione dei percorsi per i file di output delle statistiche globali (uno per ogni stablecoin)
RESULTS_GLOBAL_STATS = /data/morroneT/igraph_results/results/global_stats
STATS_DIR_DAI = $(RESULTS_GLOBAL_STATS)/dai
STATS_DIR_TETHER = $(RESULTS_GLOBAL_STATS)/tether
STATS_DIR_USDC = $(RESULTS_GLOBAL_STATS)/usdc
STATS_DIR_WRAPPED_ETHER = $(RESULTS_GLOBAL_STATS)/wrapped_ether

# Sotto-directory per i risultati di PageRank e altre statistiche
PAGERANK_DIR_DAI = $(STATS_DIR_DAI)/pagerank
PAGERANK_DIR_TETHER = $(STATS_DIR_TETHER)/pagerank
PAGERANK_DIR_USDC = $(STATS_DIR_USDC)/pagerank
PAGERANK_DIR_WRAPPED_ETHER = $(STATS_DIR_WRAPPED_ETHER)/pagerank

HUB_AUTH_DIR_DAI = $(STATS_DIR_DAI)/hub_authorities_microvelocity_score
HUB_AUTH_DIR_TETHER = $(STATS_DIR_TETHER)/hub_authorities_microvelocity_score
HUB_AUTH_DIR_USDC = $(STATS_DIR_USDC)/hub_authorities_microvelocity_score
HUB_AUTH_DIR_WRAPPED_ETHER = $(STATS_DIR_WRAPPED_ETHER)/hub_authorities_microvelocity_score

# Definizione dei percorsi per i file di output delle statistiche globali (uno per ogni stablecoin)
RESULTS_GLOBAL_STATS = /data/morroneT/igraph_results/results/global_stats
STATS_DIR_DAI = $(RESULTS_GLOBAL_STATS)/dai
STATS_DIR_TETHER = $(RESULTS_GLOBAL_STATS)/tether
STATS_DIR_USDC = $(RESULTS_GLOBAL_STATS)/usdc
STATS_DIR_WRAPPED_ETHER = $(RESULTS_GLOBAL_STATS)/wrapped_ether

# Sotto-directory per i risultati di assortatività e gradi
DEGREE_DIR_DAI = $(STATS_DIR_DAI)/degree
DEGREE_DIR_TETHER = $(STATS_DIR_TETHER)/degree
DEGREE_DIR_USDC = $(STATS_DIR_USDC)/degree
DEGREE_DIR_WRAPPED_ETHER = $(STATS_DIR_WRAPPED_ETHER)/degree

RECIPROCITY_RESULTS_DIR = /data/morroneT/igraph_results/results/chunks_stats/reciprocity
ASSORTATIVITY_CHUNKS_DIR = /data/morroneT/igraph_results/results/chunks_stats/assortativity




# Target per la compilazione di tutti i file
.PHONY: clean all builder degree components power_law split_transfers run_chunks pagerank builder_global pagerank_score_global \
assortativity_global degree_giant_wcc_components clustering_coefficient

# File di output finali 
all: builder degree components power_law split_transfers pagerank builder_global pagerank_score_global assortativity_global degree_giant_wcc_components clustering_coefficient

graph_utils.o: utils/graph_utils.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# Regola per compilare builder.cpp
builder: builder.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)
	
# Regola per compilare analyzer_degree.cpp
degree: degree.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

components: components.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

power_law: power_law.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

split_transfers: split_transfers.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

pagerank: pagerank.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

builder_global: builder_global.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

pagerank_score_global: pagerank_score_global.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

assortativity_global: assortativity_global.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

degree_giant_wcc_components: degree_giant_wcc_components.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)
	
clustering_coefficient: clustering_coefficient.o graph_utils.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)

reciprocity: reciprocity.o
	$(CXX) $(CXX_FLAGS) $^ -o $@ $(LD_FLAGS)


# Regola per compilare i file .o da .cpp
%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

results_dir:
	@mkdir -p $(CHUNKS_DIR)/dai $(CHUNKS_DIR)/tether $(CHUNKS_DIR)/usdc $(CHUNKS_DIR)/wrapped_ether
	@mkdir -p $(GRAPHS_FILES_TETHER) $(GRAPHS_FILES_WRAPPED_ETHER) $(GRAPHS_FILES_USDC) $(GRAPHS_FILES_DAI)
	@mkdir -p $(MAPPING_FILES_TETHER) $(MAPPING_FILES_WRAPPED_ETHER) $(MAPPING_FILES_USDC) $(MAPPING_FILES_DAI)
	@mkdir -p $(EDGELIST_FILES_TETHER) $(EDGELIST_FILES_WRAPPED_ETHER) $(EDGELIST_FILES_USDC) $(EDGELIST_FILES_DAI)
	@mkdir -p $(SUMMARY_DIR)
	@mkdir -p $(RESULTS_PAGERANK_TETHER) $(RESULTS_PAGERANK_WRAPPED_ETHER) $(RESULTS_PAGERANK_USDC) $(RESULTS_PAGERANK_DAI)
	@mkdir -p $(RESULTS_GIANT_WCC_TETHER) $(RESULTS_GIANT_WCC_WRAPPED_ETHER) $(RESULTS_GIANT_WCC_USDC) $(RESULTS_GIANT_WCC_DAI)
	@mkdir -p $(RESULTS_COMPONENTS_TETHER) $(RESULTS_COMPONENTS_WRAPPED_ETHER) $(RESULTS_COMPONENTS_USDC) $(RESULTS_COMPONENTS_DAI)
	@mkdir -p $(RESULTS_STATS_TETHER) $(RESULTS_STATS_WRAPPED_ETHER) $(RESULTS_STATS_USDC) $(RESULTS_STATS_DAI)
	@mkdir -p $(RESULTS_POWER_LAW_TETHER) $(RESULTS_POWER_LAW_WRAPPED_ETHER) $(RESULTS_POWER_LAW_USDC) $(RESULTS_POWER_LAW_DAI)
	@mkdir -p $(RESULTS_CLUSTERING_TETHER) $(RESULTS_CLUSTERING__WRAPPED_ETHER) $(RESULTS_CLUSTERING__USDC) $(RESULTS_CLUSTERING__DAI)
	@mkdir -p $(RESULTS_DENSITY_DAI) $(RESULTS_DENSITY_TETHER) $(RESULTS_DENSITY_WRAPPED_ETHER) $(RESULTS_DENSITY_USDC) 


global_results_dir:
	@mkdir -p $(GLOBAL_DIR_DAI) $(GLOBAL_DIR_TETHER) $(GLOBAL_DIR_USDC) $(GLOBAL_DIR_WRAPPED_ETHER)


global_stats_dir:
	@mkdir -p $(PAGERANK_DIR_DAI) $(HUB_AUTH_DIR_DAI)
	@mkdir -p $(PAGERANK_DIR_TETHER) $(HUB_AUTH_DIR_TETHER)
	@mkdir -p $(PAGERANK_DIR_USDC) $(HUB_AUTH_DIR_USDC)
	@mkdir -p $(PAGERANK_DIR_WRAPPED_ETHER) $(HUB_AUTH_DIR_WRAPPED_ETHER)
	@mkdir -p $(DEGREE_DIR_DAI) $(STATS_DIR_DAI)
	@mkdir -p $(DEGREE_DIR_TETHER) $(STATS_DIR_TETHER)
	@mkdir -p $(DEGREE_DIR_USDC) $(STATS_DIR_USDC)
	@mkdir -p $(DEGREE_DIR_WRAPPED_ETHER) $(STATS_DIR_WRAPPED_ETHER)



run_split_transfers: split_transfers results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			transfers=$(TRANSFERS_DAI); \
			values=$(VALUES_DAI); \
		elif [ "$$coin" = "tether" ]; then \
			transfers=$(TRANSFERS_TETHER); \
			values=$(VALUES_TETHER); \
		elif [ "$$coin" = "usdc" ]; then \
			transfers=$(TRANSFERS_USDC); \
			values=$(VALUES_USDC); \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			transfers=$(TRANSFERS_WRAPPED_ETHER); \
			values=$(VALUES_WRAPPED_ETHER); \
		fi; \
		echo "Eseguendo lo splitting per $$coin..."; \
		./split_transfers $(TIMESTAMP) $$transfers $$values $$coin $(CHUNKS_DIR); \
		echo "Completato per $$coin."; \
	done

run_builder: builder results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(GRAPHS_FILES_DAI); \
			mapping_dir=$(MAPPING_FILES_DAI); \
			edgelist_dir=$(EDGELIST_FILES_DAI); \
			summary_file=$(SUMMARY_DIR)/summary_dai.csv; \
		elif [ "$$coin" = "tether" ]; then \
			graph_dir=$(GRAPHS_FILES_TETHER); \
			mapping_dir=$(MAPPING_FILES_TETHER); \
			edgelist_dir=$(EDGELIST_FILES_TETHER); \
			summary_file=$(SUMMARY_DIR)/summary_tether.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(GRAPHS_FILES_USDC); \
			mapping_dir=$(MAPPING_FILES_USDC); \
			edgelist_dir=$(EDGELIST_FILES_USDC); \
			summary_file=$(SUMMARY_DIR)/summary_usdc.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(GRAPHS_FILES_WRAPPED_ETHER); \
			mapping_dir=$(MAPPING_FILES_WRAPPED_ETHER); \
			edgelist_dir=$(EDGELIST_FILES_WRAPPED_ETHER); \
			summary_file=$(SUMMARY_DIR)/summary_wrapped_ether.csv; \
		fi; \
		echo "Processing $$coin chunks..."; \
		for chunk in $(CHUNKS_DIR)/$$coin/*.csv; do \
			chunk_name=$$(basename $$chunk .csv); \
			./builder $$chunk $$graph_dir/graph_$$chunk_name.txt $$mapping_dir/mapping_$$chunk_name.csv $$edgelist_dir/edgelist_$$chunk_name.txt $$summary_file; \
		done; \
	done


run_degree: degree results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(GRAPHS_FILES_DAI); \
			output_dir=$(RESULTS_DEGREE_DAI); \
		elif [ "$$coin" = "tether" ]; then \
			graph_dir=$(GRAPHS_FILES_TETHER); \
			output_dir=$(RESULTS_DEGREE_TETHER); \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(GRAPHS_FILES_USDC); \
			output_dir=$(RESULTS_DEGREE_USDC); \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(GRAPHS_FILES_WRAPPED_ETHER); \
			output_dir=$(RESULTS_DEGREE_WRAPPED_ETHER); \
		fi; \
		echo "Calculating degrees for $$coin..."; \
		for chunk in $$graph_dir/*.txt; do \
			chunk_name=$$(basename $$chunk .txt); \
			output_file=$$output_dir/degree_$$chunk_name.csv; \
			./degree $$chunk $$output_file; \
			echo "Results saved in $$output_file"; \
		done; \
	done

run_cc_density: clustering_coefficient results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(EDGELIST_FILES_DAI); \
			clustering_output_file=$(RESULTS_CLUSTERING_DAI)/clustering_coefficients.csv; \
			density_output_file=$(RESULTS_DENSITY_DAI)/densities.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(EDGELIST_FILES_USDC); \
			clustering_output_file=$(RESULTS_CLUSTERING_USDC)/clustering_coefficients.csv; \
			density_output_file=$(RESULTS_DENSITY_USDC)/densities.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(EDGELIST_FILES_WRAPPED_ETHER); \
			clustering_output_file=$(RESULTS_CLUSTERING_WRAPPED_ETHER)/clustering_coefficients.csv; \
			density_output_file=$(RESULTS_DENSITY_WRAPPED_ETHER)/densities.csv; \
		fi; \
		echo "Calculating clustering coefficient and density for $$coin..."; \
		graph_chunks=$$(find $$graph_dir -name '*.txt'); \
		./clustering_coefficient $$graph_dir $$clustering_output_file $$density_output_file $$graph_chunks; \
		echo "Results saved in $$clustering_output_file and $$density_output_file"; \
	done

	# Directory per i file di output dell'assortatività per chunk


# Comando per calcolare l'assortatività per ogni chunk temporale di ogni stablecoin
run_assortativity: assortativity_global results_dir
	@mkdir -p $(ASSORTATIVITY_CHUNKS_DIR)
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(GRAPHS_FILES_DAI); \
			degree_dir=$(RESULTS_DEGREE_DAI); \
			output_file=$(ASSORTATIVITY_CHUNKS_DIR)/assortativity_dai.csv; \
		elif [ "$$coin" = "tether" ]; then \
			graph_dir=$(GRAPHS_FILES_TETHER); \
			degree_dir=$(RESULTS_DEGREE_TETHER); \
			output_file=$(ASSORTATIVITY_CHUNKS_DIR)/assortativity_tether.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(GRAPHS_FILES_USDC); \
			degree_dir=$(RESULTS_DEGREE_USDC); \
			output_file=$(ASSORTATIVITY_CHUNKS_DIR)/assortativity_usdc.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(GRAPHS_FILES_WRAPPED_ETHER); \
			degree_dir=$(RESULTS_DEGREE_WRAPPED_ETHER); \
			output_file=$(ASSORTATIVITY_CHUNKS_DIR)/assortativity_wrapped_ether.csv; \
		fi; \
		echo "ChunkName,InDegreeAssortativity,OutDegreeAssortativity,InStrengthAssortativity,OutStrengthAssortativity" > $$output_file; \
		for chunk in $$graph_dir/*.txt; do \
			chunk_name=$$(basename $$chunk .txt); \
			degree_file=$$degree_dir/degree_$$chunk_name.csv; \
			chunk_output_file=/tmp/assortativity_$$chunk_name.csv; \
			if [ -f $$degree_file ]; then \
				./assortativity_global $$chunk $$degree_file $$chunk_output_file; \
				echo -n "$$chunk_name," >> $$output_file; \
				tail -n 1 $$chunk_output_file >> $$output_file; \
				rm -f $$chunk_output_file; \
				echo "Assortativity for $$chunk_name saved in $$output_file"; \
			else \
				echo "Warning: Degree file $$degree_file not found for chunk $$chunk_name" >&2; \
			fi; \
		done; \
	done

run_reciprocity: reciprocity
	@mkdir -p $(RECIPROCITY_RESULTS_DIR)
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(EDGELIST_FILES_DAI); \
			output_file=$(RECIPROCITY_RESULTS_DIR)/reciprocity_dai.csv; \
		elif [ "$$coin" = "tether" ]; then \
			graph_dir=$(EDGELIST_FILES_TETHER); \
			output_file=$(RECIPROCITY_RESULTS_DIR)/reciprocity_tether.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(EDGELIST_FILES_USDC); \
			output_file=$(RECIPROCITY_RESULTS_DIR)/reciprocity_usdc.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(EDGELIST_FILES_WRAPPED_ETHER); \
			output_file=$(RECIPROCITY_RESULTS_DIR)/reciprocity_wrapped_ether.csv; \
		fi; \
		echo "ChunkName,Reciprocity" > $$output_file; \
		for chunk_file in $$graph_dir/*.txt; do \
			chunk_name=$$(basename $$chunk_file .txt); \
			reciprocity_value=$$(./reciprocity $$chunk_file tmp_reciprocity.txt && tail -n 1 tmp_reciprocity.txt); \
			echo "$$chunk_name,$$reciprocity_value" >> $$output_file; \
			rm -f tmp_reciprocity.txt; \
			echo "Reciprocity for $$chunk_name of $$coin saved in $$output_file"; \
		done; \
	done


run_degree_giant_wcc: degree_giant_wcc_components results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(GRAPHS_FILES_DAI); \
			output_dir=$(RESULTS_GIANT_WCC_DAI); \
		elif [ "$$coin" = "tether" ]; then \
			graph_dir=$(GRAPHS_FILES_TETHER); \
			output_dir=$(RESULTS_GIANT_WCC_TETHER); \
		elif [ "$$coin" = "usdc" ]; then \
			graph_dir=$(GRAPHS_FILES_USDC); \
			output_dir=$(RESULTS_GIANT_WCC_USDC); \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			graph_dir=$(GRAPHS_FILES_WRAPPED_ETHER); \
			output_dir=$(RESULTS_GIANT_WCC_WRAPPED_ETHER); \
		fi; \
		echo "Processing Giant WCC for $$coin..."; \
		for chunk in $$graph_dir/*.txt; do \
			chunk_name=$$(basename $$chunk .txt); \
			output_file=$$output_dir/wcc_giant_degree_$$chunk_name.csv; \
			./degree_giant_wcc_components $$chunk $$output_file; \
			echo "Results saved in $$output_file"; \
		done; \
	done


run_power_law: power_law results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "tether" ]; then \
			degree_dir=$(RESULTS_DEGREE_TETHER); \
			output_file=$(RESULTS_POWER_LAW_TETHER)/power_law_stats.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			degree_dir=$(RESULTS_DEGREE_USDC); \
			output_file=$(RESULTS_POWER_LAW_USDC)/power_law_stats.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			degree_dir=$(RESULTS_DEGREE_WRAPPED_ETHER); \
			output_file=$(RESULTS_POWER_LAW_WRAPPED_ETHER)/power_law_stats.csv; \
		elif [ "$$coin" = "dai" ]; then \
			degree_dir=$(RESULTS_DEGREE_DAI); \
			output_file=$(RESULTS_POWER_LAW_DAI)/power_law_stats.csv; \
		fi; \
		echo "Processing power-law analysis for $$coin..."; \
		graph_chunks=$$(find $$degree_dir -name '*.csv'); \
		./power_law $$degree_dir $$output_file $$graph_chunks; \
		echo "Results for $$coin saved in $$output_file"; \
	done



run_components: components results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			edgelist_dir=$(EDGELIST_FILES_DAI); \
			output_components=$(RESULTS_COMPONENTS_DAI)/giant_components_dai.csv; \
			output_stats=$(RESULTS_STATS_DAI)/giant_stats_dai.csv; \
		elif [ "$$coin" = "tether" ]; then \
			edgelist_dir=$(EDGELIST_FILES_TETHER); \
			output_components=$(RESULTS_COMPONENTS_TETHER)/giant_components_tether.csv; \
			output_stats=$(RESULTS_STATS_TETHER)/giant_stats_tether.csv; \
		elif [ "$$coin" = "usdc" ]; then \
			edgelist_dir=$(EDGELIST_FILES_USDC); \
			output_components=$(RESULTS_COMPONENTS_USDC)/giant_components_usdc.csv; \
			output_stats=$(RESULTS_STATS_USDC)/giant_stats_usdc.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
			edgelist_dir=$(EDGELIST_FILES_WRAPPED_ETHER); \
			output_components=$(RESULTS_COMPONENTS_WRAPPED_ETHER)/giant_components_wrapped_ether.csv; \
			output_stats=$(RESULTS_STATS_WRAPPED_ETHER)/giant_stats_wrapped_ether.csv; \
		fi; \
		echo "Processing components analysis for $$coin..."; \
		for chunk in $$edgelist_dir/*.txt; do \
			chunk_name=$$(basename $$chunk .txt); \
			./components $$chunk $$output_components $$output_stats; \
		done; \
	done

run_pagerank: pagerank results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
			graph_dir=$(GRAPHS_FILES_DAI); \
			output_pagerank_dir=$(RESULTS_PAGERANK_DAI); \
			output_hub_auth_dir=$(RESULTS_PAGERANK_DAI); \
		fi; \
		echo "Calculating PageRank for $$coin..."; \
		for chunk in $$graph_dir/*.txt; do \
			chunk_name=$$(basename $$chunk .txt); \
			output_pagerank_file=$$output_pagerank_dir/pagerank_$$chunk_name.csv; \
			output_hub_auth_file=$$output_hub_auth_dir/hub_authority_$$chunk_name.csv; \
			./pagerank $$chunk $$output_pagerank_file $$output_hub_auth_file; \
			echo "PageRank results saved in $$output_pagerank_file"; \
			echo "Hub and Authority results saved in $$output_hub_auth_file"; \
		done; \
	done



# Esegui tutti i calcoli per i chunk
run_chunks: run_builder run_degree run_components run_power_law 


# GRAFO COMPLETO
run_global_builder: builder_global global_results_dir
	# Creazione del grafo globale per DAI senza grafi collassati
	./builder_global $(TRANSFERS_DAI) $(VALUES_DAI) $(TIMESTAMP) $(GLOBAL_DIR_DAI)/graph_dai.txt $(GLOBAL_DIR_DAI)/mapping_dai.csv $(GLOBAL_DIR_DAI)/edgelist_dai.txt null
	echo "Global graph for DAI created in $(GLOBAL_DIR_DAI)"
	
	# Creazione del grafo globale per Tether
	./builder_global $(TRANSFERS_TETHER) $(VALUES_TETHER) $(TIMESTAMP) $(GLOBAL_DIR_TETHER)/graph_tether.txt $(GLOBAL_DIR_TETHER)/mapping_tether.csv $(GLOBAL_DIR_TETHER)/edgelist_tether.txt null
	echo "Global graph for Tether created in $(GLOBAL_DIR_TETHER)"
	
	# Creazione del grafo globale per USDC
	./builder_global $(TRANSFERS_USDC) $(VALUES_USDC) $(TIMESTAMP) $(GLOBAL_DIR_USDC)/graph_usdc.txt $(GLOBAL_DIR_USDC)/mapping_usdc.csv $(GLOBAL_DIR_USDC)/edgelist_usdc.txt null
	echo "Global graph for USDC created in $(GLOBAL_DIR_USDC)"
	
	# Creazione del grafo globale per Wrapped Ether
	./builder_global $(TRANSFERS_WRAPPED_ETHER) $(VALUES_WRAPPED_ETHER) $(TIMESTAMP) $(GLOBAL_DIR_WRAPPED_ETHER)/graph_wrapped_ether.txt $(GLOBAL_DIR_WRAPPED_ETHER)/mapping_wrapped_ether.csv $(GLOBAL_DIR_WRAPPED_ETHER)/edgelist_wrapped_ether.txt null
	echo "Global graph for Wrapped Ether created in $(GLOBAL_DIR_WRAPPED_ETHER)"

run_collapsed_global_builder: builder_global global_results_dir
	# Creazione del grafo collassato per DAI
	./builder_global $(TRANSFERS_DAI) $(VALUES_DAI) $(TIMESTAMP) null null null $(GLOBAL_DIR_DAI)/collapsed_dai.txt
	echo "Collapsed global graph for DAI created in $(GLOBAL_DIR_DAI)"
	
	# Creazione del grafo collassato per Tether
	./builder_global $(TRANSFERS_TETHER) $(VALUES_TETHER) $(TIMESTAMP) null null null $(GLOBAL_DIR_TETHER)/collapsed_tether.txt
	echo "Collapsed global graph for Tether created in $(GLOBAL_DIR_TETHER)"
	
	# Creazione del grafo collassato per USDC
	./builder_global $(TRANSFERS_USDC) $(VALUES_USDC) $(TIMESTAMP) null null null $(GLOBAL_DIR_USDC)/collapsed_usdc.txt
	echo "Collapsed global graph for USDC created in $(GLOBAL_DIR_USDC)"
	
	# Creazione del grafo collassato per Wrapped Ether
	./builder_global $(TRANSFERS_WRAPPED_ETHER) $(VALUES_WRAPPED_ETHER) $(TIMESTAMP) null null null $(GLOBAL_DIR_WRAPPED_ETHER)/collapsed_wrapped_ether.txt
	echo "Collapsed global graph for Wrapped Ether created in $(GLOBAL_DIR_WRAPPED_ETHER)"

run_pagerank_global: pagerank_score_global global_stats_dir
	# Calcolo del PageRank per DAI
	./pagerank_score_global $(PAGERANK_GRAPH_DAI) null null \
	$(PAGERANK_DIR_DAI)/pagerank_dai.csv null null
	echo "PageRank per DAI salvato in $(PAGERANK_DIR_DAI)"

	# Calcolo del PageRank per Tether
	./pagerank_score_global $(PAGERANK_GRAPH_TETHER) null null \
	$(PAGERANK_DIR_TETHER)/pagerank_tether.csv null null
	echo "PageRank per Tether salvato in $(PAGERANK_DIR_TETHER)"

	# Calcolo del PageRank per USDC
	./pagerank_score_global $(PAGERANK_GRAPH_USDC) null null \
	$(PAGERANK_DIR_USDC)/pagerank_usdc.csv null null
	echo "PageRank per USDC salvato in $(PAGERANK_DIR_USDC)"

	# Calcolo del PageRank per Wrapped Ether
	./pagerank_score_global $(PAGERANK_GRAPH_WRAPPED_ETHER) null null \
	$(PAGERANK_DIR_WRAPPED_ETHER)/pagerank_wrapped_ether.csv null null
	echo "PageRank per Wrapped Ether salvato in $(PAGERANK_DIR_WRAPPED_ETHER)"


run_global_components: components results_dir
	./components $(GLOBAL_DIR_DAI)/edgelist_dai.txt /data/morroneT/igraph_results/results/global_stats/dai/giant_components.csv /data/morroneT/igraph_results/results/global_stats/dai/giant_stats.csv
	./components $(GLOBAL_DIR_TETHER)/edgelist_tether.txt /data/morroneT/igraph_results/results/global_stats/tether/giant_components.csv /data/morroneT/igraph_results/results/global_stats/tether/giant_stats.csv
	./components $(GLOBAL_DIR_USDC)/edgelist_usdc.txt /data/morroneT/igraph_results/results/global_stats/usdc/giant_components.csv /data/morroneT/igraph_results/results/global_stats/usdc/giant_stats.csv
	./components $(GLOBAL_DIR_WRAPPED_ETHER)/edgelist_wrapped_ether.txt /data/morroneT/igraph_results/results/global_stats/wrapped_ether/giant_components.csv /data/morroneT/igraph_results/results/global_stats/wrapped_ether/giant_stats.csv


run_hub_authority_global: pagerank_score_global global_stats_dir
	# Calcolo degli Hub e Authority Scores per DAI
	./pagerank_score_global null $(PAGERANK_GRAPH_DAI) null \
	null $(HUB_AUTH_DIR_DAI)/hub_authority_dai.csv null
	echo "Hub e Authority Scores per DAI salvati in $(HUB_AUTH_DIR_DAI)"

	# Calcolo degli Hub e Authority Scores per Tether
	./pagerank_score_global null $(PAGERANK_GRAPH_TETHER) null \
	null $(HUB_AUTH_DIR_TETHER)/hub_authority_tether.csv null
	echo "Hub e Authority Scores per Tether salvati in $(HUB_AUTH_DIR_TETHER)"

	# Calcolo degli Hub e Authority Scores per USDC
	./pagerank_score_global null $(PAGERANK_GRAPH_USDC) null \
	null $(HUB_AUTH_DIR_USDC)/hub_authority_usdc.csv null
	echo "Hub e Authority Scores per USDC salvati in $(HUB_AUTH_DIR_USDC)"

	# Calcolo degli Hub e Authority Scores per Wrapped Ether
	./pagerank_score_global null $(PAGERANK_GRAPH_WRAPPED_ETHER) null \
	null $(HUB_AUTH_DIR_WRAPPED_ETHER)/hub_authority_wrapped_ether.csv null
	echo "Hub e Authority Scores per Wrapped Ether salvati in $(HUB_AUTH_DIR_WRAPPED_ETHER)"

run_microvelocity_community_global: pagerank_score_global global_stats_dir
	# Calcolo della Microvelocity e Comunità per DAI
	./pagerank_score_global null $(GLOBAL_DIR_DAI)/graph_dai.txt $(GLOBAL_DIR_DAI)/edgelist_dai.txt \
	#null null $(HUB_AUTH_DIR_DAI)/microvelocity_community_dai.csv
	#echo "Microvelocity e Comunità per DAI salvati in $(HUB_AUTH_DIR_DAI)"

	# Calcolo della Microvelocity e Comunità per Tether
	./pagerank_score_global null $(GLOBAL_DIR_TETHER)/graph_tether.txt $(GLOBAL_DIR_TETHER)/edgelist_tether.txt \
	null null $(HUB_AUTH_DIR_TETHER)/microvelocity_community_tether.csv
	echo "Microvelocity e Comunità per Tether salvati in $(HUB_AUTH_DIR_TETHER)"

	# Calcolo della Microvelocity e Comunità per USDC
	./pagerank_score_global null $(GLOBAL_DIR_USDC)/graph_usdc.txt $(GLOBAL_DIR_USDC)/edgelist_usdc.txt \
	null null $(HUB_AUTH_DIR_USDC)/microvelocity_community_usdc.csv
	echo "Microvelocity e Comunità per USDC salvati in $(HUB_AUTH_DIR_USDC)"

	# Calcolo della Microvelocity e Comunità per Wrapped Ether
	./pagerank_score_global null $(GLOBAL_DIR_WRAPPED_ETHER)/graph_wrapped_ether.txt $(GLOBAL_DIR_WRAPPED_ETHER)/edgelist_wrapped_ether.txt \
	null null $(HUB_AUTH_DIR_WRAPPED_ETHER)/microvelocity_community_wrapped_ether.csv
	echo "Microvelocity e Comunità per Wrapped Ether salvati in $(HUB_AUTH_DIR_WRAPPED_ETHER)"


run_assortativity_global: assortativity_global global_stats_dir
	# Calcolo per DAI
	./assortativity_global $(GLOBAL_DIR_DAI)/collapsed_dai.txt \
	$(DEGREE_DIR_DAI)/degree_dai.csv $(STATS_DIR_DAI)/assortativity_dai.csv
	echo "Assortatività per DAI salvata in $(STATS_DIR_DAI)"

	# Calcolo per Tether
	./assortativity_global $(GLOBAL_DIR_TETHER)/collapsed_tether.txt \
	$(DEGREE_DIR_TETHER)/degree_tether.csv $(STATS_DIR_TETHER)/assortativity_tether.csv
	echo "Assortatività per Tether salvata in $(STATS_DIR_TETHER)"

	# Calcolo per USDC
	./assortativity_global $(GLOBAL_DIR_USDC)/collapsed_usdc.txt \
	$(DEGREE_DIR_USDC)/degree_usdc.csv $(STATS_DIR_USDC)/assortativity_usdc.csv
	echo "Assortatività per USDC salvata in $(STATS_DIR_USDC)"

	# Calcolo per Wrapped Ether
	./assortativity_global $(GLOBAL_DIR_WRAPPED_ETHER)/collapsed_wrapped_ether.txt \
	$(DEGREE_DIR_WRAPPED_ETHER)/degree_wrapped_ether.csv $(STATS_DIR_WRAPPED_ETHER)/assortativity_wrapped_ether.csv
	echo "Assortatività per Wrapped Ether salvata in $(STATS_DIR_WRAPPED_ETHER)"

run_degree_global: degree global_stats_dir global_results_dir
	@for coin in $(STABLECOINS); do \
		if [ "$$coin" = "dai" ]; then \
				graph_file=$(PAGERANK_GRAPH_DAI); \
				output_file=$(DEGREE_DIR_DAI)/degree_dai.csv; \
		elif [ "$$coin" = "usdc" ]; then \
				graph_file=$(PAGERANK_GRAPH_USDC); \
				output_file=$(DEGREE_DIR_USDC)/degree_usdc.csv; \
		elif [ "$$coin" = "wrapped_ether" ]; then \
				graph_file=$(PAGERANK_GRAPH_WRAPPED_ETHER); \
				output_file=$(DEGREE_DIR_WRAPPED_ETHER)/degree_wrapped_ether.csv; \
		elif [ "$$coin" = "tether" ]; then \
			graph_file=$(PAGERANK_GRAPH_TETHER); \
			output_file=$(DEGREE_DIR_TETHER)/degree_tether.csv; \
		fi; \
		echo "Calculating global degrees for $$coin..."; \
		./degree $$graph_file $$output_file; \
		echo "Global degree results saved in $$output_file"; \
	done


clean:
	@rm -rf *.o builder degree components power_law split_transfers pagerank pagerank_score_global builder_global assortativity_global degree_giant_wcc_components
	@rm -rf $(RESULTS) $(CHUNKS) $(GRAPHS_DIR)
