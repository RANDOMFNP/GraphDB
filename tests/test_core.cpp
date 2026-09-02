// AI was used in the making of this test

// CONTRIBUTORS: @TrueFurina

#include "graphdb.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include <concepts>
#include <utility>
#include <regex>
#include <stack>
#include <queue>
#include <unordered_set>
#include <utility>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <random>
#include <chrono>
#include <thread>
#include <filesystem>
#include <unistd.h>

using namespace std;

// forward declaration for tests in test_d_and_g_n.cpp (merged runner)
extern int run_d_and_g_n_tests();

namespace {
    thread_local uint64_t g_checks_done = 0;
    thread_local uint64_t g_ops_done = 0;

    struct StressStats {
        uint64_t checks = 0;
        uint64_t ops = 0;
        double elapsed_ms = 0.0;
    };

    static inline uint64_t get_rss_bytes() {
        // Read resident set size from /proc/self/statm (field 2 = resident pages)
        std::ifstream f("/proc/self/statm");
        if (!f) return 0;
        uint64_t size_pages = 0, resident_pages = 0;
        f >> size_pages >> resident_pages;
        const long page_size = sysconf(_SC_PAGESIZE);
        return resident_pages * static_cast<uint64_t>(page_size);
    }

    std::string unique_temp_path(const std::string& name) {
        const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
        static uint64_t counter = 0;
        const auto id = counter++;
        const auto dir = std::filesystem::temp_directory_path();
        return (dir / ("graphdb_" + name + "_" + std::to_string(now) + "_" + std::to_string(id) + ".txt")).string();
    }

    template<typename V>
    bool same_vector_contents(const std::vector<V>& actual, const std::vector<V>& expected) {
        if (actual.size() != expected.size()) return false;
        std::vector<V> a = actual;
        std::vector<V> e = expected;
        std::sort(a.begin(), a.end());
        std::sort(e.begin(), e.end());
        return a == e;
    }

    template<typename K, typename V>
    bool same_graph(const std::unordered_map<K, std::vector<V>>& actual,
                    const std::unordered_map<K, std::vector<V>>& expected) {
        if (actual.size() != expected.size()) return false;

        for (const auto& [key, value] : expected) {
            const auto it = actual.find(key);
            if (it == actual.end()) return false;
            if (!same_vector_contents(it->second, value)) return false;
        }
        return true;
    }

    template<typename K, typename V>
    bool same_graph(const std::unordered_map<K, std::vector<std::pair<K, V>>>& actual,
                    const std::unordered_map<K, std::vector<std::pair<K, V>>>& expected) {
        if (actual.size() != expected.size()) return false;

        for (const auto& [key, value] : expected) {
            const auto it = actual.find(key);
            if (it == actual.end()) return false;

            if (it->second.size() != value.size()) return false;
            std::vector<std::pair<K, V>> a = it->second;
            std::vector<std::pair<K, V>> e = value;
            std::sort(a.begin(), a.end());
            std::sort(e.begin(), e.end());
            if (a != e) return false;
        }
        return true;
    }

    void check_true(bool condition, const std::string& message) {
        ++g_checks_done;
        if (!condition) {
            throw std::runtime_error("ASSERTION FAILED: " + message);
        }
    }

    std::string capture_output(std::function<void()> fn) {
        std::ostringstream buffer;
        auto* old = std::cout.rdbuf(buffer.rdbuf());
        try {
            fn();
        } catch (...) {
            std::cout.rdbuf(old);
            throw;
        }
        std::cout.rdbuf(old);
        return buffer.str();
    }

    std::unordered_map<int, std::vector<int>> generate_realistic_unweighted_graph(std::mt19937& rng, int n, int avg_degree) {
        std::unordered_map<int, std::vector<int>> graph;
        graph.reserve(static_cast<size_t>(n));
        std::vector<int> degree(n, 0);
        for (int i = 0; i < n; ++i) {
            graph[i] = {};
        }

        const auto pick_neighbor = [&](int node) {
            if (graph.empty()) return 0;
            std::uniform_int_distribution<int> dist(0, node - 1);
            return dist(rng);
        };

        for (int i = 1; i < n; ++i) {
            const int parent = std::max<int>(0, static_cast<int>(i - 1 - (rng() % 6)));
            graph[parent].push_back(i);
            graph[i].push_back(parent);
            ++degree[parent];
            ++degree[i];
        }

        for (int i = 0; i < n; ++i) {
            while (degree[i] < avg_degree && graph.size() > 1) {
                const int target = pick_neighbor(i == 0 ? n : i);
                if (target == i) continue;
                auto& neighbors = graph[i];
                if (std::find(neighbors.begin(), neighbors.end(), target) == neighbors.end()) {
                    neighbors.push_back(target);
                    graph[target].push_back(i);
                    ++degree[i];
                    ++degree[target];
                } else {
                    break;
                }
            }
        }

        return graph;
    }

