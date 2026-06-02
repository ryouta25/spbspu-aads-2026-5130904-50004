#include "graph.hpp"

namespace pozdeev {

  size_t PairHash::operator()(const std::pair< std::string, std::string > & p) const
  {
    StringHash h;
    return h(p.first) ^ (h(p.second) << 1);
  }

  bool PairEqual::operator()(const std::pair< std::string, std::string > & a, const std::pair< std::string, std::string > & b) const
  {
    return a.first == b.first && a.second == b.second;
  }

  void Graph::addVertex(const std::string & v)
  {
    for (size_t i = 0; i < vertexes_.size(); ++i) {
      if (vertexes_[i] == v) {
        return;
      }
    }
    vertexes_.pushBack(v);
  }

  void Graph::addEdge(const std::string & from, const std::string & to, unsigned int weight)
  {
    addVertex(from);
    addVertex(to);

    std::pair< std::string, std::string > key(from, to);
    if (!edges_.has(key)) {
      Vector< unsigned int > weights;
      weights.pushBack(weight);
      edges_.add(key, weights);
    } else {
      edges_.get(key).pushBack(weight);
    }
  }

  const Vector< std::string > & Graph::getVertexes() const
  {
    return vertexes_;
  }

  HashTable< std::pair< std::string, std::string >, Vector< unsigned int >, PairHash, PairEqual > & Graph::getEdges()
  {
    return edges_;
  }

}
