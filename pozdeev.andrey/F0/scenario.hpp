#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "vector.hpp"
#include <string>
#include <iostream>

namespace pozdeev {

  struct action_t {
    double time;
    std::string player;
    std::string type;
    std::string target;
    double prob;
  };

  struct AnalysisResult {
    bool hasActions;
    double totalProb;
    double minProb;
    double weakTime;
    std::string weakPlayer;
    std::string weakType;
    double impactProb;
  };

  class Scenario {
  public:
    Scenario() = default;
    Scenario(const std::string& id, const std::string& match,
             const std::string& team, const std::string& map,
             const std::string& type);

    const std::string& getId() const;
    const std::string& getMatch() const;
    const std::string& getTeam() const;
    const std::string& getMap() const;
    const std::string& getType() const;
    const Vector<action_t>& getActions() const;

    void addAction(const action_t& action);
    double calculateWinProbability() const;
    AnalysisResult analyzeWeakness() const;

    friend std::ostream& operator<<(std::ostream& os, const Scenario& sc);

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
