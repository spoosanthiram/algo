/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_MINCUTGRAPH_H
#define PSA_MINCUTGRAPH_H

#include <random>
#include <vector>

#include "Graph.h"

namespace psa {

class KargerMinCutGraphVertex : public Vertex
{
public:
    KargerMinCutGraphVertex(vertexid_t id) : Vertex{id} {}

    const std::vector<Edge<KargerMinCutGraphVertex>*>& edges() const { return m_edges; }

    void addEdge(Edge<KargerMinCutGraphVertex>* e) { m_edges.push_back(e); }
    void clearEdges() { m_edges.clear(); }

private:
    std::vector<Edge<KargerMinCutGraphVertex>*> m_edges{};
};

class KargerMinCutGraph : public Graph<KargerMinCutGraphVertex, Edge<KargerMinCutGraphVertex>>
{
public:
    KargerMinCutGraph() = default;
    KargerMinCutGraph(const KargerMinCutGraph& rhs);
    KargerMinCutGraph(KargerMinCutGraph&& rhs) = delete;
    ~KargerMinCutGraph();

    KargerMinCutGraph& operator=(const KargerMinCutGraph& rhs) = delete;
    KargerMinCutGraph& operator=(KargerMinCutGraph&& rhs) = delete;

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }
    KargerMinCutGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    Edge<KargerMinCutGraphVertex>* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new KargerMinCutGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, KargerMinCutGraphVertex* u,
                 KargerMinCutGraphVertex* v, int /*value*/) override
    {
        auto e = new Edge<KargerMinCutGraphVertex>{id, u, v};
        u->addEdge(e);
        m_edges.push_back(e);
        ++id;

        if (this->type() == Type::Undirected) {
            e = new Edge<KargerMinCutGraphVertex>{id, v, u};
            v->addEdge(e);
            m_edges.push_back(e);
            ++id;
        }

        return id;
    }

    std::size_t minCut();

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override { m_edges.reserve(nedges); }

    std::size_t contract();
    Edge<KargerMinCutGraphVertex>* randomEdge();
    void removeEdgeAt(std::size_t i);
    void adjustEdges(const KargerMinCutGraphVertex* u,
                     const KargerMinCutGraphVertex* v,
                     KargerMinCutGraphVertex* newVertex);
    void resetWith(const KargerMinCutGraph& rhs);

    std::vector<KargerMinCutGraphVertex*> m_vertices{};
    std::vector<Edge<KargerMinCutGraphVertex>*> m_edges{};

    std::size_t m_nedges{0};
    std::default_random_engine m_randomEngine{};
};

} // namespace psa

#endif // PSA_MINCUTGRAPH_H