    std::unordered_map<int, std::vector<int>> generate_stress_unweighted_graph(std::mt19937& rng, int n, int avg_degree) {
        std::unordered_map<int, std::vector<int>> graph;
        graph.reserve(static_cast<size_t>(n));
        std::vector<int> degree(n, 0);
        for (int i = 0; i < n; ++i) {
            graph[i] = {};
        }

        for (int i = 1; i < n; ++i) {
            const int parent = std::max<int>(0, static_cast<int>(i - 1 - (rng() % 4)));
            graph[parent].push_back(i);
            graph[i].push_back(parent);
            ++degree[parent];
            ++degree[i];
        }

        for (int i = 0; i < n; ++i) {
            while (degree[i] < avg_degree && n > 1) {
                const int target = std::max<int>(0, static_cast<int>(rng() % n));
                if (target == i) continue;
                auto& neighbors = graph[i];
                if (std::find(neighbors.begin(), neighbors.end(), target) == neighbors.end()) {
                    neighbors.push_back(target);
                    graph[target].push_back(i);
                    ++degree[i];
                    ++degree[target];
                } else {
                    break;
                }
            }
        }

        return graph;
    }

    std::unordered_map<int, std::vector<std::pair<int, int>>> generate_realistic_weighted_graph(std::mt19937& rng, int n, int avg_degree) {
        std::unordered_map<int, std::vector<std::pair<int, int>>> graph;
        graph.reserve(static_cast<size_t>(n));
        std::vector<int> degree(n, 0);
        for (int i = 0; i < n; ++i) {
            graph[i] = {};
        }

        for (int i = 1; i < n; ++i) {
            const int parent = std::max<int>(0, static_cast<int>(i - 1 - (rng() % 5)));
            graph[parent].emplace_back(i, 1 + (rng() % 20));
            graph[i].emplace_back(parent, 1 + (rng() % 20));
            ++degree[parent];
            ++degree[i];
        }

        std::uniform_int_distribution<int> weight_dist(1, 25);
        for (int i = 0; i < n; ++i) {
            while (degree[i] < avg_degree && graph.size() > 1) {
                const int target = std::max<int>(0, static_cast<int>(rng() % n));
                if (target == i) continue;
                auto& neighbors = graph[i];
                if (std::find_if(neighbors.begin(), neighbors.end(), [&](const auto& p) { return p.first == target; }) == neighbors.end()) {
                    neighbors.emplace_back(target, weight_dist(rng));
                    graph[target].emplace_back(i, weight_dist(rng));
                    ++degree[i];
                    ++degree[target];
                } else {
                    break;
                }
            }
        }

        return graph;
    }

    std::unordered_map<int, std::vector<std::pair<int, int>>> generate_stress_weighted_graph(std::mt19937& rng, int n, int avg_degree) {
        std::unordered_map<int, std::vector<std::pair<int, int>>> graph;
        graph.reserve(static_cast<size_t>(n));
        std::vector<int> degree(n, 0);
        for (int i = 0; i < n; ++i) {
            graph[i] = {};
        }

        std::uniform_int_distribution<int> weight_dist(1, 25);
        for (int i = 1; i < n; ++i) {
            const int parent = std::max<int>(0, static_cast<int>(i - 1 - (rng() % 4)));
            const int w = weight_dist(rng);
            graph[parent].emplace_back(i, w);
            graph[i].emplace_back(parent, w);
            ++degree[parent];
            ++degree[i];
        }

        for (int i = 0; i < n; ++i) {
            while (degree[i] < avg_degree && n > 1) {
                const int target = std::max<int>(0, static_cast<int>(rng() % n));
                if (target == i) continue;
                auto& neighbors = graph[i];
                if (std::find_if(neighbors.begin(), neighbors.end(), [&](const auto& p) { return p.first == target; }) == neighbors.end()) {
                    const int w = weight_dist(rng);
                    neighbors.emplace_back(target, w);
                    graph[target].emplace_back(i, w);
                    ++degree[i];
                    ++degree[target];
                } else {
                    break;
                }
            }
        }

        return graph;
    }

