// AI was used in the making of this test

// CONTRIBUTORS: @TrueFurina

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
#include <random>
#include <stdexcept>
#include <chrono>
#include <filesystem>

using namespace std;

#include "graphdb.hpp"

static inline string unique_temp_path_local(const string& name) {
    const auto now = chrono::steady_clock::now().time_since_epoch().count();
    static uint64_t counter = 0;
    const auto id = counter++;
    const auto dir = std::filesystem::temp_directory_path();
    return (dir / ("graphdb_" + name + "_" + to_string(now) + "_" + to_string(id) + ".txt")).string();
}

void check_true(bool condition, const string& message) {
    if (!condition) {
        throw runtime_error("ASSERTION FAILED: " + message);
    }
}

template<typename V>
bool same_vector_contents(const vector<V>& actual, const vector<V>& expected) {
    if (actual.size() != expected.size()) return false;
    vector<V> a = actual;
    vector<V> e = expected;
    sort(a.begin(), a.end());
    sort(e.begin(), e.end());
    return a == e;
}

template<typename K, typename V>
bool same_graph(const unordered_map<K, vector<pair<K, V>>>& actual,
                const unordered_map<K, vector<pair<K, V>>>& expected) {
    if (actual.size() != expected.size()) return false;

    for (const auto& [key, value] : expected) {
        const auto it = actual.find(key);
        if (it == actual.end()) return false;

        if (it->second.size() != value.size()) return false;
        vector<pair<K, V>> a = it->second;
        vector<pair<K, V>> e = value;
        sort(a.begin(), a.end());
        sort(e.begin(), e.end());
        if (a != e) return false;
    }
    return true;
}

// local Dijkstra implemented the same way as graphdb::dijkstras_algorithm
vector<int> local_dijkstra(const unordered_map<int, vector<pair<int,int>>>& graph, int start) {
    const long long INF = (1LL<<60);
    unordered_map<int,long long> dist;
    for (const auto &kv : graph) dist[kv.first] = INF;
    if (dist.find(start) == dist.end()) dist[start] = INF;

    using Pair = pair<long long,int>;
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    dist[start] = 0;
    q.push({0, start});

    vector<int> order;
    while (!q.empty()) {
        auto [d, u] = q.top(); q.pop();
        auto it = dist.find(u);
        if (it != dist.end() && d > it->second) continue;
        order.push_back(u);
        auto git = graph.find(u);
        if (git == graph.end()) continue;
        for (const auto &p : git->second) {
            int v = p.first; int w = p.second;
            long long nd = d + w;
            auto vt = dist.find(v);
            if (vt == dist.end() || nd < vt->second) {
                dist[v] = nd;
                q.push({nd, v});
            }
        }
    }
    return order;
}

int run_d_and_g_n_tests() {
    try {
        std::mt19937 rng(123456);
        std::uniform_int_distribution<int> nodes_dist(0, 300);
        std::uniform_int_distribution<int> deg_dist(0, 12);
        std::uniform_int_distribution<int> w_dist(1, 200);

        const int ITER = 200;

        for (int it = 0; it < ITER; ++it) {
            int N = max(0, nodes_dist(rng));

            // unweighted graph
            unordered_map<int, vector<int>> ug;
            ug.reserve(N);
            for (int i = 0; i < N; ++i) {
                int deg = deg_dist(rng);
                vector<int> adj;
                adj.reserve(deg);
                for (int e = 0; e < deg; ++e) adj.push_back(rng() % (N + 1));
                ug[i] = adj;
            }

            if (!ug.empty()) {
                // pick random existing key and check get_neighbors
                int pick_idx = rng() % ug.size();
                auto itg = ug.begin(); advance(itg, pick_idx);
                int key = itg->first;
                auto res = graphdb::get_neighbors<int>(key, ug);
                if (!res.has_value() || !same_vector_contents(res.value(), ug[key])) {
                    cerr << "get_neighbors (unweighted) mismatch on iteration " << it << "\n";
                    return 2;
                }
            }

            // weighted graph for dijkstra and weighted get_neighbors
            unordered_map<int, vector<pair<int,int>>> wg;
            wg.reserve(N);
            for (int i = 0; i < N; ++i) {
                int deg = deg_dist(rng);
                vector<pair<int,int>> adj;
                adj.reserve(deg);
                for (int e = 0; e < deg; ++e) adj.emplace_back(rng() % (N + 1), w_dist(rng));
                wg[i] = adj;
            }

            // Test weighted get_neighbors: should return neighbor node ids (first)
            if (!wg.empty()) {
                int pick_idx = rng() % wg.size();
                auto itw = wg.begin(); advance(itw, pick_idx);
                int key = itw->first;
                auto got = graphdb::get_neighbors<int,int>(key, wg);
                vector<int> expected;
                for (const auto &p : wg[key]) expected.push_back(p.first);
                if (!got.has_value() || !same_vector_contents(got.value(), expected)) {
                    cerr << "get_neighbors (weighted) returned unexpected values on iteration " << it << "\n";
                    cerr << "expected first few: ";
                    for (size_t i=0;i<min<size_t>(expected.size(),5);++i) cerr<<expected[i]<<" "; cerr<<"\n";
                    cerr << "got first few: ";
                    if (got.has_value()) {
                        for (size_t i=0;i<min<size_t>(got.value().size(),5);++i) cerr<<got.value()[i]<<" ";
                    }
                    cerr<<"\n";
                    return 3;
                }
            }

            // Read the file once, do the real work in memory, and write the final state back at the end.
            const string file = unique_temp_path_local("dijk");
            graphdb::create_graph<int,int>(wg, file);
            auto working = graphdb::parse_weighted<int,int>(file);
            check_true(same_graph(working, wg), "dijkstra startup parse mismatch");

            if (!working.empty()) {
                int pick_idx = rng() % working.size();
                auto itw = working.begin(); advance(itw, pick_idx);
                int start = itw->first;

                auto lib_order = graphdb::dijkstras_algorithm<int,int>(start, working);
                auto local_order = local_dijkstra(working, start);

                if (lib_order != local_order) {
                    cerr << "dijkstra ordering mismatch on iteration " << it << "\n";
                    cerr << "start=" << start << " lib_order_size=" << lib_order.size() << " local_order_size=" << local_order.size() << "\n";
                    cerr << "lib first 10: "; for (size_t i=0;i<min<size_t>(lib_order.size(),10);++i) cerr<<lib_order[i]<<" "; cerr<<"\n";
                    cerr << "loc first 10: "; for (size_t i=0;i<min<size_t>(local_order.size(),10);++i) cerr<<local_order[i]<<" "; cerr<<"\n";
                    remove(file.c_str());
                    return 4;
                }

                graphdb::create_graph<int,int>(working, file);
            }

            remove(file.c_str());
        }

        cout << "get_neighbors + dijkstra stress test passed (" << ITER << " iterations)" << endl;
        return 0;
    } catch (const exception &ex) {
        cerr << "Exception: " << ex.what() << endl;
        return 1;
    }
}

