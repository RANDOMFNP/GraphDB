# Graphlib 🇬
Graphlib is a C++ library that empowers users to create and edit graphs!

If you want to contribute to GRAPHLIB, check out the CONTRIBUTING.md file!

If this product helped you, please leave a star!
If it doesn't, tell me whats wrong, I respond to issues within 1-3 days!

### READEME.md UPDATES EVERY DAY AFTER A VERSION IS PUBLISHED

## NEWS
Switched license from AGPL v3 to Apache 2.0

## Great first issues
Here I will post some of the best first issues I see and its status
* https://github.com/RANDOMFNP/Graphlib/issues/8 -- Bellman-Ford
* https://github.com/RANDOMFNP/Graphlib/issues/9 -- A*
* https://github.com/RANDOMFNP/Graphlib/issues/10 -- README.md

## Importing Graphlib 📦

For most applications, `#include "graphlib.hpp"` will suffice.

## Linking without CMake 📦

### Using g++

Example usage: `g++ program.cpp -I graphlib/include/ -I graphlib/detail/ -o program`, make sure to include graphlib/include/ and graphlib/detail/
__NOTE:__ This is a C++ 20 (but soon to be 23) library and will not function if you are on too old of a g++ compiler, we recommend g++ 12 as a minimum

### Using clang

Example usage: `clang++ program.cpp -I graphlib/include/ -I ./graphlib/detail/ -o program`, make sure to include graphlib/include/ and graphlib/detail/
__NOTE:__ This is C++ 20 (but soon to be 23) library and will not function if you are on too old of a clang compiler, we recommend clang 14 as a minimum

### Using MSVC

Example usage: `cl.exe /EHsc program.cpp /Fe:program.exe`
__NOTE:__ This is a C++ 20 (but soon to be 23) library and will not function if you are using too old of an MVSC compiler, we recommend MSVC 19.44 as a minimum

## Functions

* node: AnyType
* weight: int/float/numerical


### `void add_nodes(std::unordered_map<node, std::vector<node>> graph, std::string input_file);`
Adds a node to a graph.

### `void delete_instances(std::string node_to_delete, std::string input_file);`
Deletes instances of a node.

### `void print_graph(std::string filename);`
Prints the graph.

### `void graphlib_version();`
Prints the graphlib version.


### `void create_graph(std::unordered_map<node, std::vector<std::pair<node, weights>>> graph, std::string output_file);`
Creates the initial graph for weighted graphs.

### `void create_graph(std::unordered_map<node, std::vector<node>> graph, std::string output_file);`
Creates the initial graph for unweighted graphs.

### `void add_nodes(std::unordered_map<node, std::vector<std::pair<node, weights>>> graph, std::string input_file);`
Adds a set of nodes, weights and connections to the graph to the file.

### `void add_nodes(std::unordered_map<node, std::vector<node>> graph, std::string input_file);`
Adds a set of nodes and their connections to the graph

### `void delete_instances_weighted(std::string node_to_delete, std::string input_file);`
Deletes all instances of a certain node in weighted graphs.

### `std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> add_edge(std::vector<std::pair<node, weights>> new_value, node key, std::string input_file);`
Adds an edge to a weighted graph.

### `std::optional<std::unordered_map<node, std::vector<node>>> add_edge(std::vector<node> new_value, node key, std::string input_file);`
Adds an edge to an unweighted graph

### `std::unordered_map<node, std::vector<node>> parse(std::string input_file);`
Parses a file.

### `std::unordered_map<node, std::vector<std::pair<node, weights>>> parse_weighted(std::string input_file);`
Parses a weighted graph file.


### `std::vector<node> dfs_algorithm(node starting_value, std::string input_file);`
Runs DFS over a certain graph.
### `std::vector<node> bfs_algorithm(node starting_node, std::string input_file);`
Runs BFS over a certain graph.
### `std::vector<node> dijkstras_algorithm(node starting_node, std::string input_file);`
Runs Dijkstra's algorithm over a certain graph.

## In memory

### `std::optional<std::vector<node>> get_neighbors(node key, std::unordered_map<node, std::vector<node>> graph);`
Gets the neighbors of a key IN MEMORY.

### `std::optional<std::vector<node>> get_neighbors(node key, std::unordered_map<node, std::vector<std::pair<node, weights>>> graph);`
Gets the neighbors of a weighted key IN MEMORY.

### `std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> add_edge(std::vector<std::pair<node, weights>> new_value, node key, std::unordered_map<node, std::vector<std::pair<node, weights>>> graph);`
Adds a weighted edge IN MEMORY.

### `std::optional<std::unordered_map<node, std::vector<node>>> add_edge(std::vector<node> new_value, node key, std::unordered_map<node, std::vector<node>> graph);`
Adds an unweighted edge IN MEMORY.

### `std::vector<node> bfs_algorithm(node starting_node, std::unordered_map<node, std::vector<node>> graph);`
Runs BFS IN MEMORY.
### `std::vector<node> dfs_algorithm(node starting_value, std::unordered_map<node, std::vector<node>> graph);`
Runs DFS IN MEMORY.
### std::vector<node> dijkstras_algorithm(node starting_node, std::unordered_map<node, std::vector<std::pair<node, weights>>> graph);`
Runs Dijkstra's algorithm IN MEMORY.

## Undirected

### `std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(std::unordered_map<node, std::vector<std::pair<node, weights>>> graph, node key1, node key2, std::string input_file);`
Connects 2 weighted nodes together IN MEMORY.
### `std::optional<std::unordered_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(node key1, node key2, std::string input_file);`
Connects 2 weighted nodes together.


### `std::optional<std::unordered_map<node, std::vector<node>>> undirected_connect(std::unordered_map<node, std::vector<node>> graph, node key1, node key2, std::string input_file);'
Connects 2 nodes together IN MEMORY.
### `std::optional<std::unordered_map<node, std::vector<node>>> undirected_connect(node key1, node key2, std::string input_file);`'
Connects 2 nodes together.
