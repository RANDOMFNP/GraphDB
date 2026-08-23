namespace graphlib {

template<typename node>

void add_nodes(const std::unordered_map<node, std::vector<node>>& graph, std::string input_file) {
    std::ofstream file(input_file, std::ios::app);

    std::vector<node> NodeName;
    std::vector<std::vector<node>> NodeConnects;

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
}