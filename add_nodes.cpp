#include <bits/stdc++.h>

using namespace std;

void add_nodes(unordered_map<int, vector<int>> graph, string input_file) {
    ofstream file(input_file, ios::app);

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