#include <bits/stdc++.h>
#include "graphlib.hpp"
#include <regex>

using namespace std;

void delete_instances(const string& node_to_delete, const string& input_file) {
    vector<string> lines; 
    string line;
    regex nodetodeletepattern("^Node " + node_to_delete + "\\b");
    regex nodetodeletepattern2("\\bNode " + node_to_delete + "\\b");
    regex removetrailingcomma(",\\s*\\r?$");
    ifstream in(input_file);

    while (getline(in, line)) {
        if (regex_search(line, nodetodeletepattern)) {
            continue;
        }
        string cleaned = regex_replace(line, nodetodeletepattern2, "");
        cleaned = regex_replace(cleaned, removetrailingcomma, "");
        lines.push_back(cleaned);
    }
    in.close();

    ofstream out(input_file);
    for (const string& remaining_line : lines) {
        if (!remaining_line.empty()) {
            out << remaining_line << "\n";
        }
    }
    out.close();
}