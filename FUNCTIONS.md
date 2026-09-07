## Functions

* node: AnyType
* weight: int/float/numerical

## NOTE THIS USES THE `graphdb::` namespace so prefix everything here with `graphdb::`

## This list includes the return type too.

### `void add_nodes(flat_map<node, std::vector<node>> graph, std::string input_file);`

Adds a node to a graph.

### `void delete_instances(std::string node_to_delete, std::string input_file);`

Deletes instances of a node.

### `void print_graph(std::string filename);`

Prints the graph.

### `void graphdb_version();`

Prints the graphdb version.

### `void create_graph(flat_map<node, std::vector<std::pair<node, weights>>> graph, std::string output_file);`

Creates the initial graph for weighted graphs.

### `void create_graph(flat_map<node, std::vector<node>> graph, std::string output_file);`

Creates the initial graph for unweighted graphs.

### `void add_nodes(flat_map<node, std::vector<std::pair<node, weights>>> graph, std::string input_file);`

Adds a set of nodes, weights and connections to the graph to the file.

### `void add_nodes(flat_map<node, std::vector<node>> graph, std::string input_file);`

Adds a set of nodes and their connections to the graph

### `void delete_instances_weighted(std::string node_to_delete, std::string input_file);`

Deletes all instances of a certain node in weighted graphs.

###

`std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> add_edge(std::vector<std::pair<node, weights>> new_value, node key, std::string input_file);`

Adds an edge to a weighted graph.

###

`std::optional<flat_map<node, std::vector<node>>> add_edge(std::vector<node> new_value, node key, std::string input_file);`

Adds an edge to an unweighted graph

### `flat_map<node, std::vector<node>> parse(std::string input_file);`

Parses a file.

### `flat_map<node, std::vector<std::pair<node, weights>>> parse_weighted(std::string input_file);`

Parses a weighted graph file.

### `std::vector<node> dfs_algorithm(node starting_value, std::string input_file);`

Runs DFS over a certain graph.

### `std::vector<node> bfs_algorithm(node starting_node, std::string input_file);`

Runs BFS over a certain graph.

### `std::vector<node> dijkstras_algorithm(node starting_node, std::string input_file);`

Runs Dijkstra's algorithm over a certain graph.

## In memory

### `std::optional<std::vector<node>> get_neighbors(node key, flat_map<node, std::vector<node>> graph);`

Gets the neighbors of a key IN MEMORY.

###

`std::optional<std::vector<node>> get_neighbors(node key, flat_map<node, std::vector<std::pair<node, weights>>> graph);`

Gets the neighbors of a weighted key IN MEMORY.

###

`std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> add_edge(std::vector<std::pair<node, weights>> new_value, node key, flat_map<node, std::vector<std::pair<node, weights>>> graph);`

Adds a weighted edge IN MEMORY.

###

`std::optional<flat_map<node, std::vector<node>>> add_edge(std::vector<node> new_value, node key, flat_map<node, std::vector<node>> graph);`

Adds an unweighted edge IN MEMORY.

### `std::vector<node> bfs_algorithm(node starting_node, flat_map<node, std::vector<node>> graph);`

Runs BFS IN MEMORY.

### `std::vector<node> dfs_algorithm(node starting_value, flat_map<node, std::vector<node>> graph);`

Runs DFS IN MEMORY.

###
`std::vector<node> dijkstras_algorithm (node starting_node, flat_map<node, std::vector<std::pair<node, weights>>> graph);`

Runs Dijkstra's algorithm IN MEMORY.

## Undirected

###

`std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(flat_map<node, std::vector<std::pair<node, weights>>> graph, node key1, node key2, std::string input_file);`

Connects 2 weighted nodes together IN MEMORY.

###

`std::optional<flat_map<node, std::vector<std::pair<node, weights>>>> undirected_connect(node key1, node key2, std::string input_file);`

Connects 2 weighted nodes together.

###
`std::optional<flat_map<node, std::vector<node>>> undirected_connect (flat_map<node, std::vector<node>> graph, node key1, node key2, std::string input_file);`

Connects 2 nodes together IN MEMORY.

###

`std::optional<flat_map<node, std::vector<node>>> undirected_connect(node key1, node key2, std::string input_file);`

Connects 2 nodes together.

## flat_map

Quick documentation on flat_map.

It is instantiated by using this format:

```c++
graphdb::flat_map<t1, t2> <name>;
```

NOTE: For most applications, you can just use it as a drop in replacement for std::unordered_map.

WARNING: flat_map is defined as graphdb::flat_map

### `void sort();`

Sorts a flat_map from least to greatest.

### `void reserve(std::size_t capacity);`

Reserves space inside the flat_map.

### `auto find(const K &key);`

Finds a value in O(log(N)) time in a flat_map.

### `auto value(const K &key);`

Returns the value of a certain key in a flat_map.

### `auto insert(const K &key, const V &value);`

Inserts a value at a key in a flat_map.

### `auto insert_sorted(const K &key, const V &value);`

Insert but guaranteed to be sorted.

### `auto emplace(K key, V value);`

Not a true emplace interface but needed for functionality.

### `auto &operator[](const K &key);`

Works like a normal unordered_map.

### `const V &at(const K &key);`

Works like a normal unordered_map.

### `bool erase(const K &key);`

Erases at a certain key.

### `size_t size();`

Returns the size of the flat_map.

### `bool empty();`

Checks if the graph is empty.

### `void clear();`

Clears the graph in its entirety.

### `auto begin();`
ITERATORS

### `auto end();`
ITERATORS

## Transaction

Transactions make graphDB safer. They are not required, but they are highly encouraged if your database is prone to data
corruption.

It is instantiated using this format:
```c++
Transaction tx(graph);
```
where graph is a flat_map.

### `Transaction(flat_map<K, V>& existing_graph);`

Starts a transaction.

### `void commit();`

Commits the transaction to the input graph.

### `void rollback();`

Makes the Transaction equal the original graph.

### `auto& return_graph();`

Returns a flat_map object from the Transaction.

## Data Persistence

### `save(flat_map<t1, std::vector<t2>> graph, std::string file)`

Saves and overwrites all contents of a file with the input of a flat_map.