#include "scenario.hpp"

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
