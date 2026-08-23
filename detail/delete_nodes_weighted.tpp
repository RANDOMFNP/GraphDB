namespace graphlib {

void delete_instances_weighted(const std::string& node_to_delete, const std::string& input_file) {
    std::vector<std::string> lines; 
    std::string line;
    std::regex nodetodeletepattern("^Node " + node_to_delete + "\\b");
    std::regex nodetodeletepattern2("\\bNode " + node_to_delete + "\\[[^\\]]*\\]");
    std::regex removetrailingcomma(",\\s*\\r?$");
    std::regex removedoublecomma(",?\\s\\r?$");
    std::ifstream in(input_file);

    while (getline(in, line)) {
        if (regex_search(line, nodetodeletepattern)) {
            continue;
        }
        std::string cleaned = regex_replace(line, nodetodeletepattern2, "");
        cleaned = regex_replace(cleaned, removetrailingcomma, "");
        cleaned = regex_replace(cleaned, removedoublecomma, "");
        lines.push_back(cleaned);
    }
    in.close();

    std::ofstream out(input_file);
    for (const std::string& remaining_line : lines) {
        if (!remaining_line.empty()) {
            out << remaining_line << "\n";
        }
    }
    out.close();
}
}