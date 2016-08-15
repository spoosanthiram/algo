cmake_minimum_required(VERSION 3.0)

project(Algorithms CXX)

add_compile_options(-Wall -std=c++14)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

set(SOURCE_FILES
    ${Algorithms_SOURCE_DIR}/Include/AlgoBase.h
    ${Algorithms_SOURCE_DIR}/Include/AlgoException.h
    ${Algorithms_SOURCE_DIR}/Include/Algo.h
    ${Algorithms_SOURCE_DIR}/Include/BinarySearchTree.h
    ${Algorithms_SOURCE_DIR}/Include/BinaryTree.h
    ${Algorithms_SOURCE_DIR}/Include/BreadthFirstGraph.h
    ${Algorithms_SOURCE_DIR}/Include/DijkstraGraph.h
    ${Algorithms_SOURCE_DIR}/Include/Graph.h
    ${Algorithms_SOURCE_DIR}/Include/HashTable.h
    ${Algorithms_SOURCE_DIR}/Include/HuffmanCode.h
    ${Algorithms_SOURCE_DIR}/Include/KargerMinCutGraph.h
    ${Algorithms_SOURCE_DIR}/Include/KruskalMinSpanningGraph.h
    ${Algorithms_SOURCE_DIR}/Include/Matrix.h
    ${Algorithms_SOURCE_DIR}/Include/MaxTrackingStack.h
    ${Algorithms_SOURCE_DIR}/Include/MinHeap.h
    ${Algorithms_SOURCE_DIR}/Include/PrimMinSpanningGraph.h
    ${Algorithms_SOURCE_DIR}/Include/Queue.h
    ${Algorithms_SOURCE_DIR}/Include/SinglyLinkedList.h
    ${Algorithms_SOURCE_DIR}/Include/Sorting.h
    ${Algorithms_SOURCE_DIR}/Include/StronglyConnectedGraph.h
    ${Algorithms_SOURCE_DIR}/Include/Trie.h

    ${Algorithms_SOURCE_DIR}/Source/AlgoBase.cpp
    ${Algorithms_SOURCE_DIR}/Source/Algo.cpp
    ${Algorithms_SOURCE_DIR}/Source/AlgoException.cpp
    ${Algorithms_SOURCE_DIR}/Source/BinarySearchTree.cpp
    ${Algorithms_SOURCE_DIR}/Source/BreadthFirstGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/DijkstraGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/HashTable.cpp
    ${Algorithms_SOURCE_DIR}/Source/HuffmanCode.cpp
    ${Algorithms_SOURCE_DIR}/Source/KargerMinCutGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/KruskalMinSpanningGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/Matrix.cpp
    ${Algorithms_SOURCE_DIR}/Source/MaxTrackingStack.cpp
    ${Algorithms_SOURCE_DIR}/Source/MinHeap.cpp
    ${Algorithms_SOURCE_DIR}/Source/PrimMinSpanningGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/Queue.cpp
    ${Algorithms_SOURCE_DIR}/Source/SinglyLinkedList.cpp
    ${Algorithms_SOURCE_DIR}/Source/Sorting.cpp
    ${Algorithms_SOURCE_DIR}/Source/StronglyConnectedGraph.cpp
    ${Algorithms_SOURCE_DIR}/Source/Trie.cpp
)

include_directories(
  ${Algorithms_SOURCE_DIR}/Include
  ${Algorithms_SOURCE_DIR}/../3rdParty/fmt
)

# TARGET for Algo library
add_library(Algo ${SOURCE_FILES})
target_link_libraries(Algo stdc++)

# TARGET for AlgoTest executable
add_executable(AlgoTest
    ${SOURCE_FILES}
    ${Algorithms_SOURCE_DIR}/Source/main.cpp
)

target_compile_definitions(AlgoTest PUBLIC UNIT_TEST)
target_include_directories(AlgoTest PUBLIC
    ${Algorithms_SOURCE_DIR}/../3rdParty/googletest/googletest/include
)

target_link_libraries(AlgoTest
    ${Algorithms_SOURCE_DIR}/../3rdParty/fmt/fmt/libfmt.a
    ${Algorithms_SOURCE_DIR}/../3rdParty/googletest/googletest/libgtest.a
    pthread
    stdc++
)

# install into original source root
install(TARGETS Algo AlgoTest DESTINATION ${Algorithms_SOURCE_DIR})