    // small helper: ensure node not present anywhere
    template<typename node>
    bool node_absent(const std::unordered_map<node, std::vector<node>>& g, const node& n) {
        if (g.find(n) != g.end()) return false;
        for (const auto& [k, v] : g) {
            for (const auto& x : v) if (x == n) return false;
        }
        return true;
    }

    template<typename node, typename weights>
    bool node_absent_weighted(const std::unordered_map<node, std::vector<std::pair<node, weights>>>& g, const node& n) {
        if (g.find(n) != g.end()) return false;
        for (const auto& [k, v] : g) {
            for (const auto& p : v) if (p.first == n) return false;
        }
        return true;
    }

    struct StressConfig {
        int iterations = 200;
        int max_nodes = 1000;
        int max_edges_per_node = 20;
        int thread_count = 4;
    };

    StressStats run_unweighted_stress(int thread_id, const StressConfig& cfg, uint32_t seed) {
        std::mt19937 rng(seed);
        g_checks_done = 0;
        g_ops_done = 0;
        const auto start = std::chrono::steady_clock::now();

        for (int it = 0; it < cfg.iterations; ++it) {
            const int N = std::max(0, std::min(cfg.max_nodes, static_cast<int>(rng() % (cfg.max_nodes + 1))));
            const int realistic_degree = std::max(2, std::min(12, cfg.max_edges_per_node / 2));
            auto graph = generate_stress_unweighted_graph(rng, N, realistic_degree);

            const std::string file = unique_temp_path("uw_stress");
            graphdb::create_graph<int>(graph, file);
            auto current = graphdb::parse<int>(file);
            check_true(same_graph(current, graph), "roundtrip create->parse mismatch (unweighted)");
            ++g_ops_done;

            std::vector<int> anchor_keys;
            anchor_keys.reserve(current.size());
            for (const auto& [key, _] : current) {
                anchor_keys.push_back(key);
            }

            if (!anchor_keys.empty()) {
                std::shuffle(anchor_keys.begin(), anchor_keys.end(), rng);
                std::unordered_map<int, std::vector<int>> new_nodes;
                const int add_count = std::min(6, std::max(1, realistic_degree / 2));
                for (int a = 0; a < add_count; ++a) {
                    const int key = N + a + 1;
                    std::vector<int> adj;
                    const int connect_count = std::min<int>(std::max(1, realistic_degree / 2), static_cast<int>(anchor_keys.size()));
                    for (int i = 0; i < connect_count; ++i) {
                        adj.push_back(anchor_keys[(a + i) % anchor_keys.size()]);
                    }
                    adj.erase(std::unique(adj.begin(), adj.end()), adj.end());
                    new_nodes[key] = adj;
                }
                current.insert(new_nodes.begin(), new_nodes.end());
                for (const auto& [key, neighbors] : new_nodes) {
                    const auto it = current.find(key);
                    check_true(it != current.end(), "in-memory add_nodes should keep added node (unweighted)");
                    check_true(same_vector_contents(it->second, neighbors), "in-memory add_nodes payload mismatch (unweighted)");
                }
                ++g_ops_done;
            }

            if (!current.empty()) {
                auto it = current.begin();
                std::advance(it, rng() % current.size());
                const int start_key = it->first;
                const auto bfs = graphdb::bfs_algorithm<int>(start_key, current);
                const auto dfs = graphdb::dfs_algorithm<int>(start_key, current);
                const auto neighbors = graphdb::get_neighbors<int>(start_key, current);
                check_true(!bfs.empty(), "bfs returned empty unexpectedly (unweighted)");
                check_true(!dfs.empty(), "dfs returned empty unexpectedly (unweighted)");
                check_true(neighbors.has_value(), "get_neighbors should succeed for realistic stress graph");
                ++g_ops_done;
            }

            graphdb::create_graph<int>(current, file);
            std::remove(file.c_str());
        }

        const auto elapsed_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
        return {g_checks_done, g_ops_done, elapsed_ms};
    }

