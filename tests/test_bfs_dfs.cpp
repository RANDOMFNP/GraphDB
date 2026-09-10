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
        graphdb::Edge<int>{0, std::nullopt, {}}
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
            graphdb::Edge<int>{value, std::nullopt, {}}
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

    // graphdb::save(
    //    graph,
    //    "test.db"
    // );

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
}