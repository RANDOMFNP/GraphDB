#pragma once

#ifndef GRAPHLIB_HPP
#define GRAPHLIB_HPP

#include <bits/stdc++.h>

void create_graph(const std::unordered_map<int, std::vector<int>>& graph, std::string output_file);
void add_nodes(const std::unordered_map<int, std::vector<int>>& graph, std::string input_file);
void delete_instances(const std::string& node_to_delete, const std::string& input_file);
void print_graph(const std::string& filename);

#endif