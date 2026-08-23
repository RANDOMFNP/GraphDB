namespace graphlib {
    
template<typename node, typename weights>

requires Number<weights>
void add_edge(std::vector<std::pair<node, weights>> new_value, node key, std::string input_file) {
    std::unordered_map<node, std::vector<std::pair<node, weights>>> graph;
    graph = parse_weighted<node, weights>(input_file);
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        std::cout << "Key doesnt exist" << std::endl;
        return;
    }

    for (auto [neighbor, weight] : new_value) {
        auto ex = std::find_if(g_it->second.begin(), g_it->second.end(), [neighbor](auto edge) {return edge.first == neighbor;});
        if (ex == g_it->second.end()) {
            g_it->second.push_back({neighbor, weight});
        }
    }
    create_graph<node, weights>(graph, input_file);
    }
}
}