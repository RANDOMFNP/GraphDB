namespace graphlib {

// Derivative work from: https://www.phoenixdata.ai/glossary/breadth-first-search-bfs

template<typename node, typename weights>

requires Number<weights>
std::vector<node> dijkstras_algorithm(const node starting_node, const std::string& input_file) {
    std::unordered_map<node, std::vector<std::pair<node, weights>>> graph;
    graph = parse_weighted<node, weights>(input_file);

    std::priority_queue<std::pair<weights, node>, std::vector<std::pair<weights, node>>, std::greater<std::pair<weights, node>>> q;
    std::unordered_map<node, weights> visited;
    visited[starting_node] = 0;
    q.push(std::make_pair(0, starting_node));

    std::vector<node> return_graph;

    while (!q.empty()) {
        auto [dist, current] = q.top();
        q.pop();

        auto v_it = visited.find(current);
        if (v_it != visited.end() && dist > v_it->second) {
            continue;
        }

        return_graph.push_back(current);

        auto g_it = graph.find(current);
        if (g_it == graph.end()) {
            continue;
        }

        for (const auto& [n, w] : g_it->second) {
            weights new_distance = dist + w;
            
            auto n_it = visited.find(n);

            if (n_it == visited.end() || new_distance < n_it->second) {
                visited[n] = new_distance;
                q.push({new_distance, n});
            }
        }
    }
    return return_graph;
}
}