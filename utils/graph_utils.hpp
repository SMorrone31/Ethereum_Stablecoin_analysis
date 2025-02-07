#ifndef GRAPH_UTILS_HPP
#define GRAPH_UTILS_HPP

#include <igraph.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

bool read_weighted_graph(const char* filename, igraph_t* graph, igraph_vector_t* num_trans_weights, igraph_vector_t* value_weights);
bool read_global_graph(const char* filename, igraph_t* graph, igraph_vector_t* weights);
bool read_unweighted_graph(const std::string& filename, igraph_t* graph);
bool read_global_graph_with_timestamp(const std::string& filename, igraph_t* graph, igraph_vector_t* weights, std::vector<std::string>& timestamps);

#endif 
