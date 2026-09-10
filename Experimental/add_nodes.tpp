#pragma once
namespace graphdb {
    template<typename node>
        requires std::formattable<node, char>

    void add_nodes(const flat_map<node, Node<node> > &graph, const std::string &input_file,
              std::optional<int> buffer_size_in_bytes) {
        std::FILE *file = fopen(input_file.c_str(), "a");

        int buf_size;
        std::string write_buffer;

        if (!buffer_size_in_bytes.has_value()) {
            buf_size = 64 * 1024;
            write_buffer.reserve(64 * 1024);
        } else {
            buf_size = buffer_size_in_bytes.value();
            write_buffer.reserve(buf_size);
        }

        write_buffer += "GRAPH_DEFINITION\n";

        for (const auto &[NodeName, NodeConnects]: graph) {
            std::string connections = "";

            for (size_t j = 0; j < NodeConnects.edges.size(); j++) {
                connections += std::format("Node {}", NodeConnects.edges[j].target);

                if (NodeConnects.edges[j].weight.has_value()) {
                    connections += std::format(" {}", NodeConnects.edges[j].weight.value());
                }

                if (j < NodeConnects.edges.size() - 1) {
                    connections += ", ";
                }
            }
            write_buffer += std::format("Node {} -> {}\n", NodeName, connections);

            if (write_buffer.size() >= buf_size) {
                std::fwrite(write_buffer.data(), 1, write_buffer.size(), file);
                write_buffer.clear();
            }
        }

        write_buffer += "GRAPH_DEFINITION_END\n";

        if (!write_buffer.empty()) {
            std::fwrite(write_buffer.data(), 1, write_buffer.size(), file);
        }

        std::fclose(file);
    }
}
