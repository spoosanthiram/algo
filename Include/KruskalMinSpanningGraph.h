/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_KRUSKALMINSPANNINGGRAPH_H
#define PSA_KRUSKALMINSPANNINGGRAPH_H

#include "Graph.h"

namespace psa {

class KruskalMinSpanningGraphVertex : public Vertex
{
public:
    KruskalMinSpanningGraphVertex(vertexid_t id)
        : Vertex{id}
        , m_leader{this}
    {}

    KruskalMinSpanningGraphVertex* leader() const { return m_leader; }
    int rank() const { return m_rank; }

    void setLeader(KruskalMinSpanningGraphVertex* leader) { m_leader = leader; }
    void setRank(int rank) { m_rank = rank; }

private:
    KruskalMinSpanningGraphVertex* m_leader;
    int m_rank{0};
};

class KruskalMinSpanningGraphEdge : public Edge<KruskalMinSpanningGraphVertex>
{
public:
    KruskalMinSpanningGraphEdge(edgeid_t id, KruskalMinSpanningGraphVertex* u,
                                KruskalMinSpanningGraphVertex* v, int cost)
        : Edge<KruskalMinSpanningGraphVertex>{id, u, v}
        , m_cost{cost}
    {}

    int cost() const { return m_cost; }

private:
    int m_cost;
};

class KruskalMinSpanningGraph :
    public Graph<KruskalMinSpanningGraphVertex, KruskalMinSpanningGraphEdge>
{
public:
    KruskalMinSpanningGraph() = default;
    ~KruskalMinSpanningGraph();

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }

    KruskalMinSpanningGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    KruskalMinSpanningGraphEdge* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new KruskalMinSpanningGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, KruskalMinSpanningGraphVertex* u,
                     KruskalMinSpanningGraphVertex* v, int cost) override
    {
        auto e = new KruskalMinSpanningGraphEdge{id, u, v, cost};
        m_edges.push_back(e);
        ++id;

        return id;
    }

    long findMst();

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override { m_edges.reserve(nedges); }

    KruskalMinSpanningGraphVertex* findLeader(KruskalMinSpanningGraphVertex* vertex);
    void unionLeaders(KruskalMinSpanningGraphVertex* u, KruskalMinSpanningGraphVertex* v);

    std::vector<KruskalMinSpanningGraphVertex*> m_vertices{};
    std::vector<KruskalMinSpanningGraphEdge*> m_edges{};
};

} // namespace psa

#endif // PSA_KRUSKALMINSPANNINGGRAPH_H
