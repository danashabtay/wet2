cmake_minimum_required(VERSION 3.20)
project(wet2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wet2
        main23a2.cpp
        wet2util.h
        worldcup23a2.cpp
        worldcup23a2.h team.cpp team.h player.cpp player.h Avltree.h HashTable.h UnionFind.cpp UnionFind.h)
