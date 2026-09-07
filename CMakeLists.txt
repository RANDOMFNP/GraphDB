cmake_minimum_required(VERSION 3.28.0)
project(graphlib VERSION 0.5.1 LANGUAGES C CXX)

option(BUILD_TESTS "Build project tests" ON)

add_library(graphlib)

target_sources(graphlib
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphdb.cppm
)

target_sources(graphlib PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        #Include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphdb.hpp
        #Main functions
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/add_edges.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/add_edges_weighted.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/add_nodes.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/add_nodes_weighted.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/create_graph.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/create_graph_weighted.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/delete_nodes.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/delete_nodes_weighted.tpp
        #Algorithms
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/dfs.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/bfs.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/dijkstra.tpp
        #Utilities
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/get_neighbors.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/get_neighbors_weighted.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/print_graph.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/print_version.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/txt_to_un_map.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/txt_to_un_map_weighted.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/undirected_2_edges.tpp
        ${CMAKE_CURRENT_SOURCE_DIR}/detail/undirected_2_edges_weighted.tpp

        ${CMAKE_CURRENT_SOURCE_DIR}/detail/save.tpp
)

target_compile_features(graphlib PUBLIC cxx_std_23)

target_include_directories(graphlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/detail)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
