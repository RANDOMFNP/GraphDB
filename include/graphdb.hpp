#pragma once

// GRAPHLIB.HPP

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
#include <optional>
#include <stdexcept>
#include <print>
#include <cstdio>
#include <ranges>
#include <format>
#include <mutex>
#include <shared_mutex>

namespace graphdb {
    template<typename K, typename V>
    class flat_map {
    private:
        std::vector<std::pair<K, V> > KVPairs;
        std::vector<K> QuickLookup;

    public:
        void sort() {
            std::ranges::sort(KVPairs, {}, [](const auto &p) {
                return p.first;
            });

            std::ranges::sort(QuickLookup);
        }

        flat_map() = default;

        flat_map(const K& key, const V& value) {
            insert(key, value);
        }

        flat_map(std::initializer_list<std::pair<K, V> > init) {
            reserve(init.size());
            for (const auto &pair: init) {
                insert(pair.first, pair.second);
            }
        }

        void reserve(std::size_t capacity) {
            QuickLookup.reserve(capacity);
            KVPairs.reserve(capacity);
        }


        auto find(const K &key) {
            auto it = std::ranges::lower_bound(QuickLookup, key);

            if (it == QuickLookup.end() || *it != key) {
                return KVPairs.end();
            }

            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs.begin() + idx;
        }

        const auto find(const K &key) const {
            auto it = std::ranges::lower_bound(QuickLookup, key);

            if (it == QuickLookup.end() || *it != key) {
                return KVPairs.end();
            }

            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs.begin() + idx;
        }

        auto value(const K &key) {
            if (const auto it = std::ranges::find(QuickLookup, key); it == QuickLookup.end()) {
                return static_cast<V>(-1);
            } else {
                auto idx = std::distance(QuickLookup.begin(), it);
                return KVPairs[idx].second;
            }
        }

        auto insert(const K& key, const V& value) {
            auto it = std::ranges::lower_bound(
                KVPairs,
                key,
                {},
                &std::pair<K, V>::first
            );

            if (it != KVPairs.end() && it->first == key)
                return std::make_pair(it, false);

            const auto idx = std::distance(KVPairs.begin(), it);

            it = KVPairs.insert(it, {key, value});
            QuickLookup.insert(QuickLookup.begin() + idx, key);

            return std::make_pair(it, true);
        }

        auto insert_sorted(const K &key, const V &value) {
            auto it = std::ranges::lower_bound(QuickLookup, key);
            auto idx = std::distance(QuickLookup.begin(), it);

            // Key already exists
            if (it != QuickLookup.end() && *it == key) {
                return std::make_pair(idx, false);
            }

            // Insert at the correct sorted position
            QuickLookup.insert(it, key);
            KVPairs.insert(KVPairs.begin() + idx, {key, value});

            return std::make_pair(idx, true);
        }


        auto emplace(K key, V value) {
            return insert(key, value);
        }

        auto &operator[](const K &key) {
            auto it = std::ranges::lower_bound(QuickLookup, key);
            auto idx = std::distance(QuickLookup.begin(), it);

            if (it == QuickLookup.end() || *it != key) {
                QuickLookup.insert(it, key);
                KVPairs.insert(KVPairs.begin() + idx, std::make_pair(key, V{}));
            }

            return KVPairs[idx].second;
        }

        const V &at(const K &key) const {
            auto it = std::ranges::find(QuickLookup, key);
            if (it == QuickLookup.end()) {
                throw std::out_of_range("Key not found");
            }
            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs[idx].second;
        }

        bool erase(const K &key) {
            auto it = std::ranges::find(QuickLookup, key);
            if (it == QuickLookup.end()) return false;

            auto idx = std::distance(QuickLookup.begin(), it);
            QuickLookup.erase(it);
            KVPairs.erase(KVPairs.begin() + idx);
            return true;
        }

        size_t size() const { return QuickLookup.size(); }
        bool empty() const { return QuickLookup.empty(); }

        void clear() {
            QuickLookup.clear();
            KVPairs.clear();
        }

        auto begin() { return std::begin(KVPairs); }
        auto end() { return std::end(KVPairs); }
        auto begin() const { return std::begin(KVPairs); }
        auto end() const { return std::end(KVPairs); }
    };

    // Transaction
    template<typename K, typename V>
    class Transaction {
        flat_map<K, V>& graph;
        flat_map<K, V> graph2;

    public:
        Transaction(flat_map<K, V>& existing_graph)
            : graph(existing_graph),
              graph2(existing_graph) {}

        void commit() {
            graph = graph2;
        }

        void rollback() {
            graph2 = graph;
        }

        auto& return_graph() {
            return graph2;
        }

        const auto& return_graph() const {
            return graph2;
        }
    };

    // PROPERTIES

    using Property = std::variant<
        std::string,
        std::int64_t,
        double,
        bool
    >;

    using Properties = flat_map<std::string, Property>;

    template <typename Nodename, typename Weight = int>
    struct Edge {
        Nodename target;
        std::optional<Weight> weight = std::nullopt;
        uint32_t properties_id = UINT32_MAX;
    };

    template <typename Nodename>
    struct Node {
        Properties properties;
        std::vector<Edge<Nodename>> edges;
    };
}

// EXPERIMENTAL

#include "../Experimental/txt_to_un_map.tpp"
#include "../Experimental/txt_to_un_map_weighted.tpp"

#include "../Experimental/get_neighbors.tpp"
#include "../Experimental/add_nodes.tpp"
#include "../Experimental/delete_nodes.tpp"
#include "../Experimental/add_edges.tpp"

#include "../Experimental/dfs.tpp"
#include "../Experimental/bfs.tpp"
#include "../Experimental/dijkstra.tpp"

#include "../Experimental/undirected_2_edges.tpp"

#include "../Experimental/print_graph.tpp"
#include "../Experimental/print_version.tpp"
#include "../Experimental/save.tpp"
