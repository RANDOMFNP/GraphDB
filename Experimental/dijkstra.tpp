#pragma once
namespace graphdb {
    // Derivative work from: https://www.phoenixdata.ai/glossary/breadth-first-search-bfs

    template<typename node>

    std::vector<node> dijkstras_algorithm(const node starting_node, const std::variant<std::string, flat_map<node, Node<node>>>& input_graph) {
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

        std::priority_queue<std::pair<double, node>, std::vector<std::pair<double, node>>, std::greater<std::pair<double, node>>> q;
        flat_map<node, double> visited;
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

            for (const auto& [edge] : g_it->second.edges) {
                if (!edge.weight.has_value()) {
                    continue;
                }
                const auto& n = edge.target;
                double w = edge.weight.value();

                double new_distance = dist + w;

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