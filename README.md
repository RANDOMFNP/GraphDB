# Graphlib
Graphlib is a C++ library that empowers users to create and edit graphs!

If you want to contribute to GRAPHLIB, check out the CONTRIBUTING.md file!

If this product helped you, please leave a star!
If it doesn't, tell me whats wrong, I respond to issues within 1-3 days!


# USAGE:
## Importing Graphlib
`#include "graphlib.hpp"`

## Creating an initial Graph
```
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
    return 0;
}
```

## Deleting a node from the Graph
```
#include "graphlib.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {

    delete_instances("16", "graph.txt");

    return 0;
}
```

## Adding a node to the Graph
```
#include "graphlib.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {

    unordered_map<int, vector<int>> graph = {

        {21,  {2, 4}},
        {22,  {3, 5}},
        {23,  {6}},
        {24,  {5, 7}},
    };

    add_nodes(graph, "graph.txt");
    return 0;
}
```

## Printing the Graph
```
#include "graphlib.hpp"
#include <bits/stdc++.h>

using namespace std;

int main() {
    print_graph("graph.txt");
    return 0;
}
```
