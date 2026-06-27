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

void pozdeev::Application::handleClear()
{
  database_.clear();
  std::cout << "ALL SCENARIOS CLEARED\n";
}
