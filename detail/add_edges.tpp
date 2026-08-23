namespace graphlib {
    
template<typename node>

void add_edge(std::vector<node> new_value, node key, std::string input_file) {
    std::unordered_map<node, std::vector<node>> graph;
    graph = parse<node>(input_file);
        
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << std::endl;
    }

    for (auto neighbor : new_value) {
        if (std::find(g_it->second.begin(), it->second.end(), neighbor) == it->second.end()) {
            it->second.push_back(neighbor)
        }
    }
    create_graph<node>(graph, input_file);
}
}