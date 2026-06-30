#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "avl_tree.hpp"
#include "scenario.hpp"

#include <string>

namespace pozdeev {

  class Application {
  public:
    Application();
    void run();

  private:
    AvlTree<std::string, Scenario> database_;
    bool isRunning_;

    using CommandHandler = void (Application::*)(const std::string&);
    AvlTree<std::string, CommandHandler> handlers_;

    void processCommand(const std::string& commandLine);
    std::string getToken(const std::string& line, size_t& pos) const;
    double getDoubleToken(const std::string& line, size_t& pos) const;

    void handleAddScenario(const std::string& args);
    void handleAddAction(const std::string& args);
    void handleFind(const std::string& args);
    void handleList(const std::string& args);
    void handleSimulate(const std::string& args);
    void handleAnalyze(const std::string& args);
    void handleCompare(const std::string& args);
    void handleBottleneck(const std::string& args);
    void handleStats(const std::string& args);
    void handleClear(const std::string& args);
    void handleExit(const std::string& args);
  };

}

#endif
