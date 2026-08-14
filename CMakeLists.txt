cmake_minimum_required(VERSION 3.10.0)
project(graphlib VERSION 0.1.0 LANGUAGES C CXX)

option(BUILD_TESTS "Build project tests" OFF)

file(GLOB GRAPH_SOURCES "src/*.cpp")

add_library(graphlib ${GRAPH_SOURCES})

target_include_directories(graphlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
