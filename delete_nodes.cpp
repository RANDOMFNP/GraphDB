#include <bits/stdc++.h>
#include "graphlib.hpp"
#include <regex>

using namespace std;

void delete_instances(const string& node_to_delete, const string& input_file) {
    vector<string> lines; 
    string line;
    regex nodetodeletepattern(node_to_delete);
    ifstream in(input_file);

    while (getline(in, line)) {
        if (!regex_search(line, nodetodeletepattern)) {
            lines.push_back(line);
        }
    }
    in.close();

    auto it = find(lines.begin(), lines.end(), node_to_delete);
    int index = distance(lines.begin(), it);

    if (index == lines.size()) {
        return;
    }

    ofstream out(input_file);
    for (int i = 0; i < lines.size(); i++) {
        if (i != index) out << lines[i] << "\n";
    }
    out.close();
}