// CREDITS: https://www.phoenixdata.ai/glossary/breadth-first-search-bfs
#pragma once
namespace graphdb {

template<typename node>

std::vector<node> bfs_algorithm(const node& starting_node, const std::variant<std::string, flat_map<node, Node<node>>>& input_graph) {
    flat_map<node, Node<node>> parsed_graph;

    const flat_map<node, Node<node>>* graph_ptr;

    if (std::holds_alternative<std::string>(input_graph)) {
        parsed_graph = parse_weighted<node>(
            std::get<std::string>(input_graph)
        );

        graph_ptr = &parsed_graph;
    } else {
        graph_ptr = &std::get<flat_map<node, Node<node>>>(input_graph);
    }

    const auto& graph = *graph_ptr;


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

        for (const auto& edge : g_it->second.edges) {
            const auto& neighbor = edge.target;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }
    return return_graph;
}
}