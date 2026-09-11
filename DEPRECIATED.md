Heres a list of functions that are depreciated and will be removed within the next 3 bug fix updates

### create_graph + weighted version 
* -Reason: Can be replaced with the new save function
* -Replaced with: save

### delete_nodes_weighted
* -Reason: Can be replaced with the new version of delete_nodes in experimental
* -Replaced with: delete_nodes

### All of the weighted versions of every function
* -Reason: All functions will soon use the Edge<> and Node<> types along with flat_map. GraphDB will also use std::variant so that there are less functions.
* -Replaced with: std::variant versions
