namespace graphdb {
    
template<typename node, typename weights>

std::optional<std::vector<node>> get_neighbors(const node& key, flat_map<node, std::vector<std::pair<node, weights>>>& graph) {

    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << "\n";
        return std::nullopt;
    }

    std::vector<node> neighbors;

    for (const auto& edge_pair : g_it->second) {
        neighbors.push_back(edge_pair.first);
    }

    return neighbors;
}
}