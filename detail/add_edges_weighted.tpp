namespace graphdb {
    
template<typename node, typename weights>

requires Number<weights>
std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> add_edge(const std::vector<std::pair<node, weights>>& new_value, const node& key, const std::string& input_file) {
    std::unordered_map<node, std::vector<std::pair<node, weights>>> graph;
    graph = parse_weighted<node, weights>(input_file);
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        return std::nullopt;
    }

    for (const auto &[neighbor, weight] : new_value) {
        auto ex = std::find_if(g_it->second.begin(), g_it->second.end(), [neighbor](auto edge) {return edge.first == neighbor;});
        if (ex == g_it->second.end()) {
            g_it->second.push_back({neighbor, weight});
        }
    }
    return graph;
}

template<typename node, typename weights>

requires Number<weights>
std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> add_edge(const std::vector<std::pair<node, weights>>& new_value, const node& key, std::unordered_map<node, std::vector<std::pair<node, weights>>>& graph) {
    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        return std::nullopt;
    }

    for (const auto &[neighbor, weight] : new_value) {
        auto ex = std::find_if(g_it->second.begin(), g_it->second.end(), [neighbor](auto edge) {return edge.first == neighbor;});
        if (ex == g_it->second.end()) {
            g_it->second.push_back({neighbor, weight});
        }
    }
    return graph;
}
}