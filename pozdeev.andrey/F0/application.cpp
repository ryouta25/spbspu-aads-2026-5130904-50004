#include "application.hpp"

#include <iostream>
#include <iomanip>
#include <stdexcept>

namespace pozdeev {

  Application::Application() :
    isRunning_(true)
  {
    handlers_.insert("add_scenario", &Application::handleAddScenario);
    handlers_.insert("add_action", &Application::handleAddAction);
    handlers_.insert("find", &Application::handleFind);
    handlers_.insert("list", &Application::handleList);
    handlers_.insert("simulate", &Application::handleSimulate);
    handlers_.insert("analyze", &Application::handleAnalyze);
    handlers_.insert("compare", &Application::handleCompare);
    handlers_.insert("bottleneck", &Application::handleBottleneck);
    handlers_.insert("stats", &Application::handleStats);
    handlers_.insert("clear", &Application::handleClear);
    handlers_.insert("exit", &Application::handleExit);
  }

  void Application::run()
  {
    std::string line;
    while (isRunning_ && std::getline(std::cin, line)) {
      if (line.empty()) {
        continue;
      }
      try {
        processCommand(line);
      } catch (const std::invalid_argument& e) {
        std::cout << e.what() << "\n";
      }
    }
  }

  std::string Application::getToken(const std::string& line, size_t& pos) const
  {
    while (pos < line.length() && line[pos] == ' ') {
      ++pos;
    }
    if (pos >= line.length()) {
      return "";
    }
    size_t start = pos;
    while (pos < line.length() && line[pos] != ' ') {
      ++pos;
    }
    return line.substr(start, pos - start);
  }

  double Application::getDoubleToken(const std::string& line, size_t& pos) const
  {
    std::string token = getToken(line, pos);
    if (token.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }
    return std::stod(token);
  }

  void Application::processCommand(const std::string& commandLine)
  {
    size_t pos = 0;
    std::string command = getToken(commandLine, pos);

    auto it = handlers_.find(command);
    if (it != handlers_.end()) {
      CommandHandler handler = *it;
      (this->*handler)(commandLine.substr(pos));
    } else {
      throw std::invalid_argument("<UNKNOWN COMMAND>");
    }
  }

