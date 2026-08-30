namespace graphlib {

template <typename node, typename weights>

requires Number<weights>
std::unordered_map<node, std::vector<std::pair<node, weights>>> undirected_connect(node key1, node key2, std::string input_file) {
    auto graph = parse_weighted<node, weights>(input_file);

    auto g_it = graph.find(key1);
    if (g_it != graph.end()) {
        g_it->second.push_back(key2);
    }

    auto g_it2 = graph.find(key2);
    if (g_it2 != graph.end()) {
        g_it->second.push_back(key1);
    }

    return graph;
}

template <typename node, typename weights>

requires Number<weights>
std::unordered_map<node, std::vector<std::pair<node, weights>>> undirected_connect(std::unordered_map<node, std::vector<std::pair<node, weights>>>& graph, node key1, node key2, std::string input_file) {
    auto g_it = graph.find(key1);
    if (g_it != graph.end()) {
        g_it->second.push_back(key2);
    }

    auto g_it2 = graph.find(key2);
    if (g_it2 != graph.end()) {
        g_it->second.push_back(key1);
    }

    return graph;
}
}