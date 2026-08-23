namespace graphlib {

template<typename node, typename weights>

requires Number<weights>
std::unordered_map<node, std::vector<std::pair<node, weights>>> parse_weighted(const std::string& input_file) {
    std::unordered_map<node, std::vector<std::pair<node, weights>>> output_unordered_map;
    std::ifstream file(input_file);
    std::string line;
    
    while (std::getline(file, line)) {
        std::string cleaned = std::regex_replace(line, std::regex(R"(\bNode\b|->|,|\[|\]|weight:|\s+)"), " ");
        std::stringstream ss(cleaned);

        node src;
        if (!(ss >> src)) {
            continue;
        }

        output_unordered_map[src];

        node dst;
        weights weight;
        while (ss >> dst >> weight) {
            output_unordered_map[src].push_back({dst, weight});
        }
    }
    return output_unordered_map;
}
}