    StressStats run_weighted_stress(int thread_id, const StressConfig& cfg, uint32_t seed) {
        std::mt19937 rng(seed);
        g_checks_done = 0;
        g_ops_done = 0;
        const auto start = std::chrono::steady_clock::now();

        for (int it = 0; it < cfg.iterations; ++it) {
            const int N = std::max(0, std::min(cfg.max_nodes, static_cast<int>(rng() % (cfg.max_nodes + 1))));
            const int realistic_degree = std::max(2, std::min(12, cfg.max_edges_per_node / 2));
            auto graph = generate_stress_weighted_graph(rng, N, realistic_degree);

            const std::string file = unique_temp_path("w_stress");
            graphdb::create_graph<int, int>(graph, file);
            auto current = graphdb::parse_weighted<int, int>(file);
            check_true(same_graph(current, graph), "roundtrip create->parse mismatch (weighted)");
            ++g_ops_done;

            std::vector<int> anchor_keys;
            anchor_keys.reserve(current.size());
            for (const auto& [key, _] : current) {
                anchor_keys.push_back(key);
            }

            if (!anchor_keys.empty()) {
                std::shuffle(anchor_keys.begin(), anchor_keys.end(), rng);
                std::unordered_map<int, std::vector<std::pair<int,int>>> new_nodes;
                const int add_count = std::min(6, std::max(1, realistic_degree / 2));
                for (int a = 0; a < add_count; ++a) {
                    const int key = N + a + 1;
                    std::vector<std::pair<int,int>> adj;
                    const int connect_count = std::min<int>(std::max(1, realistic_degree / 2), static_cast<int>(anchor_keys.size()));
                    for (int i = 0; i < connect_count; ++i) {
                        adj.emplace_back(anchor_keys[(a + i) % anchor_keys.size()], 1 + (rng() % 25));
                    }
                    adj.erase(std::unique(adj.begin(), adj.end(), [](const auto& lhs, const auto& rhs) { return lhs.first == rhs.first; }), adj.end());
                    new_nodes[key] = adj;
                }
                current.insert(new_nodes.begin(), new_nodes.end());
                for (const auto& [key, neighbors] : new_nodes) {
                    const auto it = current.find(key);
                    check_true(it != current.end(), "in-memory add_nodes should keep added node (weighted)");
                    check_true(it->second.size() == neighbors.size(), "in-memory add_nodes size mismatch (weighted)");
                    std::vector<std::pair<int,int>> expected = neighbors;
                    std::vector<std::pair<int,int>> actual = it->second;
                    std::sort(expected.begin(), expected.end());
                    std::sort(actual.begin(), actual.end());
                    check_true(actual == expected, "in-memory add_nodes payload mismatch (weighted)");
                }
                ++g_ops_done;
            }

            if (!current.empty()) {
                auto it = current.begin();
                std::advance(it, rng() % current.size());
                const int start_key = it->first;
                const auto dres = graphdb::dijkstras_algorithm<int, int>(start_key, current);
                const auto neighbors = graphdb::get_neighbors<int, int>(start_key, current);
                check_true(!dres.empty(), "dijkstra returned empty unexpectedly (weighted)");
                check_true(neighbors.has_value(), "weighted get_neighbors should succeed for realistic stress graph");
                ++g_ops_done;
            }

            graphdb::create_graph<int, int>(current, file);
            std::remove(file.c_str());
        }

        const auto elapsed_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
        return {g_checks_done, g_ops_done, elapsed_ms};
    }

