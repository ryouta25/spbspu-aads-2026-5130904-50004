#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "vector.hpp"

#include <string>

namespace pozdeev {

  struct action_t {
    double time_;
    std::string player_;
    std::string type_;
    std::string target_;
    double prob_;
  };

  class Scenario {
  public:
    Scenario() = default;
    Scenario(const std::string& id, const std::string& match, const std::string& team,
             const std::string& map, const std::string& type);

    const std::string& getId() const;
    const std::string& getMatch() const;
    const std::string& getTeam() const;
    const std::string& getMap() const;
    const std::string& getType() const;
    const Vector<action_t>& getActions() const;

    void addAction(const action_t& action);
    void printSimulation() const;
    double calculateWinProbability() const;
    void analyzeWeakness() const;

  private:
    std::string id_;
    std::string match_;
    std::string team_;
    std::string map_;
    std::string type_;
    Vector<action_t> actions_;
  };

}

#endif
