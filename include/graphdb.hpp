#pragma once

#ifndef GRAPHDB_HPP
#define GRAPHDB_HPP

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

namespace graphdb {

    template <typename K, typename V>
    class flat_map {
    private:
        std::vector<std::pair<K, V>> KVPairs;
        std::vector<K> QuickLookup;
    public:
        void sort() {
            std::ranges::sort(KVPairs, {}, [](const auto& p) {
                return p.first;
            });

            std::ranges::sort(QuickLookup);
        }

        flat_map() = default;

        flat_map(std::initializer_list<std::pair<K, V>> init) {
            reserve(init.size());
            for (const auto& pair : init) {
                insert(pair.first, pair.second);
            }
        }

        void reserve(std::size_t capacity) {
            QuickLookup.reserve(capacity);
            KVPairs.reserve(capacity);
        }


        auto find(const K& key) {
            auto it = std::ranges::lower_bound(QuickLookup, key);

            if (it == QuickLookup.end() || *it != key) {
                return KVPairs.end();
            }

            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs.begin() + idx;
        }

        const auto find(const K& key) const {
            auto it = std::ranges::lower_bound(QuickLookup, key);

            if (it == QuickLookup.end() || *it != key) {
                return KVPairs.end();
            }

            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs.begin() + idx;
        }

        auto value(const K& key) {
            if (const auto it = std::ranges::find(QuickLookup, key); it == QuickLookup.end()) {
                return static_cast<V>(-1);
            } else {
                auto idx = std::distance(QuickLookup.begin(), it);
                return KVPairs[idx].second;
            }
        }
        auto insert(const K& key, const V& value) {
            if (const auto it = std::ranges::find(QuickLookup, key);
                it != QuickLookup.end()) {

                auto idx = std::distance(QuickLookup.begin(), it);
                return std::make_pair(idx, false);

                } else {
                    QuickLookup.emplace_back(key);
                    KVPairs.emplace_back(key, value);

                    auto idx = static_cast<std::ptrdiff_t>(QuickLookup.size() - 1);
                    return std::make_pair(idx, true);
                }
        }

        auto insert_sorted(const K& key, const V& value) {
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
            if (const auto it = std::ranges::find(QuickLookup, key); it != QuickLookup.end()) {
                auto idx = std::distance(QuickLookup.begin(), it);
                return std::make_pair(idx, false);
            }
            QuickLookup.emplace_back(key);
            KVPairs.push_back(std::make_pair(key, value));
            return std::make_pair(QuickLookup.size() - 1, true);
        }

        auto& operator[](const K& key) {
            auto it = std::ranges::find(QuickLookup, key);

            if (it == QuickLookup.end()) {
                QuickLookup.push_back(key);
                KVPairs.push_back(std::make_pair(key, V{}));
                return KVPairs.back().second;
            } else {
                auto idx = std::distance(QuickLookup.begin(), it);
                return KVPairs[idx].second;
            }
        }

        const V& at(const K& key) const {
            auto it = std::ranges::find(QuickLookup, key);
            if (it == QuickLookup.end()) {
                throw std::out_of_range("Key not found");
            }
            auto idx = std::distance(QuickLookup.begin(), it);
            return KVPairs[idx].second;
        }

