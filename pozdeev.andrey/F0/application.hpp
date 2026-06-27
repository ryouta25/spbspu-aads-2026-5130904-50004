#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "avl_tree.hpp"
#include "scenario.hpp"

#include <string>
#include <sstream>

namespace pozdeev {

  class Application {
  public:
    Application() = default;
    void run();

  private:
    AvlTree<std::string, Scenario> database_;

    void processCommand(const std::string& commandLine);
    void handleAddScenario(std::istringstream& stream);
    void handleAddAction(std::istringstream& stream);
    void handleFind(std::istringstream& stream) const;
    void handleList() const;
    void handleSimulate(std::istringstream& stream) const;
    void handleAnalyze(std::istringstream& stream) const;
    void handleCompare(std::istringstream& stream) const;
    void handleBottleneck() const;
    void handleStats() const;
    void handleClear();
  };

}

#endif
