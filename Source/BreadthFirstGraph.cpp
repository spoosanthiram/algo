/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "BreadthFirstGraph.h"

#include <queue>
#include <sstream>

#ifdef UNIT_TEST
#include <fstream>

#include <fmt/format.h>
#include <gtest/gtest.h>
#endif

namespace psa {

std::string BreadthFirstGraphVertex::toString() const
{
    std::stringstream stream;

    stream << this->id();

    for (auto edge : m_edges)
        stream << '\t' << edge->v()->id();

    return stream.str();
}

BreadthFirstGraph::~BreadthFirstGraph()
{
    for (auto it = m_edges.begin(); it != m_edges.end(); ++it)
        delete *it;

    for (auto it = m_vertices.begin(); it != m_vertices.end(); ++it)
        delete *it;
}

void BreadthFirstGraph::traverse(vertexid_t startVertexId)
{
    BreadthFirstGraphVertex* u = this->vertex(startVertexId);
    u->setExplored(true);

    std::queue<BreadthFirstGraphVertex*> queue;
    queue.push(u);

    while (!queue.empty()) {
        u = queue.front();
        queue.pop();

        for (auto e : u->edges()) {
            BreadthFirstGraphVertex* v = e->v();
            if (!v->isExplored()) {
                v->setExplored(true);
                queue.push(v);
            }
        }
    }
}

unsigned int BreadthFirstGraph::distance(vertexid_t startVertexId, vertexid_t endVertexId)
{
    BreadthFirstGraphVertex* u = this->vertex(startVertexId);
    u->setExplored(true);
    u->setDistance(0);

    BreadthFirstGraphVertex* end = this->vertex(endVertexId);

    std::queue<BreadthFirstGraphVertex*> queue;
    queue.push(u);

    while (!queue.empty()) {
        u = queue.front();
        queue.pop();

        if (u->id() == end->id())
            break;

        for (auto e : u->edges()) {
            BreadthFirstGraphVertex* v = e->v();
            if (!v->isExplored()) {
                v->setExplored(true);
                v->setDistance(u->distance() + 1);
                queue.push(v);
            }
        }
    }

    return end->distance();
}

#ifdef UNIT_TEST

TEST(BreadthFirstGraphTest, BreadthFirstSearch)
{
    const std::string filename{"BreadthFirstAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};
    BreadthFirstGraph graph;
    graph.readAdjList(stream);
    unsigned int distanceExpected = 2;
    unsigned int distanceActual = graph.distance(0, 2);
    EXPECT_EQ(distanceExpected, distanceActual);
}

#endif
}
