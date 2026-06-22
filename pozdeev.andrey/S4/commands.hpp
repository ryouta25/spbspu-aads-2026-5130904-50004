#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"
#include <string>
#include <iosfwd>

namespace pozdeev {

  using Dictionary = BSTree< int, std::string >;
  using DataSets = BSTree< std::string, Dictionary >;

  void parseFile(std::istream & in, DataSets & dataSets);
  void processCommands(std::istream & in, std::ostream & out, DataSets & dataSets);

}

#endif
