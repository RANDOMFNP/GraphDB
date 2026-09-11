// Derivative work from: https://www.phoenixdata.ai/glossary/depth-first-search-dfs
#pragma once
namespace graphdb {

template<typename node>

std::vector<node> dfs_algorithm(const node& starting_value, const std::variant<std::string, flat_map<node, Node<node>>>& input_graph) {
    flat_map<node, Node<node>> graph;

    if (std::holds_alternative<std::string>(input_graph)) {
        graph = parse<node>(std::get<std::string>(input_graph));
    } else {
        graph = std::get<flat_map<node, Node<node>>>(input_graph);
    }
    std::unordered_set<node> visited;
    std::stack<node> stack_of_numbers;
    
    stack_of_numbers.push(starting_value);

    std::vector<node> return_vector;

    while (!stack_of_numbers.empty()) {
        node current = stack_of_numbers.top();
        stack_of_numbers.pop();

        if (visited.contains(current)) {
            continue;
        }

        visited.insert(current);
        return_vector.push_back(current);

        auto it = graph.find(current);
        if (it == graph.end()) {
            continue;
        }
        for (const auto& edge : it->second.edges) {
            stack_of_numbers.push(edge.target);
        }
    }
    return return_vector;
}

    template<typename node>

std::vector<node> bounded_cost_algorithm(const node& starting_value, const std::variant<std::string, flat_map<node, Node<node>>>& input_graph) {
    flat_map<node, Node<node>> graph;

    if (std::holds_alternative<std::string>(input_graph)) {
        graph = parse<node>(std::get<std::string>(input_graph));
    } else {
        graph = std::get<flat_map<node, Node<node>>>(input_graph);
    }
    std::unordered_set<node> visited;
    std::stack<node> stack_of_numbers;

    stack_of_numbers.push(starting_value);

    std::vector<node> return_vector;

    while (!stack_of_numbers.empty()) {
        node current = stack_of_numbers.top();
        stack_of_numbers.pop();

        if (visited.contains(current)) {
            continue;
        }

        visited.insert(current);
        return_vector.push_back(current);

        auto it = graph.find(current);
        if (it == graph.end()) {
            continue;
        }
        for (const auto& edge : it->second.edges) {
            stack_of_numbers.push(edge.target);
        }
    }
    return return_vector;
}
}