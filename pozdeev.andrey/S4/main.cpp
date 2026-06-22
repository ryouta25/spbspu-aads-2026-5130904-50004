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
    pozdeev::DataSets dataSets;
    pozdeev::parseFile(file, dataSets);
    pozdeev::processCommands(std::cin, std::cout, dataSets);
  } catch (const std::exception & e) {
    std::cerr << e.what() << '\n';
    return 2;
  }

  return 0;
}
