#include <boost/test/unit_test.hpp>

#include "scenario.hpp"

BOOST_AUTO_TEST_SUITE(ScenarioTests)

BOOST_AUTO_TEST_CASE(testScenarioCreation)
{
  pozdeev::Scenario sc("navi_dust2", "grand_final", "NaVi", "Dust2", "Execute");

  BOOST_CHECK_EQUAL(sc.getId(), "navi_dust2");
  BOOST_CHECK_EQUAL(sc.getMatch(), "grand_final");
  BOOST_CHECK_EQUAL(sc.getTeam(), "NaVi");
  BOOST_CHECK_EQUAL(sc.getMap(), "Dust2");
  BOOST_CHECK_EQUAL(sc.getType(), "Execute");
}

BOOST_AUTO_TEST_CASE(testAddActionSorting)
{
  pozdeev::Scenario sc("id1", "m1", "t1", "map", "type");

  pozdeev::action_t actionLate{2.5, "s1mple", "MOVE", "long", 1.0};
  pozdeev::action_t actionEarly{1.0, "b1t", "THROW_SMOKE", "doors", 0.9};

  sc.addAction(actionLate);
  sc.addAction(actionEarly);

  const auto& actions = sc.getActions();
  BOOST_REQUIRE_EQUAL(actions.getSize(), 2);

  BOOST_CHECK_EQUAL(actions[0].time_, 1.0);
  BOOST_CHECK_EQUAL(actions[1].time_, 2.5);
  BOOST_CHECK_EQUAL(actions[0].player_, "b1t");
}

BOOST_AUTO_TEST_CASE(testWinProbabilityCalculation)
{
  pozdeev::Scenario sc("id1", "m1", "t1", "map", "type");

  BOOST_CHECK_CLOSE(sc.calculateWinProbability(), 0.0, 1e-5);

  pozdeev::action_t a1{1.0, "p1", "MOVE", "a", 0.90};
  pozdeev::action_t a2{2.0, "p2", "SHOOT", "b", 0.80};

  sc.addAction(a1);
  sc.addAction(a2);

  BOOST_CHECK_CLOSE(sc.calculateWinProbability(), 0.72, 1e-5);
}

BOOST_AUTO_TEST_SUITE_END()
