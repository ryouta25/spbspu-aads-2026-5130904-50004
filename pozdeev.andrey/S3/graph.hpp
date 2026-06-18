#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "vector.hpp"
#include "hashTable.hpp"
#include <string>

namespace pozdeev {

  struct PairHash {
    size_t operator()(const std::pair< std::string, std::string > & p) const;
  };

  struct PairEqual {
    bool operator()(const std::pair< std::string, std::string > & a, const std::pair< std::string, std::string > & b) const;
  };

  class Graph {
  public:
    void addVertex(const std::string & v);
    void addEdge(const std::string & from, const std::string & to, unsigned int weight);

    const Vector< std::string > & getVertexes() const;
    HashTable< std::pair< std::string, std::string >, Vector< unsigned int >, PairHash, PairEqual > & getEdges();

  private:
    Vector< std::string > vertexes_;
    HashTable< std::pair< std::string, std::string >, Vector< unsigned int >, PairHash, PairEqual > edges_;
  };

}

#endif
