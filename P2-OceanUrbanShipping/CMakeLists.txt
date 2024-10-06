cmake_minimum_required(VERSION 3.23.2)
project(DA_OceanUrbanShipping)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/graph)

add_executable(DA_OceanUrbanShipping
        src/graph/Graph.cpp
        src/graph/Graph.h
        src/graph/VertexEdge.cpp
        src/graph/VertexEdge.h
        src/DatasetLoader.cpp
        src/DatasetLoader.h
        src/main.cpp
        src/Menu.cpp
        src/Menu.h
        src/Utils.cpp
        src/Utils.h)
