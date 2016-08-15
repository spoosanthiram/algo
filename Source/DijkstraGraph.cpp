/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "DijkstraGraph.h"

#include <unordered_map>

#include "AlgoBase.h"
#include "AlgoException.h"
#include "MinHeap.h"

#ifdef UNIT_TEST
#include <array>
#include <fstream>

#include "gtest/gtest.h"
#endif

namespace psa {

// LessThan function type, used for less than compare function on Dijkstra vertices
using LessThan = bool(*)(DijkstraGraphVertex*, DijkstraGraphVertex*);

DijkstraGraphEdge::DijkstraGraphEdge(edgeid_t id,
                                     DijkstraGraphVertex* u,
                                     DijkstraGraphVertex* v,
                                     unsigned int length)
    : Edge<DijkstraGraphVertex>{id, u, v}
    , m_length{length}
{
}

DijkstraGraph::~DijkstraGraph()
{
    for (auto edge : m_edges)
        delete edge;

    for (auto vertex : m_vertices)
        delete vertex;
}

void DijkstraGraph::findShortestPath(vertexid_t sourceVertexId)
{
    DijkstraGraphVertex* sourceVertex = this->vertex(sourceVertexId);
    if (nullptr == sourceVertex)
        throw AlgoException{fmt::format("The given source vertex id, {} is not in the graph!",
                                        sourceVertexId)};

    sourceVertex->setDistance(0);

    std::unordered_map<vertexid_t, std::size_t> heapIndexMap; // vertices to MinHeap index map

    // compare function of type Compare
    auto lessThan = [](DijkstraGraphVertex* lhs, DijkstraGraphVertex* rhs) -> bool {
        return lhs->distance() < rhs->distance(); };

    // the index done through heap indexing functor
    MinHeap<DijkstraGraphVertex*, LessThan, HeapIndex<DijkstraGraphVertex>>
        verticesToProcess{m_vertices, lessThan, HeapIndex<DijkstraGraphVertex>{heapIndexMap}};

    std::vector<DijkstraGraphVertex*> processedVertices{m_vertices.size()}; // fill it with nullptr

    while (!verticesToProcess.isEmpty()) {
        DijkstraGraphVertex* v = verticesToProcess.pop();

        processedVertices[v->id()] = v;

        for (auto e : v->edges()) {
            DijkstraGraphVertex* w = e->v();
            if (!processedVertices[w->id()]) {
                if (v->distance() + e->length() < w->distance()) {
                    w->setDistance(v->distance() + e->length());
                    w->setParent(v);
                    verticesToProcess.siftup(heapIndexMap[w->id()]);
                }
            }
        }
    }
}

#ifdef UNIT_TEST

TEST(DijkstraGraphTest, ShortestPath)
{
    const std::string filename{"DijkstraAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    DijkstraGraph graph;
    graph.readAdjList(stream);

    graph.findShortestPath(0);

    std::array<int, 10> expected{0, 10, 6, 7, 5, 13, 9, 16, 20, 19};
    std::array<int, 10> actual;
    for (int i = 0; i < 10; ++i) {
        psa::DijkstraGraphVertex* v = graph.vertex(i);
        actual[i] = v->distance();
    }
    EXPECT_EQ(expected, actual);
}

TEST(DijkstraGraphTest, AlgoClassShortestPath)
{
    const std::string filename{"AlgoClassDijkstraAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    DijkstraGraph graph;
    graph.readAdjList(stream);

    graph.findShortestPath(0);

    std::array<int, 10> expected{2599, 2610, 2947, 2052, 2367, 2399, 2029, 2442, 2505, 3068};

    std::array<int, 10> vertices{6, 36, 58, 81, 98, 114, 132, 164, 187, 196};
    std::array<int, 10> actual;
    for (std::size_t i = 0; i < actual.size(); ++i)
        actual[i] = graph.vertex(vertices[i])->distance();

    EXPECT_EQ(expected, actual);
}

#endif // UNIT_TEST

} // namespace psa
