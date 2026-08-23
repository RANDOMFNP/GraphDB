#pragma once

#ifndef GRAPHLIB_HPP
#define GRAPHLIB_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <concepts>
#include <utility>
#include <regex>
#include <stack>
#include <queue>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>

namespace graphlib {

template<typename T>

concept Number = std::integral<T> || std::floating_point<T>;

template<typename node>
void create_graph(const std::unordered_map<node, std::vector<node>>& graph, const std::string& output_file);
template<typename node>
void add_nodes(const std::unordered_map<node, std::vector<node>>& graph, const std::string& input_file);
template<typename node>
void delete_instances(const std::string& node_to_delete, const std::string& input_file);

void print_graph(const std::string& filename);

template<typename node, typename weights>
requires Number<weights>
void create_graph(const std::unordered_map<node, std::vector<std::pair<node, weights>>>& graph, const std::string& output_file);
template<typename node, typename weights>
requires Number<weights>
void add_nodes(const std::unordered_map<node, std::vector<std::pair<node, weights>>>& graph, const std::string& input_file);

void delete_instances_weighted(const std::string& node_to_delete, const std::string& input_file);

template<typename node, typename weights>
requires Number<weights>
void add_edge(const std::vector<std::pair<node, weights>>& new_value, const node& key, const std::string& input_file);
template<typename node>
void add_edge(const std::vector<node>& new_value, const node& key, const std::string& input_file);

template<typename node>
std::unordered_map<node, std::vector<node>> parse(const std::string& input_file);

template<typename node, typename weights>
requires Number<weights>
std::unordered_map<node, std::vector<std::pair<node, weights>>> parse_weighted(const std::string& input_file);

template<typename node>
std::vector<node> dfs_algorithm(const node& starting_value, const std::string& input_file);
template<typename node>
std::vector<node> bfs_algorithm(const node& starting_node, const std::string& input_file);
}

#include "../detail/txt_to_un_map.tpp"
#include "../detail/txt_to_un_map_weighted.tpp"

#include "../detail/add_nodes.tpp"
#include "../detail/add_nodes_weighted.tpp"
#include "../detail/create_graph.tpp"
#include "../detail/create_graph_weighted.tpp"
#include "../detail/delete_nodes.tpp"
#include "../detail/delete_nodes_weighted.tpp"
#include "../detail/add_edges.tpp"
#include "../detail/add_edges_weighted.tpp"

#include "../detail/dfs.tpp"
#include "../detail/bfs.tpp"
#include "../detail/dijkstra.tpp"
#include "../detail/print_graph.tpp"

#endif