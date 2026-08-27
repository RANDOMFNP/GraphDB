#include "graphlib.hpp"
#include <bits/stdc++.h>
#include <chrono>
#include <thread>

using namespace std;

// forward declaration for tests in test_d_and_g_n.cpp (merged runner)
extern int run_d_and_g_n_tests();

namespace {

std::atomic<uint64_t> g_checks_done{0};
std::atomic<uint64_t> g_ops_done{0};

static inline uint64_t get_rss_bytes() {
    // Read resident set size from /proc/self/statm (field 2 = resident pages)
    std::ifstream f("/proc/self/statm");
    if (!f) return 0;
    uint64_t size_pages = 0, resident_pages = 0;
    f >> size_pages >> resident_pages;
    const long page_size = sysconf(_SC_PAGESIZE);
    return resident_pages * static_cast<uint64_t>(page_size);
}

std::atomic<bool> g_monitor_running{false};

void start_monitor(std::chrono::steady_clock::time_point start_time) {
    g_monitor_running = true;
    std::thread([start_time]() {
        uint64_t last_checks = 0;
        uint64_t last_ops = 0;
        auto last_time = std::chrono::steady_clock::now();
        while (g_monitor_running.load(std::memory_order_relaxed)) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start_time).count();
            double delta_t = std::chrono::duration<double>(now - last_time).count();
            uint64_t checks = g_checks_done.load(std::memory_order_relaxed);
            uint64_t ops = g_ops_done.load(std::memory_order_relaxed);
            double checks_per_sec = delta_t > 0 ? (checks - last_checks) / delta_t : 0.0;
            double ops_per_sec = delta_t > 0 ? (ops - last_ops) / delta_t : 0.0;
            uint64_t rss = get_rss_bytes();
            std::cerr << "[STATS] elapsed=" << (uint64_t)elapsed << "s"
                      << " checks/s=" << (uint64_t)checks_per_sec
                      << " total_checks=" << checks
                      << " ops/s=" << (uint64_t)ops_per_sec
                      << " total_ops=" << ops
                      << " rss_mb=" << (rss / 1024 / 1024)
                      << std::endl;
            last_checks = checks;
            last_ops = ops;
            last_time = now;
        }
    }).detach();
}

