#include <boost/test/unit_test.hpp>
#include <string>
#include <cmath>

#include "scenario.hpp"

BOOST_AUTO_TEST_SUITE(Scenario_tests)

using Scenario = pozdeev::Scenario;
using action_t = pozdeev::action_t;

BOOST_AUTO_TEST_CASE(test_add_action_sorting)
{
  Scenario sc("navi_dust2", "grand_final", "NaVi", "Dust2", "Execute");

  action_t a1{2.5, "s1mple", "MOVE", "long", 1.0};
  action_t a2{1.0, "b1t", "THROW_SMOKE", "doors", 0.9};

  sc.addAction(a1);
  sc.addAction(a2);

  const auto& actions = sc.getActions();
  BOOST_CHECK_EQUAL(actions.getSize(), 2);

  BOOST_CHECK_EQUAL(actions[0].time, 1.0);
  BOOST_CHECK_EQUAL(actions[1].time, 2.5);
  BOOST_CHECK_EQUAL(actions[0].player, "b1t");
  BOOST_CHECK_EQUAL(actions[1].player, "s1mple");
}

BOOST_AUTO_TEST_CASE(test_calculate_win_probability)
{
  Scenario sc("test", "test", "test", "test", "test");

  BOOST_CHECK(std::abs(sc.calculateWinProbability() - 1.0) < 0.0001);

  action_t a1{1.0, "p1", "type", "target", 0.9};
  action_t a2{2.0, "p2", "type", "target", 0.8};

  sc.addAction(a1);
  sc.addAction(a2);

  BOOST_CHECK(std::abs(sc.calculateWinProbability() - 0.72) < 0.0001);
}

BOOST_AUTO_TEST_CASE(test_analyze_weakness)
{
  Scenario sc("test", "test", "test", "test", "test");

  action_t a1{1.0, "p1", "type", "target", 0.9};
  action_t a2{15.0, "weak_player", "type", "target", 0.1};
  action_t a3{30.0, "p3", "type", "target", 0.8};

  sc.addAction(a1);
  sc.addAction(a2);
  sc.addAction(a3);

  pozdeev::AnalysisResult result = sc.analyzeWeakness();

  BOOST_CHECK(result.hasActions == true);
  BOOST_CHECK(std::abs(result.minProb - 0.1) < 0.0001);
  BOOST_CHECK_EQUAL(result.weakPlayer, "weak_player");
  BOOST_CHECK_EQUAL(result.weakTime, 15.0);
}

BOOST_AUTO_TEST_SUITE_END()
