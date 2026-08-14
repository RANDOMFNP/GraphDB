#include "graphlib.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {

    unordered_map<int, vector<int>> graph = {

        {1,  {2, 4}},
        {2,  {3, 5}},
        {3,  {6}},
        {4,  {5, 7}},
        {5,  {3, 8}},
        {6,  {9}},
        {7,  {8, 10}},
        {8,  {6, 11}},
        {9,  {12}},
        {10, {11}},
        {11, {12, 13}},
        {12, {14}},
        {13, {8, 14}},
        {14, {15}},
        {15, {5, 16}},
        {16, {}}
    };

    create_graph(graph, "graph.txt");
    delete_instances("16", "graph.txt");

    return 0;
}