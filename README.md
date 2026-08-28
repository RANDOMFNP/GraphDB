# Graphlib, the one stop, C++ 23/20 library for directional (and soon undirectional) graphs!

Graphlib is a C++ library that empowers users to create and edit graphs!

If you want to contribute to GRAPHLIB, check out the CONTRIBUTING.md file!

If this product helped you, please leave a star!
If it doesn't, tell me whats wrong, I respond to issues within 1-3 days!

## NEWS
Switched license from AGPL v3 to Apache 2.0

## Great first issues
Here I will post some of the best first issues I see and its status
* https://github.com/RANDOMFNP/Graphlib/issues/2 -- Inefficiencies

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
__NOTE:__ This is a C++ 20 (but soon to be 23) library and will not function if you are using too old of an MVSC compile, we recommend MSVC 19.44 as a minimum

## Functions

* node: AnyType
* weight: int/float

### `graphlib::create_graph(std::unordered_map<node, std::vector<node>> graph, std::string output_file)`
create_graph() creates the initial graph containing the first node vector node pairs.
### `graphlib::create_graph(std::unordered_map<node, std::vector<std::pair<node, weight>>> graph, std::string output_file)`
The weighted version of create_graph() creates the initial graph containing the first node vector node weight pairs.

### `graphlib::add_nodes(std::unordered_map<node, std::vector<node>> graph, std::string input_file)`
add_nodes() appends `graph` to the end of a file.
### `graphlib::add_nodes(std::unordered_map<node, std::vector<std::pair<node, weight> graph, std::string input_file)`
The weighted version of add_nodes() appends `graph` to the end of a file.

### `graphlib::delete_instances(std::string node_to_delete, std::string input_file)`
delete_instances() removes all instances of `node_to_delete` from the file.
### `graphlib::delete_instances_weighted(std::string node_to_delete, std::string input_file)`
The weighted version of delete_instances() removes all instances of `node_to_delete` and its weight from the file.

### `graphlib::add_edge(std::vector<node>, node key, std::string input_file)`
add_edge() appends the vector of nodes to the vector at location `key` and then writes it to the file.
### `graphlib::add_edge(std::vector<std::pair<node, weight> node key, std::string input_file)`
The weighted version of add_edge() appends the vector of nodes to the vector at location `key` and then writes it to the file.

### `graphlib::parse<type1(node)>(std::string input_file)`
parse<type>() opens a file and returns a std::unordered_map<node, std::vector<node>>.
### `graphlib::parse_weighted<type1(node), type2(weight)>(std::string input_file)`
parse<type, weight>() opens a file with weights and returns a std::unordered_map<node, std::vector<std::pair<node, weight>>>.

### `graphlib::print_graph(std::string input_file)`
print_graph() prints out the contents of a file.

## Algorithms

### `graphlib::bfs_algorithm(node starting_node, std::string input_file)`
bfs_algorithm() runs BFS starting from `starting_node` to the end of the file.
### `graphlib::dfs_algorithm(node starting_node, std::string input_file)`
dfs_algorithm() runs DFS starting from `starting_node` to the end of the file.

## Soon to be added

### `graphlib::dijkstras_algorithm(node starting_node, std::string input_file)`
dijkstras_algorithm() runs Dijkstra's algorithm from `starting_node` to the end of the file.