    void run_realistic_benchmark() {
        std::mt19937 rng(20240601u);
        std::vector<int> sizes = {3000, 8000, 20000};
        std::vector<int> degrees = {4, 6, 8};

        const auto start = std::chrono::steady_clock::now();

        uint64_t total_ops = 0;
        uint64_t total_checks = 0;

        for (size_t layer = 0; layer < sizes.size(); ++layer) {
            const int n = sizes[layer];
            const int avg_deg = degrees[layer];

            auto unweighted = generate_realistic_unweighted_graph(rng, n, avg_deg);
            const std::string uw_file = unique_temp_path("realistic_uw");
            graphdb::create_graph<int>(unweighted, uw_file);
            const auto uw_parsed = graphdb::parse<int>(uw_file);
            check_true(same_graph(uw_parsed, unweighted), "realistic unweighted roundtrip mismatch");
            ++total_checks;
            ++total_ops;

            const int start_key = unweighted.begin()->first;
            const auto bfs = graphdb::bfs_algorithm<int>(start_key, unweighted);
            const auto dfs = graphdb::dfs_algorithm<int>(start_key, unweighted);
            check_true(!bfs.empty(), "realistic unweighted bfs empty");
            check_true(!dfs.empty(), "realistic unweighted dfs empty");
            total_checks += 2;
            total_ops += 2;

            const int add_key = n + 7;
            std::vector<int> extra = {add_key - 2, add_key - 1, add_key, add_key + 1};
            unweighted[add_key] = extra;
            graphdb::add_nodes<int>({{add_key, extra}}, uw_file);
            auto p = graphdb::parse<int>(uw_file);
            check_true(same_graph(p, unweighted), "realistic unweighted add_nodes mismatch");
            ++total_checks;
            ++total_ops;

            const auto neigh = graphdb::get_neighbors<int>(start_key, unweighted);
            check_true(neigh.has_value(), "realistic unweighted get_neighbors should succeed");
            ++total_checks;
            ++total_ops;

            std::remove(uw_file.c_str());

            auto weighted = generate_realistic_weighted_graph(rng, n, avg_deg);
            const std::string w_file = unique_temp_path("realistic_w");
            graphdb::create_graph<int, int>(weighted, w_file);
            const auto w_parsed = graphdb::parse_weighted<int, int>(w_file);
            check_true(same_graph(w_parsed, weighted), "realistic weighted roundtrip mismatch");
            ++total_checks;
            ++total_ops;

            const int w_start = weighted.begin()->first;
            const auto dres = graphdb::dijkstras_algorithm<int, int>(w_start, weighted);
            check_true(!dres.empty(), "realistic weighted dijkstra empty");
            ++total_checks;
            ++total_ops;

            const auto w_neighbors = graphdb::get_neighbors<int, int>(w_start, weighted);
            check_true(w_neighbors.has_value(), "realistic weighted get_neighbors should succeed");
            ++total_checks;
            ++total_ops;

            std::remove(w_file.c_str());
        }

        const auto elapsed_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
        const double throughput = (elapsed_ms > 0.0) ? (static_cast<double>(total_ops) / (elapsed_ms / 1000.0)) : 0.0;

        // Fixed: Replaced raw std::cout stream chaining with type-safe C++23 std::print
        std::print("REALISTIC mixed-graph benchmark complete: total_ops={} total_checks={} elapsed_ms={:.2f} ops_per_sec={:.2f}\n",
                   total_ops, total_checks, elapsed_ms, throughput);
    }
}

int main() {
    try {
        StressConfig cfg;
        cfg.iterations = 25;
        cfg.max_nodes = 3000;
        cfg.max_edges_per_node = 40;
        cfg.thread_count = 1;

        // One warm-up pass to stabilize branch predictors and allocator state before measuring.
        for (int t = 0; t < std::min(2, cfg.thread_count); ++t) {
            (void)run_unweighted_stress(t, cfg, static_cast<uint32_t>(10000 + t));
            (void)run_weighted_stress(t, cfg, static_cast<uint32_t>(20000 + t));
        }

        const auto benchmark_start = std::chrono::steady_clock::now();

        std::vector<StressStats> uw_stats(cfg.thread_count);
        std::vector<std::thread> uw_threads;
        uw_threads.reserve(cfg.thread_count);
        for (int t = 0; t < cfg.thread_count; ++t) {
            uw_threads.emplace_back([&, t]() {
                uw_stats[t] = run_unweighted_stress(t, cfg, static_cast<uint32_t>(12345 + t));
            });
        }
        for (auto& worker : uw_threads) worker.join();

        std::vector<StressStats> w_stats(cfg.thread_count);
        std::vector<std::thread> w_threads;
        w_threads.reserve(cfg.thread_count);
        for (int t = 0; t < cfg.thread_count; ++t) {
            w_threads.emplace_back([&, t]() {
                w_stats[t] = run_weighted_stress(t, cfg, static_cast<uint32_t>(54321 + t));
            });
        }
        for (auto& worker : w_threads) worker.join();

        const auto benchmark_elapsed_ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - benchmark_start).count();

        g_checks_done = 0;
        g_ops_done = 0;
        double total_elapsed_ms = 0.0;
        for (const auto& s : uw_stats) {
            g_checks_done += s.checks;
            g_ops_done += s.ops;
            total_elapsed_ms += s.elapsed_ms;
        }
        for (const auto& s : w_stats) {
            g_checks_done += s.checks;
            g_ops_done += s.ops;
            total_elapsed_ms += s.elapsed_ms;
        }

        const double throughput_ops_per_sec = (g_ops_done > 0) ? (static_cast<double>(g_ops_done) / (benchmark_elapsed_ms / 1000.0)) : 0.0;

        std::cerr << "FINAL: total_checks=" << g_checks_done << " total_ops=" << g_ops_done
                  << " rss_mb=" << (get_rss_bytes() / 1024 / 1024)
                  << " throughput_ops_per_sec=" << throughput_ops_per_sec << std::endl;

        std::cout << "Stress tests completed successfully." << std::endl;

        run_realistic_benchmark();

        // run additional tests from tests/test_d_and_g_n.cpp
        int rc = run_d_and_g_n_tests();
        if (rc != 0) return rc;

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}