/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#include "KargerMinCutGraph.h"

#include <string>

#ifdef UNIT_TEST
#include <fstream>

#include <fmt/format.h>
#include <gtest/gtest.h>
#endif

namespace psa {

KargerMinCutGraph::KargerMinCutGraph(const KargerMinCutGraph& rhs)
    : Graph<psa::KargerMinCutGraphVertex, Edge<psa::KargerMinCutGraphVertex>>{}
{
    m_vertices.reserve(rhs.m_vertices.size());
    for (auto vertex : rhs.m_vertices)
        m_vertices.push_back(new KargerMinCutGraphVertex{vertex->id()});

    m_edges.reserve(rhs.m_edges.size());
    for (auto edge : rhs.m_edges) {
        auto u = this->vertex(edge->u()->id());
        auto v = this->vertex(edge->v()->id());

        m_edges.push_back(new Edge<KargerMinCutGraphVertex>{edge->id(), u, v});
    }

    for (auto rhsVertex : rhs.m_vertices) {
        auto vertex = this->vertex(rhsVertex->id());

        for (auto rhsEdge : rhsVertex->edges())
            vertex->addEdge(this->edge(rhsEdge->id()));
    }
}

KargerMinCutGraph::~KargerMinCutGraph()
{
    for (auto edge : m_edges)
        delete edge;

    for (auto vertex : m_vertices)
        delete vertex;
}

std::size_t KargerMinCutGraph::minCut()
{
    KargerMinCutGraph g = *this;
    std::size_t mincut = m_vertices.size();

    std::size_t niter = m_vertices.size() * m_vertices.size();
    while (niter-- > 0) {
        std::size_t ncut = this->contract();
        if (ncut < mincut)
            mincut = ncut;

        this->resetWith(g);
    }

    return mincut;
}

std::size_t KargerMinCutGraph::contract()
{
    m_nedges = m_edges.size();

    std::size_t nvertices = m_vertices.size();
    while (nvertices > 2) {
        auto e = this->randomEdge();

        auto u = this->vertex(e->u()->id());
        auto v = this->vertex(e->v()->id());

        auto newVertex = new KargerMinCutGraphVertex{static_cast<vertexid_t>(m_vertices.size())};

        this->adjustEdges(u, v, newVertex);
        this->adjustEdges(v, u, newVertex);

        for (std::size_t i = 0; i < m_nedges; ++i) {
            auto edge = this->edge(i);

            if (edge->u() == u || edge->u() == v)
                edge->setU(newVertex);
            else if (edge->v() == u || edge->v() == v)
                edge->setV(newVertex);
        }

        m_vertices.push_back(newVertex);

        --nvertices;
    }

    auto edge = this->randomEdge();
    return edge->u()->edges().size();
}

Edge<KargerMinCutGraphVertex>* KargerMinCutGraph::randomEdge()
{
    std::uniform_int_distribution<std::size_t> ud{0, m_nedges-1};
    std::size_t i = ud(m_randomEngine);
    return m_edges[i];
}

void KargerMinCutGraph::removeEdgeAt(std::size_t i)
{
    std::swap(m_edges[i], m_edges[m_nedges - 1]);
    m_edges[i]->setId(i);
    m_edges[m_nedges - 1]->setId(m_nedges - 1);
    --m_nedges;
}

void KargerMinCutGraph::adjustEdges(const KargerMinCutGraphVertex* u,
                                    const KargerMinCutGraphVertex* v,
                                    KargerMinCutGraphVertex* newVertex)
{
    for (auto edge : u->edges()) {
        if (edge->v() == v)
            this->removeEdgeAt(edge->id());
        else {
            edge->setU(newVertex);
            newVertex->addEdge(edge);
        }
    }
}

void KargerMinCutGraph::resetWith(const KargerMinCutGraph& rhs)
{
    for (auto rhsEdge : rhs.m_edges) {
        auto edge = this->edge(rhsEdge->id());
        edge->setId(rhsEdge->id());

        auto u = this->vertex(rhsEdge->u()->id());
        auto v = this->vertex(rhsEdge->v()->id());
        edge->setU(u);
        edge->setV(v);
    }

    std::size_t nInterVertices = m_vertices.size() - rhs.m_vertices.size();
    while (nInterVertices > 0) {
        delete m_vertices.back();
        m_vertices.pop_back();
        --nInterVertices;
    }

    for (auto rhsVertex : rhs.m_vertices) {
        auto vertex = this->vertex(rhsVertex->id());

        vertex->clearEdges();
        for (auto rhsVertexEdge : rhsVertex->edges())
            vertex->addEdge(this->edge(rhsVertexEdge->id()));
    }
}

#ifdef UNIT_TEST

TEST(KargerMinCutGraphTest, MinCut)
{
    const std::string filename{"KargerMinCutAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    KargerMinCutGraph graph;
    graph.readAdjList(stream);

    std::size_t expected = 2;
    std::size_t actual = graph.minCut();
    EXPECT_EQ(expected, actual);
}

#ifdef NDEBUG // release mode only as it takes long time!
TEST(KargerMinCutGraphTest, AlgoClassMinCut)
{
    const std::string filename{"AlgoClassKargerMinCutAdjList.txt"};
    std::ifstream stream{filename};
    if (!stream)
        throw AlgoException{fmt::format(AlgoException::FileOpenRead, filename)};

    KargerMinCutGraph graph;
    graph.readAdjList(stream);

    std::size_t expected = 17;
    std::size_t actual = graph.minCut();
    EXPECT_EQ(expected, actual);
}
#endif

#endif // UNIT_TEST

} // namespace psa
