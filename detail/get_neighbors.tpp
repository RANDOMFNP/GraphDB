namespace graphdb {
    
template<typename node>

std::optional<std::vector<node>> get_neighbors(const node& key, flat_map<node, std::vector<node>>& graph) {

    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << "\n";
        return std::nullopt;
    }

    return g_it->second;
}
}