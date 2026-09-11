#pragma once
namespace graphdb {
    template<typename node>
    flat_map<node, Node<node>> parse(const std::string& input_file) {
        flat_map<node, Node<node>> output_unordered_map;

        std::ifstream file(input_file);
        std::string line;

        bool reading_graph = false;

        while (std::getline(file, line)) {

            if (line == "GRAPH_DEFINITION") {
                reading_graph = true;
                continue;
            }

            if (line == "GRAPH_DEFINITION_END") {
                reading_graph = false;
                continue;
            }

            if (!reading_graph) {
                continue;
            }

            std::string cleaned = std::regex_replace(
                line,
                std::regex(R"(\bNode\b|->|,|\[|\]|weight:|\s+)"),
                " "
            );

            std::stringstream ss(cleaned);

            node src;
            if (!(ss >> src)) {
                continue;
            }

            output_unordered_map[src];

            node dst;
            while (ss >> dst) {
                Edge<node> edge;
                edge.target = dst;

                output_unordered_map[src].edges.push_back(edge);
            }
        }

        return output_unordered_map;
    }
}