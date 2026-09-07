#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include <stack>
#include <queue>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdio>
#include <ranges>
#include <chrono>

import graphdb;

struct Config {
    int iterations;
    int nodes;
    int threads;
};

int main() {
    // Config
    Config config;
    config.iterations = 10000;
    config.nodes = 10000;
    config.threads = 1;

    // Initialization
    graphdb::flat_map<int, std::vector<int> > graph = {{0, {0}}};
    std::unordered_map<int, std::vector<int> > graph2 = {{0, {0}}};
    std::vector<int> to_be_appended;

    // Create graph
    graphdb::create_graph(graph, "test.db");
    for (int i = 0; i < config.iterations; i++) {
        to_be_appended.push_back(i);
    }

    // Remove duplicates
    for (auto &[key, value] : graph) {
        if (auto it = std::ranges::find(value, key); it != value.end())
            value.erase(it);
    }

    for (auto &[key, value] : graph2) {
        if (auto it = std::ranges::find(value, key); it != value.end())
            value.erase(it);
    }


    // Append everything to the graph
    for (int i = 0; i < config.iterations; i++) {
        graph.insert(i, to_be_appended);
        graph2.insert({i, to_be_appended});
    }

    graph.sort();

    // Find speed of add_nodes
    auto add_nodes_start = std::chrono::high_resolution_clock::now();
    graphdb::add_nodes(graph, "test.db", 1024 * 1024);
    auto add_nodes_end = std::chrono::high_resolution_clock::now();

    auto add_nodes_duration = std::chrono::duration_cast<std::chrono::microseconds>(add_nodes_end - add_nodes_start);

    std::cout << add_nodes_duration.count() << std::endl;

    // Find speed of BFS
    auto start = std::chrono::high_resolution_clock::now();
    auto search = graphdb::bfs_algorithm(1, graph);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << duration.count() << std::endl;

    // Find speed of DFS
    auto start2 = std::chrono::high_resolution_clock::now();
    auto dfs_search = graphdb::dfs_algorithm(1, graph);
    auto end2 = std::chrono::high_resolution_clock::now();

    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2);
    std::cout << duration2.count() << std::endl;

    auto start3 = std::chrono::steady_clock::now();

    volatile std::size_t checksum = 0;
    // Tests flat_map's .find() method
    graph.sort();
    for (int i = 0; i < 100'000'000; ++i) {
        auto it = graph.find(i % 10'000);

        if (it == graph.end())
            std::abort();

        checksum += it->first;
    }

    auto end3 = std::chrono::steady_clock::now();

    auto start4 = std::chrono::steady_clock::now();

    volatile std::size_t checksum2 = 0;


    for (int i = 0; i < 100'000'000; ++i) {
        auto it = graph2.find(i % 10'000);

        if (it == graph2.end())
            std::abort();

        checksum2 += it->first;
    }

    auto end4 = std::chrono::steady_clock::now();

    // Checksums should equal the same value
    std::cout << "checksum: " << checksum << '\n';
    std::cout << "checksum2: " << checksum2 << '\n';

    std::cout
            << std::chrono::duration_cast<std::chrono::microseconds>(
                end3 - start3
            ).count()
            << '\n';

    std::cout
            << std::chrono::duration_cast<std::chrono::microseconds>(
                end4 - start4
            ).count()
            << '\n';

    // ERRORS & TRANSACTIONS

    auto graph3 = graphdb::flat_map<int, std::vector<int>>(1, {1});
    graphdb::Transaction<int, std::vector<int>> graph_to_be_corrupted(graph3);

    // Example corruption converting key 1 to 3132
    graph_to_be_corrupted.return_graph()[1] = {3132};

    graph_to_be_corrupted.rollback();

    for (const auto &key: graph_to_be_corrupted.return_graph() | std::views::keys) {
        auto it = graph3.find(key);
        // Should equal 1, 1
        std::cout << it->first << ", ";
        for (const auto& v : it->second) {
            std::cout << v << '\n';
        }
    }

    graph_to_be_corrupted.return_graph()[1] = {3132};
    graph_to_be_corrupted.commit();

    for (const auto &key: graph3 | std::views::keys) {
        auto it = graph3.find(key);
        // Should equal 1, 1
        std::cout << it->first << ", ";
        for (const auto& v : it->second) {
            std::cout << v << '\n';
        }
    }

    auto g3 = graph_to_be_corrupted.return_graph();

    // Tests save
    graphdb::save(g3, "test2.db");

}
