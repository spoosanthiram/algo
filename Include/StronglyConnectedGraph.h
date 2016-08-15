/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_STRONGLYCONNECTEDGRAPH_H
#define PSA_STRONGLYCONNECTEDGRAPH_H

#include <unordered_map>
#include <vector>

#include "Graph.h"

namespace psa {

class StronglyConnectedGraphVertex : public Vertex
{
public:
    StronglyConnectedGraphVertex(vertexid_t id) : Vertex{id} {}

    bool explored() const { return m_explored; }
    int finishTime() const { return m_finishTime; }
    StronglyConnectedGraphVertex* leader() const { return m_leader; }
    const std::vector<Edge<StronglyConnectedGraphVertex>*>& edges() const { return m_edges; }
    const std::vector<Edge<StronglyConnectedGraphVertex>*>& reverseEdges() const
    {
        return m_reverseEdges;
    }

    void setExplored(bool explored) { m_explored = explored; }
    void setFinishTime(int time) { m_finishTime = time; }
    void setLeader(StronglyConnectedGraphVertex* leader) { m_leader = leader; }

    void addEdge(Edge<StronglyConnectedGraphVertex>* e) { m_edges.push_back(e); }
    void addReverseEdge(Edge<StronglyConnectedGraphVertex>* e) { m_reverseEdges.push_back(e); }

private:
    bool m_explored{false};
    int m_finishTime{-1};
    StronglyConnectedGraphVertex* m_leader{nullptr};
    std::vector<Edge<StronglyConnectedGraphVertex>*> m_edges{};
    std::vector<Edge<StronglyConnectedGraphVertex>*> m_reverseEdges{};
};

class StronglyConnectedGraph :
    public Graph<StronglyConnectedGraphVertex, Edge<StronglyConnectedGraphVertex>>
{
public:
    StronglyConnectedGraph() = default;
    ~StronglyConnectedGraph();

    std::size_t nvertices() const override { return m_vertices.size(); }
    std::size_t nedges() const override { return m_edges.size(); }
    StronglyConnectedGraphVertex* vertex(vertexid_t id) const override { return m_vertices[id]; }
    Edge<StronglyConnectedGraphVertex>* edge(edgeid_t id) const override { return m_edges[id]; }

    void addVertex(vertexid_t id) override
    {
        auto v = new StronglyConnectedGraphVertex{id};
        m_vertices.push_back(v);
    }
    edgeid_t addEdge(edgeid_t id, StronglyConnectedGraphVertex* u,
                 StronglyConnectedGraphVertex* v, int /*value*/) override;

    std::unordered_map<vertexid_t, unsigned int> kosaraju();

private:
    void reserveVertices(std::size_t nvertices) override { m_vertices.reserve(nvertices); }
    void reserveEdges(std::size_t nedges) override
    {
        m_edges.reserve(nedges);
        m_reverseEdges.reserve(nedges);
    }

    void dfsLoopReverse();
    void dfsReverse(StronglyConnectedGraphVertex* v);
    void dfsLoop();
    void dfs(StronglyConnectedGraphVertex* v, StronglyConnectedGraphVertex* leader);

    std::vector<StronglyConnectedGraphVertex*> m_vertices{};
    std::vector<StronglyConnectedGraphVertex*> m_verticesByFinishTime{};
    std::vector<Edge<StronglyConnectedGraphVertex>*> m_edges{};
    std::vector<Edge<StronglyConnectedGraphVertex>*> m_reverseEdges{};
};

} // namespace psa

#endif // PSA_STRONGLYCONNECTEDGRAPH_H
