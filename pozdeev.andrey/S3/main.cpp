#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char ** argv)
{
  if (argc < 2) {
    std::cerr << "File name not provided.\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Failed to open file.\n";
    return 1;
  }

  try {
    pozdeev::GraphTable allGraphs(32);

    std::string graphName;
    size_t edgesCount = 0;

    while (file >> graphName >> edgesCount) {
      pozdeev::Graph g;
      for (size_t i = 0; i < edgesCount; ++i) {
        std::string from, to;
        unsigned int weight;
        file >> from >> to >> weight;
        g.addEdge(from, to, weight);
      }

      if (!allGraphs.has(graphName)) {
        allGraphs.add(graphName, g);
      } else {
        allGraphs.get(graphName) = g;
      }
    }

    pozdeev::processCommands(std::cin, std::cout, allGraphs);
  } catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
