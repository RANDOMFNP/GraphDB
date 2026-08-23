namespace graphlib {

template<typename node, typename weights>

requires Number<weights>
void create_graph(const std::unordered_map<node, std::vector<std::pair<node, weights>>>& graph, std::string output_file) {

    std::ofstream file(output_file);

    std::vector<node> NodeName;
    std::vector<std::vector<std::pair<node, weights>>> NodeConnects;

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
}