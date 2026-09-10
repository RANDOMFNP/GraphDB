cmake_minimum_required(VERSION 3.28)

project(graphlib VERSION 0.5.1 LANGUAGES CXX)

option(BUILD_TESTS "Build project tests" ON)

add_library(graphlib)

target_sources(graphlib
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/graphdb.cppm
)

target_compile_features(graphlib PUBLIC cxx_std_23)

target_include_directories(graphlib
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/Experimental
)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
