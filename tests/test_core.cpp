#include <unordered_map>
#include <vector>
#include <string>
#include <utility>
#include <stack>
#include <queue>
#include <unordered_set>
#include <iostream>
#include <algorithm>
#include <ranges>
#include <chrono>
#include <cstdlib>

#include "graphdb.hpp"

struct Config {
    int iterations;
    int edges_per_node;
    int find_iterations;
};

int main() {

    // ============================================================
    // CONFIG
    // ============================================================

    Config config;
    config.iterations = 10'000;
    config.edges_per_node = 10'00;
    config.find_iterations = 100'000'000;


    // ============================================================
    // INITIALIZATION
    // ============================================================

    graphdb::flat_map<int, graphdb::Node<int>> graph;
    std::unordered_map<int, std::vector<int>> graph2;

    std::vector<int> to_be_appended;
    to_be_appended.reserve(config.edges_per_node);


    // ============================================================
    // CREATE VALUES TO APPEND
    // ============================================================

    for (int i = 0; i < config.edges_per_node; ++i) {
        to_be_appended.push_back(i);
    }


    // ============================================================
    // CREATE INITIAL GRAPH
    // ============================================================

    graphdb::Node<int> node0;

    node0.edges.push_back(
        graphdb::Edge<int>{0, 3, {}}
    );

    graph.insert(0, node0);

    graph2 = {{0, {0}}};


    // ============================================================
    // REMOVE INITIAL SELF-EDGE
    // ============================================================

    for (auto &[key, value] : graph) {

        auto it = std::ranges::find_if(
            value.edges,
            [key](const auto& edge) {
                return edge.target == key;
            }
        );

        if (it != value.edges.end()) {
            value.edges.erase(it);
        }
    }

    for (auto &[key, value] : graph2) {

        if (auto it = std::ranges::find(value, key);
            it != value.end()) {

            value.erase(it);
        }
    }


    // ============================================================
    // CREATE NODE TEMPLATE
    // ============================================================

    graphdb::Node<int> new_node;

    new_node.edges.reserve(config.edges_per_node);

    for (int value : to_be_appended) {

        new_node.edges.push_back(
            graphdb::Edge<int>{value, value*2, {}}
        );
    }


    // ============================================================
    // APPEND NODES
    // ============================================================

    for (int i = 0; i < config.iterations; ++i) {

        graph.insert(i, new_node);

        graph2.insert({
            i,
            to_be_appended
        });
    }


    // ============================================================
    // SORT GRAPH
    // ============================================================

    graph.sort();


    // ============================================================
    // SAVE GRAPH
    // ============================================================

    graphdb::save(
        graph,
        "test.db"
    );

    std::cout << "Graph created:\n";
    std::cout << "  Nodes: " << config.iterations << '\n';
    std::cout << "  Edges per node: "
              << config.edges_per_node << '\n';
    std::cout << "  Approximate edges: "
              << static_cast<long long>(config.iterations)
                 * config.edges_per_node
              << "\n\n";


    // ============================================================
    // BFS
    // ============================================================

    auto start =
        std::chrono::high_resolution_clock::now();

    auto search =
        graphdb::bfs_algorithm<int>(1, graph);

    auto end =
        std::chrono::high_resolution_clock::now();

    auto duration =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end - start);

    std::cout << "BFS: "
              << duration.count()
              << " microseconds\n";

    std::cout << "BFS visited: "
              << search.size()
              << " nodes\n";


    // ============================================================
    // DFS
    // ============================================================

    auto start2 =
        std::chrono::high_resolution_clock::now();

    auto dfs_search =
        graphdb::dfs_algorithm<int>(1, graph);

    auto end2 =
        std::chrono::high_resolution_clock::now();

    auto duration2 =
        std::chrono::duration_cast<
            std::chrono::microseconds
        >(end2 - start2);

    std::cout << "DFS: "
              << duration2.count()
              << " microseconds\n";

    std::cout << "DFS visited: "
              << dfs_search.size()
              << " nodes\n";


    // ============================================================
    // FLAT_MAP FIND BENCHMARK
    // ============================================================

    auto start3 =
        std::chrono::steady_clock::now();

    volatile std::size_t checksum = 0;

    graph.sort();

    for (int i = 0; i < config.find_iterations; ++i) {

        auto it =
            graph.find(i % config.iterations);

        if (it == graph.end()) {
            std::abort();
        }

        checksum += it->first;
    }

    auto end3 =
        std::chrono::steady_clock::now();


    // ============================================================
    // UNORDERED_MAP FIND BENCHMARK
    // ============================================================

    auto start4 =
        std::chrono::steady_clock::now();

    volatile std::size_t checksum2 = 0;

    for (int i = 0; i < config.find_iterations; ++i) {

        auto it =
            graph2.find(i % config.iterations);

        if (it == graph2.end()) {
            std::abort();
        }

        checksum2 += it->first;
    }

    auto end4 =
        std::chrono::steady_clock::now();


    // ============================================================
    // CHECKSUMS
    // ============================================================

    std::cout << "\nChecksums:\n";

    std::cout << "flat_map: "
              << checksum
              << '\n';

    std::cout << "unordered_map: "
              << checksum2
              << '\n';


    // ============================================================
    // FIND TIMINGS
    // ============================================================

    std::cout
        << "\nflat_map find: "
        << std::chrono::duration_cast<
            std::chrono::microseconds
        >(end3 - start3).count()
        << " microseconds\n";

    std::cout
        << "unordered_map find: "
        << std::chrono::duration_cast<
            std::chrono::microseconds
        >(end4 - start4).count()
        << " microseconds\n";


    // ============================================================
    // TRANSACTION TEST
    // ============================================================

    graphdb::flat_map<int, graphdb::Node<int>> graph3;

    graphdb::Node<int> transaction_node;

    transaction_node.edges.push_back(
        graphdb::Edge<int>{1, std::nullopt, {}}
    );

    graph3.insert(1, transaction_node);


    graphdb::Transaction<
        int,
        graphdb::Node<int>
    > graph_to_be_corrupted(graph3);


    // ============================================================
    // CORRUPT GRAPH
    // ============================================================

    graph_to_be_corrupted
        .return_graph()[1]
        .edges
        .clear();

    graph_to_be_corrupted
        .return_graph()[1]
        .edges
        .push_back(
            graphdb::Edge<int>{3132, std::nullopt, {}}
        );


    // ============================================================
    // ROLLBACK
    // ============================================================

    graph_to_be_corrupted.rollback();

    std::cout << "\nAfter rollback:\n";

    for (const auto &key :
         graph_to_be_corrupted.return_graph()
         | std::views::keys) {

        auto it = graph3.find(key);

        std::cout << "Node: "
                  << it->first
                  << '\n';

        for (const auto& edge : it->second.edges) {

            std::cout << "  Edge -> "
                      << edge.target
                      << '\n';
        }
    }


    // ============================================================
    // CORRUPT AGAIN
    // ============================================================

    graph_to_be_corrupted
        .return_graph()[1]
        .edges
        .clear();

    graph_to_be_corrupted
        .return_graph()[1]
        .edges
        .push_back(
            graphdb::Edge<int>{3132, std::nullopt, {}}
        );


    // ============================================================
    // COMMIT
    // ============================================================

    graph_to_be_corrupted.commit();

    std::cout << "\nAfter commit:\n";

    for (const auto &key :
         graph3 | std::views::keys) {

        auto it = graph3.find(key);

        std::cout << "Node: "
                  << it->first
                  << '\n';

        for (const auto& edge : it->second.edges) {

            std::cout << "  Edge -> "
                      << edge.target
                      << '\n';
        }
    }


    // ============================================================
    // SAVE TRANSACTION GRAPH
    // ============================================================

    auto g3 =
        graph_to_be_corrupted.return_graph();

    graphdb::save(
        g3,
        "test2.db",
        64 * 1024
    );


    std::cout << "\nTests complete.\n";

    return 0;
}
