namespace graphlib {
    
template<typename node>

void add_edge(const std::vector<node>& new_value, const node& key, const std::string& input_file) {
    std::unordered_map<node, std::vector<node>> graph;
    graph = parse<node>(input_file);
        
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << "\n";
        return;
    }

    for (const auto& neighbor : new_value) {
        if (std::find(g_it->second.begin(), g_it->second.end(), neighbor) == g_it->second.end()) {
            g_it->second.push_back(neighbor);
        }
    }
    create_graph<node>(graph, input_file);
}

// Added in-memory abilities
template<typename node>

void add_edge(const std::vector<node>& new_value, const node& key, std::unordered_map<node, std::vector<node>>& graph, std::string& input_file) {
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << "\n";
        return;
    }

    for (const auto& neighbor : new_value) {
        if (std::find(g_it->second.begin(), g_it->second.end(), neighbor) == g_it->second.end()) {
            g_it->second.push_back(neighbor);
        }
    }
    create_graph<node>(graph, input_file);
}
}