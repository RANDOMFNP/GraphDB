cmake_minimum_required(VERSION 3.10.0)
project(graphlib VERSION 0.1.0 LANGUAGES C CXX)

option(BUILD_TESTS "Build project tests" OFF)

add_library(graphlib INTERFACE)

target_compile_features(graphlib INTERFACE cxx_std_23)

target_include_directories(graphlib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
