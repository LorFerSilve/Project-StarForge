#include "starforge/ai/ai.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::ai;

namespace {
NavigationSpace make_navigation() {
    NavigationSpace nav;
    REQUIRE(nav.add_node({NavNodeId{1}, true, 0, 0}));
    REQUIRE(nav.add_node({NavNodeId{2}, true, 0, 0}));
    REQUIRE(nav.add_node({NavNodeId{3}, true, 0x2, 0x4}));
    REQUIRE(nav.add_edge({NavNodeId{1}, NavNodeId{2}, 1, true}));
    REQUIRE(nav.add_edge({NavNodeId{2}, NavNodeId{3}, 1, true}));
    return nav;
}

AIRuntime make_runtime() {
    AIRuntime runtime;
    REQUIRE(runtime.add_actor({ActorId{1}, ActorKind::Crew, NavNodeId{1},
                               {Mobility::GroundStandard, 0x2, 0x4}}));
    REQUIRE(runtime.add_actor({ActorId{2}, ActorKind::Robot, NavNodeId{1},
                               {Mobility::GroundHeavy, 0x2, 0x4}}));
    REQUIRE(runtime.add_actor({ActorId{3}, ActorKind::Enemy, NavNodeId{1},
                               {Mobility::GroundStandard, 0x2, 0x4}}));
    REQUIRE(runtime.add_task({TaskId{1}, NavNodeId{3}, 10, false}));
    REQUIRE(runtime.assign_task(ActorId{1}, TaskId{1}));
    REQUIRE(runtime.set_robot_command(ActorId{2}, NavNodeId{3}, RulesOfEngagement::FreeFire));
    REQUIRE(runtime.form_squad(SquadId{1}, {ActorId{1}, ActorId{2}}));
    runtime.observe(ActorId{3}, {FactId{1}, ActorId{1}, 0, 100, 100});
    return runtime;
}
}  // namespace

TEST_CASE("navigation filters capability and access deterministically", "[ai][navigation]") {
    auto nav = make_navigation();
    const auto allowed = nav.find_path(NavNodeId{1}, NavNodeId{3},
                                       {Mobility::GroundStandard, 0x2, 0x4});
    REQUIRE(allowed);
    REQUIRE(*allowed == std::vector<NavNodeId>{NavNodeId{1}, NavNodeId{2}, NavNodeId{3}});
    REQUIRE_FALSE(nav.find_path(NavNodeId{1}, NavNodeId{3},
                                {Mobility::GroundStandard, 0x2, 0}));
}

TEST_CASE("navigation revision invalidates stale async path results", "[ai][navigation]") {
    auto nav = make_navigation();
    AsyncPathService paths;
    PathRequest request{PathRequestId{1}, ActorId{1}, NavNodeId{1}, NavNodeId{3},
                        {Mobility::GroundStandard, 0x2, 0x4}, nav.revision(), 7, 9};
    const auto result = paths.execute(nav, request);
    REQUIRE(result.reachable);
    REQUIRE(paths.fresh(result, nav.revision(), 7, 9));
    REQUIRE(nav.set_node_enabled(NavNodeId{2}, false));
    REQUIRE_FALSE(paths.fresh(result, nav.revision(), 7, 9));
}

TEST_CASE("knowledge rejects older observations and expires by simulation tick", "[ai][knowledge]") {
    KnowledgeStore knowledge;
    REQUIRE(knowledge.observe({FactId{1}, ActorId{7}, 10, 20, 80}));
    REQUIRE_FALSE(knowledge.observe({FactId{1}, ActorId{7}, 9, 30, 100}));
    const auto before = knowledge.digest();
    knowledge.expire(20);
    REQUIRE(knowledge.digest() == before);
    knowledge.expire(21);
    REQUIRE_FALSE(knowledge.fact(FactId{1}));
}

TEST_CASE("AI worker partition count cannot change authoritative outcome", "[ai][determinism]") {
    const auto nav = make_navigation();
    auto one = make_runtime();
    auto eight = make_runtime();
    one.advance(nav, 4, SimulationMode::Active, 1);
    eight.advance(nav, 4, SimulationMode::Active, 8);
    REQUIRE(one.authoritative_digest() == eight.authoritative_digest());
    REQUIRE(one.actor(ActorId{3})->tactical == TacticalState::Engage);
}

TEST_CASE("active and offscreen actor evolution is equivalent", "[ai][offscreen]") {
    const auto nav = make_navigation();
    auto active = make_runtime();
    auto offscreen = make_runtime();
    active.advance(nav, 4, SimulationMode::Active, 4);
    offscreen.advance(nav, 4, SimulationMode::Offscreen, 2);
    REQUIRE(active.authoritative_digest() == offscreen.authoritative_digest());
    REQUIRE(active.actor(ActorId{1})->location == NavNodeId{3});
    REQUIRE(active.actor(ActorId{2})->location == NavNodeId{3});
    REQUIRE(active.actor(ActorId{2})->roe == RulesOfEngagement::FreeFire);
}
