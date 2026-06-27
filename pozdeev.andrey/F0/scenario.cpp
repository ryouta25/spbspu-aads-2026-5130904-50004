#include "scenario.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>

pozdeev::Scenario::Scenario(const std::string& id, const std::string& match,
                            const std::string& team, const std::string& map,
                            const std::string& type):
  id_(id),
  match_(match),
  team_(team),
  map_(map),
  type_(type)
{
}

const std::string& pozdeev::Scenario::getId() const
{
  return id_;
}

const std::string& pozdeev::Scenario::getMatch() const
{
  return match_;
}

const std::string& pozdeev::Scenario::getTeam() const
{
  return team_;
}

const std::string& pozdeev::Scenario::getMap() const
{
  return map_;
}

const std::string& pozdeev::Scenario::getType() const
{
  return type_;
}

const pozdeev::Vector<pozdeev::action_t>& pozdeev::Scenario::getActions() const
{
  return actions_;
}

void pozdeev::Scenario::addAction(const action_t& action)
{
  actions_.pushBack(action);
  std::sort(actions_.begin(), actions_.end(),
            [](const action_t& lhs, const action_t& rhs) -> bool {
              return lhs.time_ < rhs.time_;
            });
}

void pozdeev::Scenario::printSimulation() const
{
  std::cout << "--- SIMULATION: " << id_ << " ---\n";
  std::cout << "Match: " << match_ << " | Team: " << team_ << "\n";

  for (const action_t& act : actions_) {
    double remainingTime = 120.0 - act.time_;
    if (remainingTime < 0.0) {
      remainingTime = 0.0;
    }

    int minutes = static_cast<int>(remainingTime) / 60;
    double seconds = remainingTime - (minutes * 60.0);

    std::cout << "[" << minutes << ":";

    if (seconds < 10.0) {
      std::cout << "0";
    }

    std::cout << std::fixed << std::setprecision(1) << seconds << "] "
              << act.player_ << ": " << act.type_ << " -> " << act.target_
              << " (Prob: " << act.prob_ * 100.0 << "%)\n";
  }
  std::cout << "Simulation complete.\n";
}

double pozdeev::Scenario::calculateWinProbability() const
{
  if (actions_.isEmpty()) {
    return 0.0;
  }
  double totalProb = 1.0;
  for (const action_t& act : actions_) {
    totalProb *= act.prob_;
  }
  return totalProb;
}
