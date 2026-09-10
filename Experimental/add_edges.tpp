#pragma once
namespace graphdb {
    
template<typename node>

std::optional<flat_map<node, Node<node>>> add_edge(const Node<node>& new_value, const node& key, const std::variant<std::string, flat_map<node, Node<node>>> input_graph) {
    flat_map<node, Node<node>> graph;

    if (std::holds_alternative<std::string>(input_graph)) {
        graph = parse(std::get<std::string>(input_graph));
    } else {
        graph = std::get<flat_map<node, Node<node>>>(input_graph);
    }

    auto g_it = graph.find(key);

    if (g_it == graph.end()) {
        return std::nullopt;
    }

    for (const auto &new_edge : new_value.edges) {
        auto ex = std::find_if(g_it->second.edges.begin(), g_it->second.edges.end(), [&new_edge](auto edge) {return edge.target == new_edge.target;});
        if (ex == g_it->second.edges.end()) {
            g_it->second.edges.push_back(new_edge);
        }
    }
    return graph;
}

}