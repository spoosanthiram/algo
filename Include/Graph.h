/**
 * Copyright 2016, Saravanan Poosanthiram
 * All rights reserved.
 */

#ifndef PSA_GRAPH_H
#define PSA_GRAPH_H

#include <istream>
#include <ostream>
#include <regex>
#include <string>
#include <unordered_map>

#include <fmt/format.h>

#include "AlgoException.h"

namespace psa {

using vertexid_t = unsigned int;
#ifdef WINDOWS
using edgeid_t = unsigned long long;
#else
using edgeid_t = unsigned long;
#endif // WINDOWS

// Heap indexing function object used for Dijkstra shortest path, Prim's minimum spanning tree
template<typename VertexType> struct HeapIndex
{
    HeapIndex(std::unordered_map<vertexid_t, std::size_t>& map) : m_map{map} {}
    void operator()(VertexType* element, std::size_t i)
    {
        m_map[element->id()] = i;
    }
    std::unordered_map<vertexid_t, std::size_t>& m_map;
};

/**
 * Vertex class abstracts Graph vertex. Edges list structure is left to derived
 * class to define as it is well informed to make which kind will suit for the need.
 */
class Vertex
{
public:
    Vertex(vertexid_t id) : m_id{id} {}
    virtual ~Vertex() {}

    vertexid_t id() const { return m_id; }
    virtual std::string toString() const { return std::to_string(m_id); }

private:
    vertexid_t m_id;
};

/**
 * Simple Edge class.
 */
template<typename VertexType> class Edge
{
public:
    Edge(edgeid_t id, VertexType* u, VertexType* v)
        : m_id{id}
        , m_u{u}
        , m_v{v}
    {}

    edgeid_t id() const { return m_id; }
    VertexType* u() const { return m_u; }
    VertexType* v() const { return m_v; }

    void setId(edgeid_t id) { m_id = id; }
    void setU(VertexType* u) { m_u = u; }
    void setV(VertexType* v) { m_v = v; }

private:
    edgeid_t m_id;
    VertexType* m_u;
    VertexType* m_v;
};

/**
 * Graph class abstract common behaviors for graph.
 */
template<typename VertexType, typename EdgeType>
class Graph
{
public:
    static const char kSeparator = '\t';
    enum class Type { Directed, Undirected };

public:
    Graph() {}
    virtual ~Graph() {}

    Type type() const { return m_type; }
    virtual std::size_t nvertices() const = 0;
    virtual std::size_t nedges() const = 0;
    virtual VertexType* vertex(vertexid_t id) const = 0;
    virtual EdgeType* edge(edgeid_t id) const = 0;

    void setType(Type type) { m_type = type; }
    virtual void addVertex(vertexid_t id) = 0;
    virtual edgeid_t addEdge(edgeid_t id, VertexType* u, VertexType* v, int value) = 0;

    void readAdjList(std::istream& stream);
    void writeAdjList(std::ostream& stream);

protected:
    virtual void reserveVertices(std::size_t nvertices) = 0;
    virtual void reserveEdges(std::size_t nedges) = 0;

    void readEdges(std::istream& stream);

private:
    Type m_type;
};

template<typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::readAdjList(std::istream& stream)
{
    std::string buf;

    std::getline(stream, buf); // Read type: "directed" or "undirected"
    if (buf == "directed")
        m_type = Type::Directed;
    else if (buf == "undirected")
        m_type = Type::Undirected;
    else
        throw AlgoException{fmt::format(AlgoException::GraphBadFormat,
                                        "directed or undirected", buf)};

    std::getline(stream, buf); // Read line that has number of vertices and edges.

    std::smatch results;
    if (!std::regex_search(buf, results, std::regex{R"(^(\d+)(\s+\d+)?)"}))
        throw AlgoException{fmt::format(AlgoException::GraphBadFormat,
                                        "number of nodes followed by optional number of edges",
                                        buf)};

    unsigned int nvertices = std::stoi(results[1]);
    this->reserveVertices(nvertices);
    if (results[2].matched) {
        long nedges = std::stol(results[2]);
        this->reserveEdges(nedges);
    }

    // Create all vertices and add to graph
    for (vertexid_t i = 0; i < nvertices; ++i)
        this->addVertex(i);

    this->readEdges(stream);
}

template<typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::writeAdjList(std::ostream& stream)
{
    std::size_t nvertices = this->nvertices();

    stream << nvertices << kSeparator << this->nedges() << std::endl;

    for (std::size_t id = 1; id <= nvertices; ++id)
        stream << this->vertex(id)->toString() << '\n';
}

template<typename VertexType, typename EdgeType>
void Graph<VertexType, EdgeType>::readEdges(std::istream& stream)
{
    auto pattern = std::regex{R"(\s+)"};

    edgeid_t edgeid{0};
    int value{};

    std::string buf;
    while (!stream.eof()) {
        std::getline(stream, buf);
        if (buf.size() == 0) // empty string
            continue;

        auto it = std::sregex_token_iterator{buf.begin(), buf.end(), pattern, -1};
        const std::sregex_token_iterator endIt{};

        int id = std::stoi(it->str());
        VertexType* u = this->vertex(id);

        std::string str{};
        for (++it; it != endIt; ++it) {
            str = it->str();

            std::size_t i = str.find(",");
            if (i != std::string::npos) {
                id = std::stoi(str.substr(0, i));
                value = std::stoi(str.substr(i+1));
            } else {
                id = std::stoi(it->str());
                value = 0;
            }

            VertexType* v = this->vertex(id);

            edgeid = this->addEdge(edgeid, u, v, value);
        }
    }
}

} // namespace psa

#endif // PSA_GRAPH_H
