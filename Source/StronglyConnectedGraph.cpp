/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "StronglyConnectedGraph.h"

#include <functional>

#ifdef UNIT_TEST
#include <fstream>

#include <fmt/format.h>
#include <gtest/gtest.h>
#endif

namespace psa {

StronglyConnectedGraph::~StronglyConnectedGraph()
{
    for (auto redge : m_reverseEdges)
        delete redge;

    for (auto edge : m_edges)
        delete edge;

    for (auto vertex : m_vertices)
        delete vertex;
}

edgeid_t StronglyConnectedGraph::addEdge(edgeid_t id, StronglyConnectedGraphVertex* u,
                                         StronglyConnectedGraphVertex* v, int /*value*/)
{
    auto e = new Edge<StronglyConnectedGraphVertex>{id, u, v};
    e->u()->addEdge(e);
    m_edges.push_back(e);

    auto reverseEdge = new Edge<StronglyConnectedGraphVertex>{e->id(), e->v(), e->u()};
    e->v()->addReverseEdge(reverseEdge);
    m_reverseEdges.push_back(reverseEdge);

    ++id;

    return id;
}

namespace {

unsigned int s_finishTime = 0;
unsigned int nextFinishTime() { return s_finishTime++; }

} // anonymous

/**
 * @brief StronglyConnectedGraph::kosaraju computes strongly connected components (SCC) for the
 * graph using Kosaraju's algorithm.
 * @return unordered_map that maps leader vertex id to size of SCC.
 */
std::unordered_map<vertexid_t, unsigned int> StronglyConnectedGraph::kosaraju()
{
    s_finishTime = 0; // make sure finishTime = 0 before starting

    this->dfsLoopReverse();

    m_verticesByFinishTime.resize(m_vertices.size()); // resize, so we can index

    for (auto v : m_vertices) {
        v->setExplored(false);
        m_verticesByFinishTime[v->finishTime()] = v;
    }

    this->dfsLoop();

    std::unordered_map<vertexid_t, unsigned int> leaderMap;
    for (auto v : m_vertices)
        ++leaderMap[v->leader()->id()];

    return leaderMap;
}

void StronglyConnectedGraph::dfsLoopReverse()
{
    for (auto rit = m_vertices.rbegin(); rit != m_vertices.rend(); ++rit) {
        if (!(*rit)->explored())
            this->dfsReverse(*rit);
    }
}

void StronglyConnectedGraph::dfsReverse(StronglyConnectedGraphVertex* v)
{
    v->setExplored(true);
    for (auto e : v->reverseEdges()) {
        if (!e->v()->explored())
            this->dfsReverse(e->v());
    }
    v->setFinishTime(nextFinishTime());
}

void StronglyConnectedGraph::dfsLoop()
{
    StronglyConnectedGraphVertex* v;
    for (auto rit = m_verticesByFinishTime.rbegin(); rit != m_verticesByFinishTime.rend(); ++rit) {
        v = *rit;
        if (!v->explored())
            this->dfs(v, v);
    }
}

void StronglyConnectedGraph::dfs(StronglyConnectedGraphVertex* v,
                                 StronglyConnectedGraphVertex* leader)
{
    v->setExplored(true);
    v->setLeader(leader);
    for (auto e : v->edges()) {
        if (!e->v()->explored())
            this->dfs(e->v(), leader);
    }
}

#ifdef UNIT_TEST

TEST(StronglyConnectedGraphTest, Kosaraju)
{
    const std::string filename{"StronglyConnectedAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    StronglyConnectedGraph graph;
    graph.readAdjList(stream);
    EXPECT_EQ(StronglyConnectedGraph::Type::Directed, graph.type());

    std::vector<unsigned int> expected{4, 3, 3, 1};

    std::unordered_map<vertexid_t, unsigned int> leaderMap = graph.kosaraju();

    std::vector<unsigned int> scc;
    for (auto leader : leaderMap)
        scc.push_back(leader.second);

    std::sort(scc.begin(), scc.end(), std::greater<unsigned int>{});

    EXPECT_EQ(expected, scc);
}

#ifdef NDEBUG // release mode only as it takes long time!
TEST(StronglyConnectedGraphTest, AlgoClassKosaraju)
{
    const std::string filename{"AlgoClassStronglyConnectedAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    StronglyConnectedGraph graph;
    graph.readAdjList(stream);
    EXPECT_EQ(StronglyConnectedGraph::Type::Directed, graph.type());

    std::vector<unsigned int> expected{434821, 968, 459, 313, 211};

    std::unordered_map<vertexid_t, unsigned int> leaderMap = graph.kosaraju();
    EXPECT_GE(leaderMap.size(), expected.size());

    std::vector<unsigned int> scc;
    for (auto leader : leaderMap)
        scc.push_back(leader.second);

    std::sort(scc.begin(), scc.end(), std::greater<unsigned int>{});

    std::vector<unsigned int> scc2{scc.begin(), scc.begin() + expected.size()};

    EXPECT_EQ(expected, scc2);
}
#endif

#endif // UNIT_TEST

} // namespace psa
