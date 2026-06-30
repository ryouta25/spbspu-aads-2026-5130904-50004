#include "scenario.hpp"

namespace pozdeev {

  Scenario::Scenario(const std::string& id, const std::string& match,
                     const std::string& team, const std::string& map,
                     const std::string& type)
      : id_(id)
      , match_(match)
      , team_(team)
      , map_(map)
      , type_(type)
  {}

  const std::string& Scenario::getId() const
  {
    return id_;
  }

  const std::string& Scenario::getMatch() const
  {
    return match_;
  }

  const std::string& Scenario::getTeam() const
  {
    return team_;
  }

  const std::string& Scenario::getMap() const
  {
    return map_;
  }

  const std::string& Scenario::getType() const
  {
    return type_;
  }

  const Vector<action_t>& Scenario::getActions() const
  {
    return actions_;
  }

  void Scenario::addAction(const action_t& action)
  {
    actions_.pushBack(action);
    quickSort(actions_.begin(), actions_.end(), [](const action_t& lhs, const action_t& rhs) {
      return lhs.time < rhs.time;
    });
  }

  double Scenario::calculateWinProbability() const
  {
    double prob = 1.0;
    for (size_t i = 0; i < actions_.getSize(); ++i) {
      prob *= actions_[i].prob;
    }
    return prob;
  }

  AnalysisResult Scenario::analyzeWeakness() const
  {
    AnalysisResult res{};
    res.hasActions = !actions_.isEmpty();
    if (!res.hasActions) {
      return res;
    }

    res.totalProb = calculateWinProbability();
    res.minProb = 1.0;
    size_t weakIndex = 0;

    for (size_t i = 0; i < actions_.getSize(); ++i) {
      if (actions_[i].prob < res.minProb) {
        res.minProb = actions_[i].prob;
        weakIndex = i;
      }
    }

    const action_t& weakAct = actions_[weakIndex];
    res.weakTime = weakAct.time;
    res.weakPlayer = weakAct.player;
    res.weakType = weakAct.type;
    res.impactProb = (res.minProb > 0.0) ? (res.totalProb / res.minProb) * (1.0 - res.minProb) : 0.0;

    return res;
  }

  std::ostream& operator<<(std::ostream& os, const Scenario& sc)
  {
    os << "Match: " << sc.getMatch() << " | Team: " << sc.getTeam()
       << " | Map: " << sc.getMap() << " | Type: " << sc.getType();
    return os;
  }

}
