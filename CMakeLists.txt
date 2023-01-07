cmake_minimum_required(VERSION 3.20)
project(wet2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(wet2
        main23a2.cpp
        wet2util.h
        worldcup23a2.h
        worldcup23a2.cpp
        team.h
        team.cpp
        player.h
        player.cpp
        WorldCupManager.h
        WorldCupManager.cpp
        AvlRankTree.h
        Avltree.h
        AVLTree_new.h
        HashTable.h)