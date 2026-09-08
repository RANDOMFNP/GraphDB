namespace graphdb {

template <typename node>
flat_map<node, std::vector<node>> undirected_connect(node key1, node key2, std::string input_file) {
    auto graph = parse<node>(input_file);

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

template <typename node> 
flat_map<node, std::vector<node>> undirected_connect(flat_map<node, std::vector<node>>& graph, node key1, node key2, std::string input_file) {
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