#include <bits/stdc++.h>
#include "graphlib.hpp"

using namespace std;

void add_nodes(const unordered_map<int, vector<pair<int, int>>>& graph, string input_file) {
    ofstream file(input_file, ios::app);

    vector<int> NodeName;
    vector<vector<pair<int, int>>> NodeConnects;

    for (auto const& [key, value] : graph) {
        NodeName.push_back(key);
        NodeConnects.push_back(value);
    }


    for (int i = 0; i < graph.size(); i++) {
        file << "Node " << NodeName[i] << "-> ";

        for (size_t j = 0; j < NodeConnects[i].size(); j++) {
            file << "Node " << NodeConnects[i][j].first << "[" << "weight:" << NodeConnects[i][j].second << "]";
            if (j < NodeConnects[i].size() - 1) {
                file << ", ";
            }
        }
        file << "\n";
    }
    file.close();
}