namespace graphdb {

template<typename node, typename weights>
    requires Number<weights> && std::formattable<node, char> && std::formattable<weights, char>
void add_nodes(const flat_map<node, std::vector<std::pair<node, weights>>>& graph, const std::string& input_file, std::optional<int> buffer_size_in_bytes) {
    std::FILE* file = fopen(input_file.c_str(), "a");

    int buf_size;
    std::string write_buffer;

    if (!buffer_size_in_bytes.has_value()) {
        buf_size = 64 * 1024;
        write_buffer.reserve(64 * 1024);
    } else {
        buf_size = buffer_size_in_bytes.value();
        write_buffer.reserve(buf_size);
    }

    for (const auto& [NodeName, NodeConnects] : graph) {
        std::string connections = "";

        for (size_t j = 0; j < NodeConnects.size(); j++) {
            connections += std::format("Node {} [weight: {}]", NodeConnects[j].first, NodeConnects[j].second);
            if (j < NodeConnects.size() - 1) {
                connections += ", ";
            }
        }
        write_buffer += std::format("Node {} -> {}\n", NodeName, connections);

        if (write_buffer.size() >= buf_size) {
            std::fwrite(write_buffer.data(), 1, write_buffer.size(), file);
            write_buffer.clear();
        }
    }

    if (!write_buffer.empty()) {
        std::fwrite(write_buffer.data(), 1, write_buffer.size(), file);
    }

    std::fclose(file);
}
}