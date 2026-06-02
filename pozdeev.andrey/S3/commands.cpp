#include "commands.hpp"

namespace {

  template< class T >
  void sortVectorAscending(pozdeev::Vector< T > & vec)
  {
    if (vec.size() < 2) {
      return;
    }
    for (size_t i = 0; i < vec.size() - 1; ++i) {
      for (size_t j = 0; j < vec.size() - i - 1; ++j) {
        if (vec[j] > vec[j + 1]) {
          T temp = vec[j];
          vec[j] = vec[j + 1];
          vec[j + 1] = temp;
        }
      }
    }
  }

}

namespace pozdeev {

  void printGraphs(GraphTable & allGraphs, std::ostream & out)
  {
    Vector< std::string > names;
    for (auto it = allGraphs.begin(); it != allGraphs.end(); ++it) {
      names.pushBack(it.getKey());
    }

    sortVectorAscending(names);

    for (size_t i = 0; i < names.size(); ++i) {
      out << names[i] << '\n';
    }
  }

  void printVertexes(GraphTable & allGraphs, const std::string & graphName, std::ostream & out)
  {
    if (!allGraphs.has(graphName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph & g = allGraphs.get(graphName);
    Vector< std::string > verts = g.getVertexes();

    sortVectorAscending(verts);

    for (size_t i = 0; i < verts.size(); ++i) {
      out << verts[i] << '\n';
    }
  }

  void printOutbound(GraphTable & allGraphs, const std::string & graphName, const std::string & vertexName, std::ostream & out)
  {
    if (!allGraphs.has(graphName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph & g = allGraphs.get(graphName);
    const Vector< std::string > & verts = g.getVertexes();

    bool hasVertex = false;
    for (size_t i = 0; i < verts.size(); ++i) {
      if (verts[i] == vertexName) {
        hasVertex = true;
        break;
      }
    }

    if (!hasVertex) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    auto & edges = g.getEdges();
    Vector< std::string > destinations;

    for (auto it = edges.begin(); it != edges.end(); ++it) {
      std::pair< std::string, std::string > key = it.getKey();
      if (key.first == vertexName) {
        destinations.pushBack(key.second);
      }
    }

    sortVectorAscending(destinations);

    for (size_t i = 0; i < destinations.size(); ++i) {
      out << destinations[i];
      std::pair< std::string, std::string > edgeKey(vertexName, destinations[i]);
      Vector< unsigned int > weights = edges.get(edgeKey);

      sortVectorAscending(weights);

      for (size_t j = 0; j < weights.size(); ++j) {
        out << " " << weights[j];
      }
      out << '\n';
    }
  }

  void printInbound(GraphTable & allGraphs, const std::string & graphName, const std::string & vertexName, std::ostream & out)
  {
    if (!allGraphs.has(graphName)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph & g = allGraphs.get(graphName);
    const Vector< std::string > & verts = g.getVertexes();

    bool hasVertex = false;
    for (size_t i = 0; i < verts.size(); ++i) {
      if (verts[i] == vertexName) {
        hasVertex = true;
        break;
      }
    }

    if (!hasVertex) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    auto & edges = g.getEdges();
    Vector< std::string > origins;

    for (auto it = edges.begin(); it != edges.end(); ++it) {
      std::pair< std::string, std::string > key = it.getKey();
      if (key.second == vertexName) {
        origins.pushBack(key.first);
      }
    }

    sortVectorAscending(origins);

    for (size_t i = 0; i < origins.size(); ++i) {
      out << origins[i];
      std::pair< std::string, std::string > edgeKey(origins[i], vertexName);
      Vector< unsigned int > weights = edges.get(edgeKey);

      sortVectorAscending(weights);

      for (size_t j = 0; j < weights.size(); ++j) {
        out << " " << weights[j];
      }
      out << '\n';
    }
  }

  void processCommands(std::istream & in, std::ostream & out, GraphTable & allGraphs)
  {
    std::string cmd;
    while (in >> cmd) {
      if (cmd == "graphs") {
        printGraphs(allGraphs, out);
      } else if (cmd == "vertexes") {
        std::string graphName;
        in >> graphName;
        printVertexes(allGraphs, graphName, out);
      } else if (cmd == "outbound") {
        std::string graphName, vertexName;
        in >> graphName >> vertexName;
        printOutbound(allGraphs, graphName, vertexName, out);
      } else if (cmd == "inbound") {
        std::string graphName, vertexName;
        in >> graphName >> vertexName;
        printInbound(allGraphs, graphName, vertexName, out);
      } else {
        out << "<INVALID COMMAND>\n";
      }
    }
  }

}
