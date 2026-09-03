namespace graphdb {

template<typename node>

flat_map<node, std::vector<node>> parse(const std::string& input_file) {
    flat_map<node, std::vector<node>> output_unordered_map;
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
        while (ss >> dst) {
            output_unordered_map[src].push_back(dst);
        }
    }
    return output_unordered_map;
}
}