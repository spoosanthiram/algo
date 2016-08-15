/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_DIJKSTRAGRAPH_H
#define PSA_DIJKSTRAGRAPH_H

#include <limits>
#include <vector>

#include "Graph.h"

namespace psa {

class DijkstraGraphEdge;

class DijkstraGraphVertex : public Vertex
{
public:
    DijkstraGraphVertex(vertexid_t id) : Vertex{id} {}

    unsigned int distance() const { return m_distance; }
    DijkstraGraphVertex* parent()const { return m_parent; }
    const std::vector<DijkstraGraphEdge*>& edges() const { return m_edges; }

    void setDistance(unsigned int distance) { m_distance = distance; }
    void setParent(DijkstraGraphVertex* parent) { m_parent = parent; }

    void addEdge(DijkstraGraphEdge* edge) { m_edges.push_back(edge); }

private:
    unsigned int m_distance{std::numeric_limits<unsigned int>::max()};
    DijkstraGraphVertex *m_parent{nullptr};
    std::vector<DijkstraGraphEdge*> m_edges{};
};

class DijkstraGraphEdge : public Edge<DijkstraGraphVertex>
{
public:
    DijkstraGraphEdge(edgeid_t id,
                      DijkstraGraphVertex* u,
                      DijkstraGraphVertex* v,
                      unsigned int length);

    unsigned int length() const { return m_length; }

    void setLength(unsigned int length) { m_length = length; }

private:
    unsigned int m_length;
};

class DijkstraGraph : public Graph<DijkstraGraphVertex, DijkstraGraphEdge>
{
public:
    DijkstraGraph() = default;
    ~DijkstraGraph();

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }
    DijkstraGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    DijkstraGraphEdge* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new DijkstraGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, DijkstraGraphVertex* u,
                 DijkstraGraphVertex* v, int length) override
    {
        auto e = new DijkstraGraphEdge{id, u, v, static_cast<unsigned int>(length)};
        u->addEdge(e);
        m_edges.push_back(e);
        ++id;

        if (this->type() == Type::Undirected) {
            e = new DijkstraGraphEdge{id, v, u, static_cast<unsigned int>(length)};
            v->addEdge(e);
            m_edges.push_back(e);
            ++id;
        }

        return id;
    }

    void findShortestPath(vertexid_t sourceVertexId);

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override { m_edges.reserve(nedges); }

    std::vector<DijkstraGraphVertex*> m_vertices{};
    std::vector<DijkstraGraphEdge*> m_edges{};
};

} //namespace psa

#endif // PSA_DIJKSTRAGRAPH_H
