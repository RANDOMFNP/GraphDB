// Derivative work from: https://www.phoenixdata.ai/glossary/depth-first-search-dfs

namespace graphdb {

template<typename node>

std::vector<node> dfs_algorithm(const node& starting_value, const std::string& input_file) {
    std::unordered_set<node> visited;
    std::stack<node> stack_of_numbers;
    flat_map<node, std::vector<node>> graph;
    graph = parse<node>(input_file);
    
    stack_of_numbers.emplace(starting_value);

    std::vector<node> return_vector;
    return_vector.reserve(graph.size());

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
        for (const auto& neighbor : it->second) {
            stack_of_numbers.emplace(neighbor);
        }
    }
    return return_vector;
}

// Added in-memory abilities
template<typename node>

std::vector<node> dfs_algorithm(const node& starting_value, const flat_map<node, std::vector<node>>& graph) {
    std::unordered_set<node> visited;
    std::stack<node> stack_of_numbers;
    
    stack_of_numbers.emplace(starting_value);

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
        for (const auto& neighbor : it->second) {
            stack_of_numbers.emplace(neighbor);
        }
    }
    return return_vector;
}
}