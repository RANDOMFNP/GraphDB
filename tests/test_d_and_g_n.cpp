// Generated with the assistance of Artificial intelligence
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
#include <atomic>
#include <chrono>

using namespace std;

#include "graphlib.hpp"

static inline string unique_temp_path_local(const string& name) {
    const auto now = chrono::steady_clock::now().time_since_epoch().count();
    static atomic<uint64_t> counter{0};
    const auto id = counter.fetch_add(1);
    return "/tmp/graphlib_" + name + "_" + to_string(now) + "_" + to_string(id) + ".txt";
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

// local Dijkstra implemented the same way as graphlib::dijkstras_algorithm
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
                auto res = graphlib::get_neighbors<int>(key, ug);
                if (!same_vector_contents(res, ug[key])) {
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
                auto got = graphlib::get_neighbors<int,int>(key, wg);
                vector<int> expected;
                for (const auto &p : wg[key]) expected.push_back(p.first);
                if (!same_vector_contents(got, expected)) {
                    cerr << "get_neighbors (weighted) returned unexpected values on iteration " << it << "\n";
                    cerr << "expected first few: ";
                    for (size_t i=0;i<min<size_t>(expected.size(),5);++i) cerr<<expected[i]<<" "; cerr<<"\n";
                    cerr << "got first few: ";
                    for (size_t i=0;i<min<size_t>(got.size(),5);++i) cerr<<got[i]<<" "; cerr<<"\n";
                    return 3;
                }
            }

            // Dijkstra: write to file, call library dijkstra and compare order with local implementation
            const string file = unique_temp_path_local("dijk");
            graphlib::create_graph<int,int>(wg, file);

            if (!wg.empty()) {
                int pick_idx = rng() % wg.size();
                auto itw = wg.begin(); advance(itw, pick_idx);
                int start = itw->first;

                auto lib_order = graphlib::dijkstras_algorithm<int,int>(start, file);
                auto local_order = local_dijkstra(wg, start);

                if (lib_order != local_order) {
                    cerr << "dijkstra ordering mismatch on iteration " << it << "\n";
                    cerr << "start=" << start << " lib_order_size=" << lib_order.size() << " local_order_size=" << local_order.size() << "\n";
                    cerr << "lib first 10: "; for (size_t i=0;i<min<size_t>(lib_order.size(),10);++i) cerr<<lib_order[i]<<" "; cerr<<"\n";
                    cerr << "loc first 10: "; for (size_t i=0;i<min<size_t>(local_order.size(),10);++i) cerr<<local_order[i]<<" "; cerr<<"\n";
                    remove(file.c_str());
                    return 4;
                }
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

