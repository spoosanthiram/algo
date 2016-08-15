/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "KruskalMinSpanningGraph.h"

#include <forward_list>

#ifdef UNIT_TEST
#include <fstream>

#include <fmt/format.h>
#include <gtest/gtest.h>
#endif

namespace psa {

KruskalMinSpanningGraph::~KruskalMinSpanningGraph()
{
    for (auto edge : m_edges)
        delete edge;

    for (auto vertex : m_vertices)
        delete vertex;
}

long KruskalMinSpanningGraph::findMst()
{
    long cost = 0;

    // sort the edges
    auto cmp = [](KruskalMinSpanningGraphEdge* lhs, KruskalMinSpanningGraphEdge* rhs) -> bool
                { return lhs->cost() < rhs->cost(); };
    std::sort(m_edges.begin(), m_edges.end(), cmp);

    for (auto e : m_edges) {
        KruskalMinSpanningGraphVertex* uleader = this->findLeader(e->u());
        KruskalMinSpanningGraphVertex* vleader = this->findLeader(e->v());
        if (uleader != vleader) {
            cost += e->cost();
            this->unionLeaders(uleader, vleader);
        }
    }

    return cost;
}

KruskalMinSpanningGraphVertex* KruskalMinSpanningGraph::findLeader(
        KruskalMinSpanningGraphVertex* v)
{
    if (v->leader() == v || v->leader()->leader() == v->leader())
        return v->leader();

    std::forward_list<KruskalMinSpanningGraphVertex*> vertices;
    while (v->leader() != v) {
        vertices.push_front(v);
        v = v->leader();
    }

    vertices.pop_front();
    for (auto u : vertices)
        u->setLeader(v);

    return v;
}

void KruskalMinSpanningGraph::unionLeaders(KruskalMinSpanningGraphVertex* u,
                                           KruskalMinSpanningGraphVertex* v)
{
    if (u->rank() > v->rank())
        v->setLeader(u);
    else if (u->rank() < v->rank())
        u->setLeader(v);
    else {
        u->setRank(u->rank() + 1);
        v->setLeader(u);
    }
}

#ifdef UNIT_TEST

TEST(KruskalMinSpanningGraphTest, Mst)
{
    const std::string filename{"MinSpanningGraphAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    KruskalMinSpanningGraph graph;
    graph.readAdjList(stream);

    long actual = graph.findMst();
    long expected = 39;
    EXPECT_EQ(expected, actual);
}

TEST(KruskalMinSpanningGraphTest, AlgoClassMst)
{
    const std::string filename{"AlgoClassMinSpanningGraphAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    KruskalMinSpanningGraph graph;
    graph.readAdjList(stream);

    long actual = graph.findMst();
    long expected = -3612829;
    EXPECT_EQ(expected, actual);
}

#endif // UNIT_TEST

} // namespace psa
