/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "PrimMinSpanningGraph.h"

#include <unordered_map>
#include <utility>

#ifdef UNIT_TEST
#include <fstream>

#include "gtest/gtest.h"
#endif

#include "AlgoBase.h"
#include "MinHeap.h"

namespace psa {

// LessThan function type, for less than compare function on PrimMinSpanningGraphVertex
using LessThan = bool(*)(PrimMinSpanningGraphVertex*, PrimMinSpanningGraphVertex*);

PrimMinSpanningGraph::~PrimMinSpanningGraph()
{
    for (auto edge : m_edges)
        delete edge;

    for (auto vertex : m_vertices)
        delete vertex;
}

long PrimMinSpanningGraph::findMst()
{
    PrimMinSpanningGraphVertex* v = this->vertex(0); // some arbitrary vertex
    v->setCost(0);

    long cost = 0;
    std::unordered_map<vertexid_t, std::size_t> heapIndexMap;

    // less than lambda
    auto lessThan = [](PrimMinSpanningGraphVertex* lhs, PrimMinSpanningGraphVertex* rhs) -> bool
        { return lhs->cost() < rhs->cost(); };

    MinHeap<PrimMinSpanningGraphVertex*, LessThan, HeapIndex<PrimMinSpanningGraphVertex>>
        verticesToProcess{m_vertices, lessThan, HeapIndex<PrimMinSpanningGraphVertex>{heapIndexMap}};

    std::vector<PrimMinSpanningGraphVertex*> processedVertices{m_vertices.size()}; // fill with nullptr

    while (!verticesToProcess.isEmpty()) {
        v = verticesToProcess.pop();

        processedVertices[v->id()] = v;
        cost += v->cost();

        for (auto e : v->edges()) {
            PrimMinSpanningGraphVertex* w = e->v();
            if (!processedVertices[w->id()]) {
                if (e->cost() < w->cost()) {
                    w->setCost(e->cost());
                    verticesToProcess.siftup(heapIndexMap[w->id()]);
                }
            }
        }
    }

    return cost;
}

#ifdef UNIT_TEST

TEST(PrimMinSpanningGraphTest, Mst)
{
    const std::string filename{"MinSpanningGraphAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    PrimMinSpanningGraph graph;
    graph.readAdjList(stream);

    long actual = graph.findMst();
    long expected = 39;
    EXPECT_EQ(expected, actual);
}

TEST(PrimMinSpanningGraphTest, AlgoClassMst)
{
    const std::string filename{"AlgoClassMinSpanningGraphAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    PrimMinSpanningGraph graph;
    graph.readAdjList(stream);

    long actual = graph.findMst();
    long expected = -3612829;
    EXPECT_EQ(expected, actual);
}

#endif // UNIT_TEST

} // namespace psa