  void Application::handleAddScenario(const std::string& args)
  {
    size_t pos = 0;
    std::string id = getToken(args, pos);
    std::string match = getToken(args, pos);
    std::string team = getToken(args, pos);
    std::string map = getToken(args, pos);
    std::string type = getToken(args, pos);

    if (id.empty() || match.empty() || team.empty() || map.empty() || type.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    Scenario newScenario(id, match, team, map, type);
    if (database_.insert(id, newScenario)) {
      std::cout << "OK\n";
    } else {
      throw std::invalid_argument("<INVALID COMMAND: SCENARIO '" + id + "' ALREADY EXISTS>");
    }
  }

  void Application::handleAddAction(const std::string& args)
  {
    size_t pos = 0;
    std::string id = getToken(args, pos);
    double time = getDoubleToken(args, pos);
    std::string player = getToken(args, pos);
    std::string type = getToken(args, pos);
    std::string target = getToken(args, pos);
    double prob = getDoubleToken(args, pos);

    if (id.empty() || player.empty() || type.empty() || target.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    auto it = database_.find(id);
    if (it != database_.end()) {
      action_t action{time, player, type, target, prob};
      it->addAction(action);
      std::cout << "OK\n";
    } else {
      throw std::invalid_argument("<SCENARIO NOT FOUND>");
    }
  }

  void Application::handleFind(const std::string& args)
  {
    size_t pos = 0;
    std::string id = getToken(args, pos);

    if (id.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    auto it = database_.find(id);
    if (it != database_.end()) {
      std::cout << "FOUND [" << id << "]:\n";
      std::cout << *it << "\n";
      std::cout << "Actions count: " << it->getActions().getSize() << "\n";
    } else {
      throw std::invalid_argument("<NOT FOUND>");
    }
  }

  void Application::handleList(const std::string&)
  {
    for (auto it = database_.begin(); it != database_.end(); ++it) {
      std::cout << "[" << it.key() << "]: " << it->getTeam() << " - "
                << it->getMap() << " - " << it->getType() << " (Match: " << it->getMatch() << ")\n";
    }
  }

  void Application::handleSimulate(const std::string& args)
  {
    size_t pos = 0;
    std::string id = getToken(args, pos);

    if (id.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    auto it = database_.find(id);
    if (it != database_.end()) {
      std::cout << "--- SIMULATION: " << id << " ---\n";
      std::cout << "Match: " << it->getMatch() << " | Team: " << it->getTeam() << "\n";

      const Vector<action_t>& acts = it->getActions();
      for (size_t i = 0; i < acts.getSize(); ++i) {
        double remainingTime = 120.0 - acts[i].time;
        if (remainingTime < 0.0) {
          remainingTime = 0.0;
        }
        int mins = static_cast<int>(remainingTime) / 60;
        double secs = remainingTime - (mins * 60.0);

        std::cout << "[" << mins << ":";
        if (secs < 10.0) {
          std::cout << "0";
        }
        std::cout << std::fixed << std::setprecision(1) << secs << "] "
                  << acts[i].player << ": " << acts[i].type << " -> " << acts[i].target
                  << " (Prob: " << (acts[i].prob * 100.0) << "%)\n";
      }
      std::cout << "Simulation complete.\n";
    } else {
      throw std::invalid_argument("<SCENARIO NOT FOUND>");
    }
  }

  void Application::handleAnalyze(const std::string& args)
  {
    size_t pos = 0;
    std::string id = getToken(args, pos);

    if (id.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    auto it = database_.find(id);
    if (it != database_.end()) {
      AnalysisResult res = it->analyzeWeakness();
      std::cout << "ANALYSIS RESULT [" << id << "]:\n";
      if (!res.hasActions) {
        std::cout << "No actions to analyze.\n";
        return;
      }

      std::cout << "Total Win Probability: " << std::fixed << std::setprecision(1)
                << (res.totalProb * 100.0) << "%\n";

      double remainingTime = 120.0 - res.weakTime;
      if (remainingTime < 0.0) {
        remainingTime = 0.0;
      }
      int mins = static_cast<int>(remainingTime) / 60;
      double secs = remainingTime - (mins * 60.0);

      std::cout << "Critical Weakness: Action at " << mins << ":";
      if (secs < 10.0) {
        std::cout << "0";
      }
      std::cout << std::fixed << std::setprecision(1) << secs << " ("
                << res.weakPlayer << " " << res.weakType << ")\n";

      std::cout << "Impact: Failure reduces win chance to "
                << (res.impactProb * 100.0) << "%\n";
      std::cout << "Recommendation: Ensure high success rate for this action.\n";

    } else {
      throw std::invalid_argument("<SCENARIO NOT FOUND>");
    }
  }

  void Application::handleCompare(const std::string& args)
  {
    size_t pos = 0;
    std::string id1 = getToken(args, pos);
    std::string id2 = getToken(args, pos);

    if (id1.empty() || id2.empty()) {
      throw std::invalid_argument("<INVALID ARGUMENTS>");
    }

    auto it1 = database_.find(id1);
    auto it2 = database_.find(id2);

    if (it1 != database_.end() && it2 != database_.end()) {
      const double p1 = it1->calculateWinProbability() * 100.0;
      const double p2 = it2->calculateWinProbability() * 100.0;

      std::cout << "COMPARISON:\n";
      std::cout << "1. [" << id1 << "]: Win Prob " << std::fixed << std::setprecision(1) << p1 << "%\n";
      std::cout << "2. [" << id2 << "]: Win Prob " << p2 << "%\n";

      if (p1 > p2) {
        std::cout << "Winner: " << id1 << " (Higher efficiency by " << (p1 - p2) << "%)\n";
      } else if (p2 > p1) {
        std::cout << "Winner: " << id2 << " (Higher efficiency by " << (p2 - p1) << "%)\n";
      } else {
        std::cout << "Efficiency is equal.\n";
      }
    } else {
      throw std::invalid_argument("<ONE OR BOTH SCENARIOS NOT FOUND>");
    }
  }

  void Application::handleBottleneck(const std::string&)
  {
    if (database_.getSize() == 0) {
      std::cout << "DATABASE IS EMPTY\n";
      return;
    }

    auto it = database_.begin();
    std::string weakestId = it.key();
    double minProb = it->calculateWinProbability();
    ++it;

    for (; it != database_.end(); ++it) {
      const double prob = it->calculateWinProbability();
      if (prob < minProb) {
        minProb = prob;
        weakestId = it.key();
      }
    }

    std::cout << "GLOBAL BOTTLENECK DETECTED:\n";
    std::cout << "Scenario: [" << weakestId << "]\n";
    handleAnalyze(weakestId);
  }

  void Application::handleStats(const std::string&)
  {
    size_t totalActions = 0;
    double maxProb = 0.0;
    double minProb = 1.0;

    for (auto it = database_.begin(); it != database_.end(); ++it) {
      totalActions += it->getActions().getSize();
      const double prob = it->calculateWinProbability();
      if (prob > maxProb) {
        maxProb = prob;
      }
      if (prob < minProb) {
        minProb = prob;
      }
    }

    const double avgActions = database_.getSize() == 0 ? 0.0 : static_cast<double>(totalActions) / database_.getSize();
    if (database_.getSize() == 0) {
      minProb = 0.0;
    }

    std::cout << "TREE HEIGHT: " << database_.getHeight()
              << " | TOTAL SCENARIOS: " << database_.getSize() << "\n";
    std::cout << "Average Actions: " << std::fixed << std::setprecision(1) << avgActions
              << " | Max Win Rate: " << (maxProb * 100.0)
              << "% | Min Win Rate: " << (minProb * 100.0) << "%\n";
  }

  void Application::handleClear(const std::string&)
  {
    database_.clear();
    std::cout << "ALL SCENARIOS CLEARED\n";
  }

  void Application::handleExit(const std::string&)
  {
    std::cout << "(Программа завершает работу)\n";
    isRunning_ = false;
  }

}
