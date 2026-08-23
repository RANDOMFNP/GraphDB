// CREDITS: https://www.phoenixdata.ai/glossary/breadth-first-search-bfs

namespace graphlib {

template<typename node>

std::vector<node> bfs_algorithm(const node& starting_node, const std::string& input_file) {
    std::unordered_map<node, std::vector<node>> graph;
    graph = parse<node>(input_file);

    std::queue<node> q;
    std::unordered_set<node> visited;
    visited.insert(starting_node);
    q.push(starting_node);

    std::vector<node> return_graph;

    while (!q.empty()) {
        node node2 = q.front();
        q.pop();
        return_graph.push_back(node2);

        auto g_it = graph.find(node2);
        if (g_it == graph.end()) {
            continue;
        }

        for (const auto& neighbor : g_it->second) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return return_graph;
}
}