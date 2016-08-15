/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_PRIMMINSPANNINGGRAPH_H
#define PSA_PRIMMINSPANNINGGRAPH_H

#include <cassert>
#include <limits>
#include <vector>

#include "Graph.h"

namespace psa {

class PrimMinSpanningGraphEdge;

class PrimMinSpanningGraphVertex : public Vertex
{
public:
    PrimMinSpanningGraphVertex(vertexid_t id) : Vertex{id} {}

    int cost() const { return m_cost; }
    const std::vector<PrimMinSpanningGraphEdge*>& edges() const { return m_edges; }

    void setCost(int cost) { m_cost = cost; }

    void addEdge(PrimMinSpanningGraphEdge* e) { m_edges.push_back(e); }

private:
    int m_cost{std::numeric_limits<int>::max()};
    std::vector<PrimMinSpanningGraphEdge*> m_edges{};
};

class PrimMinSpanningGraphEdge : public Edge<PrimMinSpanningGraphVertex>
{
public:
    PrimMinSpanningGraphEdge(edgeid_t id,
                             PrimMinSpanningGraphVertex* u,
                             PrimMinSpanningGraphVertex* v,
                             int cost)
        : Edge<PrimMinSpanningGraphVertex>{id, u, v}
        , m_cost{cost}
    {}

    int cost() const { return m_cost; }

private:
    int m_cost;
};

class PrimMinSpanningGraph : public Graph<PrimMinSpanningGraphVertex, PrimMinSpanningGraphEdge>
{
public:
    PrimMinSpanningGraph() = default;
    ~PrimMinSpanningGraph();

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }
    PrimMinSpanningGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    PrimMinSpanningGraphEdge* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new PrimMinSpanningGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, PrimMinSpanningGraphVertex* u,
                     PrimMinSpanningGraphVertex* v, int cost) override
    {
        auto e = new PrimMinSpanningGraphEdge{id, u, v, cost};
        u->addEdge(e);
        m_edges.push_back(e);
        ++id;

        if (this->type() == Type::Undirected) {
            e = new PrimMinSpanningGraphEdge{id, v, u, cost};
            v->addEdge(e);
            m_edges.push_back(e);
            ++id;
        }

        return id;
    }

    long findMst();

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override { m_edges.reserve(nedges); }

    std::vector<PrimMinSpanningGraphVertex*> m_vertices{};
    std::vector<PrimMinSpanningGraphEdge*> m_edges{};
};

} // namespace psa

#endif // PSA_PRIMMINSPANNINGGRAPH_H
