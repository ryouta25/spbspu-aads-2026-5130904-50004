#include "application.hpp"

#include <iostream>
#include <iomanip>
#include <stdexcept>

void pozdeev::Application::run()
{
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    processCommand(line);
  }
}

void pozdeev::Application::processCommand(const std::string& commandLine)
{
  std::istringstream stream(commandLine);
  std::string command;
  stream >> command;

  if (command == "add_scenario") {
    handleAddScenario(stream);
  } else if (command == "add_action") {
    handleAddAction(stream);
  } else if (command == "find") {
    handleFind(stream);
  } else if (command == "list") {
    handleList();
  } else if (command == "simulate") {
    handleSimulate(stream);
  } else if (command == "analyze") {
    handleAnalyze(stream);
  } else if (command == "compare") {
    handleCompare(stream);
  } else if (command == "bottleneck") {
    handleBottleneck();
  } else if (command == "stats") {
    handleStats();
  } else if (command == "clear") {
    handleClear();
  } else if (command == "exit") {
    std::cout << "(Программа завершает работу)\n";
    exit(0);
  } else {
    throw std::invalid_argument("<UNKNOWN COMMAND>");
  }
}

void pozdeev::Application::handleAddScenario(std::istringstream& stream)
{
  std::string id, match, team, map, type;
  if (stream >> id >> match >> team >> map >> type) {
    Scenario newScenario(id, match, team, map, type);
    if (database_.insert(id, newScenario)) {
      std::cout << "OK\n";
    } else {
      std::cout << "<INVALID COMMAND: SCENARIO '" << id << "' ALREADY EXISTS>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleAddAction(std::istringstream& stream)
{
  std::string id, player, type, target;
  double time = 0.0;
  double prob = 0.0;

  if (stream >> id >> time >> player >> type >> target >> prob) {
    Scenario* scenario = database_.find(id);
    if (scenario) {
      action_t action{time, player, type, target, prob};
      scenario->addAction(action);
      std::cout << "OK\n";
    } else {
      std::cout << "<SCENARIO NOT FOUND>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleFind(std::istringstream& stream) const
{
  std::string id;
  if (stream >> id) {
    Scenario* scenario = database_.find(id);
    if (scenario) {
      std::cout << "FOUND [" << id << "]:\n";
      std::cout << "Match: " << scenario->getMatch() << " | Team: " << scenario->getTeam()
                << " | Map: " << scenario->getMap() << " | Type: " << scenario->getType() << "\n";
      std::cout << "Actions count: " << scenario->getActions().getSize() << "\n";
    } else {
      std::cout << "<NOT FOUND>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleList() const
{
  Vector<const Scenario*> scenarios;
  database_.inOrderTraversal(scenarios);
  for (const Scenario* s : scenarios) {
    std::cout << "[" << s->getId() << "]: " << s->getTeam() << " - "
              << s->getMap() << " - " << s->getType() << " (Match: " << s->getMatch() << ")\n";
  }
}

void pozdeev::Application::handleSimulate(std::istringstream& stream) const
{
  std::string id;
  if (stream >> id) {
    Scenario* scenario = database_.find(id);
    if (scenario) {
      scenario->printSimulation();
    } else {
      std::cout << "<SCENARIO NOT FOUND>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleAnalyze(std::istringstream& stream) const
{
  std::string id;
  if (stream >> id) {
    Scenario* scenario = database_.find(id);
    if (scenario) {
      scenario->analyzeWeakness();
    } else {
      std::cout << "<SCENARIO NOT FOUND>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleCompare(std::istringstream& stream) const
{
  std::string id1, id2;
  if (stream >> id1 >> id2) {
    Scenario* s1 = database_.find(id1);
    Scenario* s2 = database_.find(id2);
    if (s1 && s2) {
      const double p1 = s1->calculateWinProbability() * 100.0;
      const double p2 = s2->calculateWinProbability() * 100.0;
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
      std::cout << "<ONE OR BOTH SCENARIOS NOT FOUND>\n";
    }
  } else {
    throw std::invalid_argument("<INVALID ARGUMENTS>");
  }
}

void pozdeev::Application::handleBottleneck() const
{
  Vector<const Scenario*> scenarios;
  database_.inOrderTraversal(scenarios);
  if (scenarios.isEmpty()) {
    std::cout << "DATABASE IS EMPTY\n";
    return;
  }

  const Scenario* weakest = scenarios[0];
  double minProb = weakest->calculateWinProbability();

  for (size_t i = 1; i < scenarios.getSize(); ++i) {
    const double prob = scenarios[i]->calculateWinProbability();
    if (prob < minProb) {
      minProb = prob;
      weakest = scenarios[i];
    }
  }

  std::cout << "GLOBAL BOTTLENECK DETECTED:\n";
  std::cout << "Scenario: [" << weakest->getId() << "]\n";
  weakest->analyzeWeakness();
}

void pozdeev::Application::handleStats() const
{
  Vector<const Scenario*> scenarios;
  database_.inOrderTraversal(scenarios);

  size_t totalActions = 0;
  double maxProb = 0.0;
  double minProb = 1.0;

  for (const Scenario* s : scenarios) {
    totalActions += s->getActions().getSize();
    const double prob = s->calculateWinProbability();
    if (prob > maxProb) maxProb = prob;
    if (prob < minProb) minProb = prob;
  }

  const double avgActions = scenarios.isEmpty() ? 0.0 : static_cast<double>(totalActions) / scenarios.getSize();
  if (scenarios.isEmpty()) {
    minProb = 0.0;
  }

  std::cout << "TREE HEIGHT: " << database_.getHeight()
            << " | TOTAL SCENARIOS: " << database_.getSize() << "\n";
  std::cout << "Average Actions: " << std::fixed << std::setprecision(1) << avgActions
            << " | Max Win Rate: " << (maxProb * 100.0)
            << "% | Min Win Rate: " << (minProb * 100.0) << "%\n";
}

void pozdeev::Application::handleClear()
{
  database_.clear();
  std::cout << "ALL SCENARIOS CLEARED\n";
}