        bool erase(const K& key) {
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

    template<typename T>

    concept Number = std::integral<T> || std::floating_point<T>;

    template<typename node>
        requires std::formattable<node, char>
    void create_graph(const flat_map<node, std::vector<node>> &graph, const std::string &output_file, std::optional<int> buffer_size_in_bytes = std::nullopt);

    template<typename node>
        requires std::formattable<node, char>
    void add_nodes(const flat_map<node, std::vector<node>> &graph, const std::string &input_file, std::optional<int> buffer_size_in_bytes = std::nullopt);

    inline void delete_instances(const std::string &node_to_delete, const std::string &input_file);

    void print_graph(const std::string &filename);

    void graphlib_version();

    template<typename node, typename weights>
        requires Number<weights> && std::formattable<node, char> && std::formattable<weights, char>
    void create_graph(const flat_map<node, std::vector<std::pair<node, weights>>> &graph,
                      const std::string &output_file, std::optional<int> buffer_size_in_bytes = std::nullopt);

    template<typename node, typename weights>
        requires Number<weights> && std::formattable<node, char> && std::formattable<weights, char>
    void add_nodes(const flat_map<node, std::vector<std::pair<node, weights>>> &graph,
                   const std::string &input_file, std::optional<int> buffer_size_in_bytes = std::nullopt);

    inline void delete_instances_weighted(const std::string &node_to_delete, const std::string &input_file);

    template<typename node, typename weights>
        requires Number<weights>
    std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> add_edge(
        const std::vector<std::pair<node, weights>> &new_value, const node &key, const std::string &input_file);

    template<typename node>
    std::optional<flat_map<node, std::vector<node>> > add_edge(
        const std::vector<node> &new_value, const node &key, const std::string &input_file);

    template<typename node>
    flat_map<node, std::vector<node>> parse(const std::string &input_file);

    template<typename node, typename weights>
        requires Number<weights>
    flat_map<node, std::vector<std::pair<node, weights>>> parse_weighted(const std::string &input_file);

    template<typename node>
    std::optional<std::vector<node> > get_neighbors(const node &key,
                                                    flat_map<node, std::vector<node>> &graph);

    template<typename node, typename weights>
    std::optional<std::vector<node> > get_neighbors(const node &key,
                                                    flat_map<node, std::vector<std::pair<node, weights>>> &
                                                    graph);

    template<typename node>
    std::vector<node> dfs_algorithm(const node &starting_value, const std::string &input_file);

    template<typename node>
    std::vector<node> bfs_algorithm(const node &starting_node, const std::string &input_file);

    template<typename node, typename weights>

        requires Number<weights>
    std::vector<node> dijkstras_algorithm(const node starting_node, const std::string &input_file);

    // In memory
    template<typename node, typename weights>
        requires Number<weights>
    std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> add_edge(
        const std::vector<std::pair<node, weights>> &new_value, const node &key,
        flat_map<node, std::vector<std::pair<node, weights>>> &graph);

    template<typename node>
    std::optional<flat_map<node, std::vector<node>> > add_edge(
        const std::vector<node> &new_value, const node &key, flat_map<node, std::vector<node>> &graph);

    template<typename node>
    std::vector<node> bfs_algorithm(const node &starting_node, flat_map<node, std::vector<node>> &graph);

    template<typename node>
    std::vector<node> dfs_algorithm(const node &starting_value,
                                    const flat_map<node, std::vector<node>> &graph);

    template<typename node, typename weights>
        requires Number<weights>
    std::vector<node> dijkstras_algorithm(const node starting_node,
                                          flat_map<node, std::vector<std::pair<node, weights>>> &graph);

    template<typename node, typename weights>
        requires Number<weights>
    std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(
        flat_map<node, std::vector<std::pair<node, weights>>> &graph, node key1, node key2,
        std::string input_file);

    template<typename node, typename weights>
        requires Number<weights>
    std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(
        node key1, node key2, std::string input_file);

    template<typename node>
    std::optional<flat_map<node, std::vector<node>>> undirected_connect(
        flat_map<node, std::vector<node>> &graph, node key1, node key2, std::string input_file);

    template<typename node>
    std::optional<flat_map<node, std::vector<node>>> undirected_connect(
        node key1, node key2, std::string input_file);
}

#include "../detail/txt_to_un_map.tpp"
#include "../detail/txt_to_un_map_weighted.tpp"

#include "../detail/get_neighbors.tpp"
#include "../detail/get_neighbors_weighted.tpp"
#include "../detail/add_nodes.tpp"
#include "../detail/add_nodes_weighted.tpp"
#include "../detail/create_graph.tpp"
#include "../detail/create_graph_weighted.tpp"
#include "../detail/delete_nodes.tpp"
#include "../detail/delete_nodes_weighted.tpp"
#include "../detail/add_edges.tpp"
#include "../detail/add_edges_weighted.tpp"

#include "../detail/dfs.tpp"
#include "../detail/bfs.tpp"
#include "../detail/dijkstra.tpp"

#include "../detail/undirected_2_edges.tpp"
#include "../detail/undirected_2_edges_weighted.tpp"

#include "../detail/print_graph.tpp"
#include "../detail/print_version.tpp"


#endif
