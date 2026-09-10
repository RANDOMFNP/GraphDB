#pragma once
namespace graphdb {

    template <typename node>
    flat_map<node, Node<node>> undirected_connect(
        node key1,
        node key2,
        std::variant<std::string, flat_map<node, Node<node>>> input_graph,
        std::optional<double> weight = std::nullopt
    ) {
        flat_map<node, Node<node>> graph;

        if (std::holds_alternative<std::string>(input_graph)) {
            graph = parse(std::get<std::string>(input_graph));
        } else {
            graph = std::get<flat_map<node, Node<node>>>(input_graph);
        }

        auto g_it = graph.find(key1);

        if (g_it != graph.end()) {
            g_it->second.edges.push_back(
                Edge<node>{key2, weight, {}}
            );
        }

        auto g_it2 = graph.find(key2);

        if (g_it2 != graph.end()) {
            g_it2->second.edges.push_back(
                Edge<node>{key1, weight, {}}
            );
        }

        return graph;
    }
}