std::string unique_temp_path(const std::string& name) {
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::atomic<uint64_t> counter{0};
    const auto id = counter.fetch_add(1);
    return "/tmp/graphlib_" + name + "_" + std::to_string(now) + "_" + std::to_string(id) + ".txt";
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
    g_checks_done.fetch_add(1, std::memory_order_relaxed);
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

void run_unweighted_stress(int thread_id, const StressConfig& cfg, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> node_dist(0, cfg.max_nodes);
    std::uniform_int_distribution<int> edges_dist(0, cfg.max_edges_per_node);

    for (int it = 0; it < cfg.iterations; ++it) {
        const int N = std::max(0, node_dist(rng));
        std::unordered_map<int, std::vector<int>> graph;
        graph.reserve(N);
        for (int i = 0; i < N; ++i) {
            int m = edges_dist(rng);
            std::vector<int> adj;
            adj.reserve(m);
            for (int e = 0; e < m; ++e) {
                adj.push_back(rng() % (N + 1));
            }
            graph[i] = adj;
        }

        const std::string file = unique_temp_path("uw_create");
        graphlib::create_graph<int>(graph, file);
        const auto parsed = graphlib::parse<int>(file);
        check_true(same_graph(parsed, graph), "roundtrip create->parse mismatch (unweighted)");
        g_ops_done.fetch_add(1, std::memory_order_relaxed);

        // random add_nodes
        std::unordered_map<int, std::vector<int>> new_nodes;
        int add_count = std::min(10, cfg.max_nodes / 10 + 1);
        for (int a = 0; a < add_count; ++a) {
            int key = N + a + 1;
            int m = edges_dist(rng);
            std::vector<int> adj;
            for (int e = 0; e < m; ++e) adj.push_back(rng() % (N + add_count + 1));
            new_nodes[key] = adj;
        }
        graphlib::add_nodes<int>(new_nodes, file);
        for (auto &p : new_nodes) graph[p.first] = p.second;
        {
            const auto p2 = graphlib::parse<int>(file);
            check_true(same_graph(p2, graph), "add_nodes mismatch (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        // random edge replacement
        if (!graph.empty()) {
            int key = rng() % (N + add_count + 1);
            std::vector<int> new_adj;
            int m = edges_dist(rng);
            for (int e = 0; e < m; ++e) new_adj.push_back(rng() % (N + add_count + 1));

            // determine whether key exists before calling (current add_edge prints/returns if missing)
            bool key_exists = (graph.find(key) != graph.end());
            graphlib::add_edge<int>(new_adj, key, file);

            if (key_exists) {
                // add_edge appends neighbors (no duplicates) — merge into local model
                auto &vec = graph[key];
                for (auto n : new_adj) {
                    if (std::find(vec.begin(), vec.end(), n) == vec.end()) vec.push_back(n);
                }
            } else {
                // add_edge does nothing when key missing (implementation prints and returns)
                // local graph remains unchanged
            }

            const auto p3 = graphlib::parse<int>(file);
            check_true(same_graph(p3, graph), "add_edge mismatch (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        // random delete
        if (!graph.empty()) {
            int candidate = rng() % (N + add_count + 1);
            graphlib::delete_instances(std::to_string(candidate), file);
            // remove candidate locally
            graph.erase(candidate);
            for (auto &kv : graph) {
                auto &vec = kv.second;
                vec.erase(std::remove(vec.begin(), vec.end(), candidate), vec.end());
            }
            const auto p4 = graphlib::parse<int>(file);
            check_true(same_graph(p4, graph), "delete_instances mismatch (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
            check_true(node_absent<int>(p4, candidate), "deleted node still present (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        // traversal sanity
        if (!graph.empty()) {
            int start = rng() % (graph.size());
            // pick an existing key
            auto it = graph.begin();
            std::advance(it, start);
            const int start_key = it->first;
            auto bfs = graphlib::bfs_algorithm<int>(start_key, file);
            auto dfs = graphlib::dfs_algorithm<int>(start_key, file);
            check_true(!bfs.empty(), "bfs returned empty unexpectedly (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
            check_true(!dfs.empty(), "dfs returned empty unexpectedly (unweighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        std::remove(file.c_str());
    }
}

void run_weighted_stress(int thread_id, const StressConfig& cfg, uint32_t seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> node_dist(0, cfg.max_nodes);
    std::uniform_int_distribution<int> edges_dist(0, cfg.max_edges_per_node);
    std::uniform_int_distribution<int> weight_dist(1, 1000);

    for (int it = 0; it < cfg.iterations; ++it) {
        const int N = std::max(0, node_dist(rng));
        std::unordered_map<int, std::vector<std::pair<int, int>>> graph;
        graph.reserve(N);
        for (int i = 0; i < N; ++i) {
            int m = edges_dist(rng);
            std::vector<std::pair<int, int>> adj;
            adj.reserve(m);
            for (int e = 0; e < m; ++e) {
                adj.emplace_back(rng() % (N + 1), weight_dist(rng));
            }
            graph[i] = adj;
        }

        const std::string file = unique_temp_path("w_create");
        graphlib::create_graph<int, int>(graph, file);
        const auto parsed = graphlib::parse_weighted<int, int>(file);
        check_true(same_graph(parsed, graph), "roundtrip create->parse mismatch (weighted)");
        g_ops_done.fetch_add(1, std::memory_order_relaxed);

        // random add_nodes
        std::unordered_map<int, std::vector<std::pair<int, int>>> new_nodes;
        int add_count = std::min(10, cfg.max_nodes / 10 + 1);
        for (int a = 0; a < add_count; ++a) {
            int key = N + a + 1;
            int m = edges_dist(rng);
            std::vector<std::pair<int, int>> adj;
            for (int e = 0; e < m; ++e) adj.emplace_back(rng() % (N + add_count + 1), weight_dist(rng));
            new_nodes[key] = adj;
        }
        graphlib::add_nodes<int, int>(new_nodes, file);
        for (auto &p : new_nodes) graph[p.first] = p.second;
        {
            const auto p2 = graphlib::parse_weighted<int, int>(file);
            check_true(same_graph(p2, graph), "add_nodes mismatch (weighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        // random edge replacement
        if (!graph.empty()) {
            int key = rng() % (N + add_count + 1);
            std::vector<std::pair<int, int>> new_adj;
            int m = edges_dist(rng);
            for (int e = 0; e < m; ++e) new_adj.emplace_back(rng() % (N + add_count + 1), weight_dist(rng));

            bool key_exists = (graph.find(key) != graph.end());
            graphlib::add_edge<int, int>(new_adj, key, file);

            if (key_exists) {
                // add_edge appends weighted neighbors (no duplicate neighbor keys) — merge into local model
                auto &vec = graph[key];
                for (auto &p : new_adj) {
                    auto ex = std::find_if(vec.begin(), vec.end(), [&](const auto &e) { return e.first == p.first; });
                    if (ex == vec.end()) vec.push_back(p);
                }
            } else {
                // add_edge does nothing when key missing
            }

            const auto p3 = graphlib::parse_weighted<int, int>(file);
            if (!same_graph(p3, graph)) {
                std::cerr << "DEBUG: add_edge weighted mismatch\n";
                std::cerr << "file: " << file << " key: " << key << " key_exists: " << key_exists << "\n";
                std::cerr << "new_adj:\n";
                for (auto &pp : new_adj) std::cerr << "  (" << pp.first << "," << pp.second << ")\n";
                std::cerr << "expected adjacency for key:\n";
                if (graph.find(key) != graph.end()) {
                    for (auto &pp : graph[key]) std::cerr << "  (" << pp.first << "," << pp.second << ")\n";
                } else {
                    std::cerr << "  <missing>\n";
                }
                std::cerr << "actual adjacency for key in file:\n";
                auto itf = p3.find(key);
                if (itf != p3.end()) {
                    for (auto &pp : itf->second) std::cerr << "  (" << pp.first << "," << pp.second << ")\n";
                } else {
                    std::cerr << "  <missing>\n";
                }
                check_true(false, "add_edge mismatch (weighted)");
            } else {
                g_ops_done.fetch_add(1, std::memory_order_relaxed);
            }
        }

        // random delete
        if (!graph.empty()) {
            int candidate = rng() % (N + add_count + 1);
            graphlib::delete_instances_weighted(std::to_string(candidate), file);
            graph.erase(candidate);
            for (auto &kv : graph) {
                auto &vec = kv.second;
                vec.erase(std::remove_if(vec.begin(), vec.end(), [&](const auto &p) { return p.first == candidate; }), vec.end());
            }
            const auto p4 = graphlib::parse_weighted<int, int>(file);
            check_true(same_graph(p4, graph), "delete_instances mismatch (weighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
            check_true(node_absent_weighted<int, int>(p4, candidate), "deleted node still present (weighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        // dijkstra traversal sanity
        if (!graph.empty()) {
            int start = rng() % (graph.size());
            auto it = graph.begin();
            std::advance(it, start);
            const int start_key = it->first;
            auto dres = graphlib::dijkstras_algorithm<int, int>(start_key, file);
            check_true(!dres.empty(), "dijkstra returned empty unexpectedly (weighted)");
            g_ops_done.fetch_add(1, std::memory_order_relaxed);
        }

        std::remove(file.c_str());
    }
}

}  // namespace

int main() {
    try {
        StressConfig cfg;
        cfg.iterations = 50;
        cfg.max_nodes = 1000;
        cfg.max_edges_per_node = 20;
        cfg.thread_count = 10;

        std::vector<std::thread> threads;

        // start monitor
        auto start_time = std::chrono::steady_clock::now();
        start_monitor(start_time);

        // launch unweighted stressers
        for (int t = 0; t < cfg.thread_count; ++t) {
            threads.emplace_back(run_unweighted_stress, t, cfg, static_cast<uint32_t>(12345 + t));
        }

        // launch weighted stressers
        for (int t = 0; t < cfg.thread_count; ++t) {
            threads.emplace_back(run_weighted_stress, t, cfg, static_cast<uint32_t>(54321 + t));
        }

        for (auto &th : threads) th.join();

        // stop monitor and print final stats
        g_monitor_running = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        std::cerr << "FINAL: total_checks=" << g_checks_done.load() << " total_ops=" << g_ops_done.load()
                  << " rss_mb=" << (get_rss_bytes() / 1024 / 1024) << std::endl;

        std::cout << "Stress tests completed successfully." << std::endl;

        // run additional tests from tests/test_d_and_g_n.cpp
        int rc = run_d_and_g_n_tests();
        if (rc != 0) return rc;

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }
}