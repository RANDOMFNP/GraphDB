#include <bits/stdc++.h>
#include "graphlib.hpp"

using namespace std;

void create_graph(const unordered_map<int, vector<int>>& graph, string output_file) {

    ofstream file(output_file);

    vector<int> NodeName;
    vector<vector<int>> NodeConnects;

    for (auto const& [key, value] : graph) {
        NodeName.push_back(key);
        NodeConnects.push_back(value);
    }

    
    for (int i = 0; i < graph.size(); i++) {
        file << "Node " << NodeName[i] << "-> ";

        for (size_t j = 0; j < NodeConnects[i].size(); j++) {
            file << "Node " << NodeConnects[i][j];
            if (j < NodeConnects[i].size() - 1) {
                file << ", ";
            }
        }
        file << "\n";
    }
    file.close();
}