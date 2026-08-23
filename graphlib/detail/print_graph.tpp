namespace graphlib {

inline void print_graph(const std::string& filename) {
    std::string line;
    std::fstream in(filename);

    if (!in.is_open()) {
        std::cout << "ERROR: Could not open file '" << filename << "'\n";
    }

    while (getline(in, line)) {
        std::cout << line << "\n";
    }
    in.close();
}
}