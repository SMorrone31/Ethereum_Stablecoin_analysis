/*Questo codice calcolava le statistiche per la distribuzione power law, ma ho utilizzato poi plfit con python*/


/*#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <cmath>
#include <igraph.h>
#include <filesystem>
#include <algorithm>

using namespace std;

string extract_chunk_name(const string& file_path) {
    filesystem::path path(file_path);
    string stem = path.stem().string();  
    string prefix = "degree_graph_";  
    if (stem.find(prefix) == 0) {
        return stem.substr(prefix.length());  
    }
    return stem;  
}

bool all_same_value(const vector<igraph_real_t>& degrees) {
    return all_of(degrees.begin(), degrees.end(), [&](double d) { return d == degrees[0]; });
}

double stddev(const vector<igraph_real_t>& v) {
    double sum = 0.0, mean, variance = 0.0;
    int size = v.size();
    for (auto& num : v) {
        sum += num;
    }
    mean = sum / size;
    for (auto& num : v) {
        variance += pow(num - mean, 2);
    }
    variance /= size;

    return sqrt(variance);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <degree_dir> <output_file> <chunk_files...>\n";
        return 1;
    }
    FILE *output_file = fopen(argv[2], "a");
    if (!output_file) {
        cerr << "Error: could not open output file!\n";
        return 1;
    }
    fseek(output_file, 0, SEEK_END); 
    if (ftell(output_file) == 0) {
        fprintf(output_file, "chunk,InAlpha,OutAlpha,InXmin,OutXmin,InL,OutL,InD,OutD,InPvalue,OutPvalue\n");
    }
    for (int i = 3; i < argc; i++) {
        FILE *input_file = fopen(argv[i], "r");
        if (!input_file) {
            cerr << "Error: could not open input file " << argv[i] << "!\n";
            continue;
        }
        string chunk_name = extract_chunk_name(argv[i]);
        cout << "Reading input file for chunk: " << chunk_name << "\n";
        char *line_buf = NULL;
        size_t line_size = 0;
        vector<igraph_real_t> indegrees;
        vector<igraph_real_t> outdegrees;
        if (getline(&line_buf, &line_size, input_file) == -1) {
            cerr << "Error reading header from file " << argv[i] << "!\n";
            fclose(input_file);
            continue;
        }
        while (getline(&line_buf, &line_size, input_file) > 0) {
            char *token = strtok(line_buf, ",");
            int node_id = atoi(token);
            token = strtok(NULL, ",");
            indegrees.push_back((igraph_real_t) strtod(token, NULL));
            token = strtok(NULL, ",");
            outdegrees.push_back((igraph_real_t) strtod(token, NULL));
            strtok(NULL, ",");
            strtok(NULL, ",");
        }

        fclose(input_file);
        if (indegrees.size() < 5 || outdegrees.size() < 5) {
            cerr << "Not enough data for chunk: " << chunk_name << ". Skipping...\n";
            fprintf(output_file, "%s,NaN,NaN,NaN,NaN,NaN,NaN,NaN,NaN,0,0\n", chunk_name.c_str());
            continue;
        }
        if (all_same_value(indegrees) || all_same_value(outdegrees)) {
            cerr << "Invalid data (constant values) for chunk: " << chunk_name << ". Skipping...\n";
            fprintf(output_file, "%s,NaN,NaN,NaN,NaN,NaN,NaN,NaN,NaN,0,0\n", chunk_name.c_str());
            continue;
        }
        double indegree_stddev = stddev(indegrees);
        double outdegree_stddev = stddev(outdegrees);

        if (indegree_stddev < 1e-3 || outdegree_stddev < 1e-3) {
            cerr << "Low variance in degree data for chunk: " << chunk_name << ". Skipping...\n";
            fprintf(output_file, "%s,NaN,NaN,NaN,NaN,NaN,NaN,NaN,NaN,0,0\n", chunk_name.c_str());
            continue;
        }

        cout << "Finished reading input file for chunk: " << chunk_name << "\n";
        igraph_vector_t igraph_indegree, igraph_outdegree;
        igraph_real_t *ptr_in = (igraph_real_t *) indegrees.data();
        igraph_real_t *ptr_out = (igraph_real_t *) outdegrees.data();

        igraph_vector_init_array(&igraph_indegree, ptr_in, indegrees.size());
        igraph_vector_init_array(&igraph_outdegree, ptr_out, outdegrees.size());
        igraph_plfit_result_t model_in, model_out;
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                cout << "Fitting power law for in-degree...\n";
                if (igraph_power_law_fit(&igraph_indegree, &model_in, -1, 0) != IGRAPH_SUCCESS) {
                    cerr << "Error fitting power law for in-degree in chunk: " << chunk_name << "\n";
                    model_in.alpha = model_in.xmin = model_in.L = model_in.D = NAN;
                }
            }

            #pragma omp section
            {
                cout << "Fitting power law for out-degree...\n";
                if (igraph_power_law_fit(&igraph_outdegree, &model_out, -1, 0) != IGRAPH_SUCCESS) {
                    cerr << "Error fitting power law for out-degree in chunk: " << chunk_name << "\n";
                    model_out.alpha = model_out.xmin = model_out.L = model_out.D = NAN;
                }
            }
        }
        fprintf(output_file, "%s,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,0,0\n", 
                chunk_name.c_str(),
                model_in.alpha, model_out.alpha, 
                model_in.xmin, model_out.xmin, 
                model_in.L, model_out.L, 
                model_in.D, model_out.D);
        igraph_vector_destroy(&igraph_indegree);
        igraph_vector_destroy(&igraph_outdegree);

        cout << "Done. Results for chunk " << chunk_name << " saved to file.\n";
    }
    fclose(output_file);

    return 0;
}*/
