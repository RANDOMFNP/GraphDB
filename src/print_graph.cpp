#include <bits/stdc++.h>
#include "graphlib.hpp"

using namespace std;

void print_graph(const string& filename) {
    string line;
    fstream in(filename);

    if (!in.is_open()) {
        cout << "ERROR: Could not open file '" << filename << "'";
    }

    while (getline(in, line)) {
        cout << line << "\n";
    }
    in.close();
}
