/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_TRAVERSALGRAPH_H
#define PSA_TRAVERSALGRAPH_H

#include <vector>

#include "Graph.h"

namespace psa {

class BreadthFirstGraphVertex : public Vertex
{
public:
    BreadthFirstGraphVertex(vertexid_t id) : Vertex{id} {}

    bool isExplored() const { return m_explored; }
    int distance() const { return m_distance; }
    const std::vector<Edge<BreadthFirstGraphVertex>*>& edges() const { return m_edges; }
    std::string toString() const override;

    void setExplored(bool explored) { m_explored = explored; }
    void setDistance(unsigned int distance) { m_distance = distance; }

    void addEdge(Edge<BreadthFirstGraphVertex>* e) { m_edges.push_back(e); }

private:
    bool m_explored{false};
    int m_distance{-1};
    std::vector<Edge<BreadthFirstGraphVertex>*> m_edges{};
};

class BreadthFirstGraph : public Graph<BreadthFirstGraphVertex, Edge<BreadthFirstGraphVertex>>
{
public:
    BreadthFirstGraph() = default;
    ~BreadthFirstGraph();

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }
    BreadthFirstGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    Edge<BreadthFirstGraphVertex>* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new BreadthFirstGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, BreadthFirstGraphVertex* u,
                 BreadthFirstGraphVertex* v, int /*value*/) override
    {
        auto e = new Edge<BreadthFirstGraphVertex>{id, u, v};
        u->addEdge(e);
        m_edges.push_back(e);
        ++id;

        if (this->type() == Type::Undirected) {
            e = new Edge<BreadthFirstGraphVertex>{id, v, u};
            v->addEdge(e);
            m_edges.push_back(e);
            ++id;
        }

        return id;
    }

    void traverse(vertexid_t startVertexId);
    unsigned int distance(vertexid_t startVertexId, vertexid_t endVertexId);

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override { m_edges.reserve(nedges); }

    std::vector<BreadthFirstGraphVertex*> m_vertices{};
    std::vector<Edge<BreadthFirstGraphVertex>*> m_edges{};
};

} // namespace psa

#endif // PSA_TRAVERSALGRAPH_H
