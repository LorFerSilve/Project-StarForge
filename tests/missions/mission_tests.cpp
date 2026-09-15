#include "starforge/missions/missions.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::missions;

namespace {
MissionRecord make_mission(MissionId id, bool retryable = true) {
    MissionRecord mission{};
    mission.id = id;
    mission.retryable = retryable;
    mission.generated = {UINT64_C(0x12345678), 1, 2, 3, 100};
    mission.objectives = {{ObjectiveId{1}, {}, true, false},
                          {ObjectiveId{2}, {ObjectiveId{1}}, true, false}};
    return mission;
}
}  // namespace

TEST_CASE("IMP-9 objective DAG commits exactly once and gates extraction") {
    MissionRuntime runtime{};
    REQUIRE(runtime.add_mission(make_mission(MissionId{10})));
    const auto deployment = runtime.deploy(MissionId{10}, 42);
    REQUIRE(deployment);
    REQUIRE_FALSE(runtime.commit_objective(deployment.value(), ObjectiveId{1}, 99));
    REQUIRE(runtime.activate(deployment.value()));
    REQUIRE_FALSE(runtime.commit_objective(deployment.value(), ObjectiveId{2}, 100));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{1}, 100));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{1}, 100));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{2}, 101));
    REQUIRE(runtime.mission(MissionId{10})->state == MissionState::ExtractionAvailable);
    REQUIRE(runtime.secure_extraction(deployment.value()));
    REQUIRE(runtime.extract(deployment.value(), 102));
    REQUIRE(runtime.mission(MissionId{10})->state == MissionState::Succeeded);
    REQUIRE(runtime.instance(deployment.value())->rewards_applied);
    REQUIRE_FALSE(runtime.active_external_instance().has_value());
}

TEST_CASE("IMP-9 deployment enforces one external mission and retry uses a new instance") {
    MissionRuntime runtime{};
    REQUIRE(runtime.add_mission(make_mission(MissionId{20})));
    REQUIRE(runtime.add_mission(make_mission(MissionId{21})));
    const auto first = runtime.deploy(MissionId{20}, 1);
    REQUIRE(first);
    REQUIRE_FALSE(runtime.deploy(MissionId{21}, 1));
    REQUIRE(runtime.fail(first.value()));
    REQUIRE(runtime.prepare_retry(MissionId{20}));
    const auto retry = runtime.deploy(MissionId{20}, 2);
    REQUIRE(retry);
    REQUIRE(retry.value() != first.value());
    REQUIRE_FALSE(runtime.fail(first.value()));
    REQUIRE(runtime.active_external_instance() == retry.value());
    REQUIRE_FALSE(runtime.deploy(MissionId{21}, 3));
    REQUIRE(runtime.instance(retry.value())->attempt_ordinal == 2);
    REQUIRE(runtime.mission(MissionId{20})->generated == make_mission(MissionId{20}).generated);
}

TEST_CASE("IMP-9 failed mission cannot be extracted") {
    MissionRuntime runtime{};
    REQUIRE(runtime.add_mission(make_mission(MissionId{22})));
    const auto deployment = runtime.deploy(MissionId{22}, 1);
    REQUIRE(deployment);
    REQUIRE(runtime.activate(deployment.value()));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{1}, 1));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{2}, 2));
    REQUIRE(runtime.secure_extraction(deployment.value()));
    REQUIRE(runtime.fail(deployment.value()));
    REQUIRE_FALSE(runtime.extract(deployment.value(), 3));
    REQUIRE(runtime.mission(MissionId{22})->state == MissionState::Failed);
}

TEST_CASE("IMP-9 valid converging objective DAG is accepted") {
    MissionRuntime runtime{};
    auto mission = make_mission(MissionId{23});
    mission.objectives = {{ObjectiveId{1}, {}, true, false},
                          {ObjectiveId{2}, {ObjectiveId{1}}, true, false},
                          {ObjectiveId{3}, {ObjectiveId{1}}, true, false},
                          {ObjectiveId{4}, {ObjectiveId{2}, ObjectiveId{3}}, true, false}};
    REQUIRE(runtime.add_mission(std::move(mission)));
}

TEST_CASE("IMP-9 procedural offers are stable and cursor prevents reload rerolls") {
    GenerationCursor left{77, 0, 1};
    GenerationCursor right{77, 0, 1};
    const GenerationRequest request{123456, 77, 1, 3, 8};
    const auto first = generate_offer(request, left);
    const auto replay = generate_offer(request, right);
    REQUIRE(first);
    REQUIRE(replay);
    REQUIRE(first.value() == replay.value());
    REQUIRE(left.ordinal == 1);
    REQUIRE(left.revision == 2);
    REQUIRE_FALSE(generate_offer(request, left));
    const GenerationRequest next_request{123456, 77, 2, 3, 8};
    const auto next = generate_offer(next_request, left);
    REQUIRE(next);
    REQUIRE(next.value() != first.value());
}

TEST_CASE("IMP-9 save load preserves mission instance objective hazard and extraction state") {
    MissionRuntime runtime{};
    REQUIRE(runtime.add_mission(make_mission(MissionId{30})));
    const auto deployment = runtime.deploy(MissionId{30}, 55);
    REQUIRE(deployment);
    REQUIRE(runtime.activate(deployment.value()));
    REQUIRE(runtime.commit_objective(deployment.value(), ObjectiveId{1}, 500));
    REQUIRE(runtime.advance_hazards(deployment.value(), 99));
    const auto encoded = runtime.serialize();
    auto decoded = MissionRuntime::deserialize(encoded);
    REQUIRE(decoded);
    const auto* instance = decoded.value().instance(deployment.value());
    REQUIRE(instance != nullptr);
    REQUIRE(instance->hazard_tick == 99);
    REQUIRE(instance->objectives.front().state == ObjectiveState::Completed);
    REQUIRE(decoded.value().active_external_instance() == deployment.value());
    REQUIRE(decoded.value().commit_objective(deployment.value(), ObjectiveId{1}, 500));
    REQUIRE(decoded.value().commit_objective(deployment.value(), ObjectiveId{2}, 501));
    REQUIRE(decoded.value().secure_extraction(deployment.value()));
    REQUIRE(decoded.value().extract(deployment.value(), 502));
}

TEST_CASE("IMP-9 load rejects broken mission references and active slots") {
    REQUIRE_FALSE(MissionRuntime::deserialize("SFM1 2 1 0 1\nI 1 99 1 0 0 0 0 1 0 0\n"));
    REQUIRE_FALSE(MissionRuntime::deserialize("SFM1 2 99 0 0\n"));
}

TEST_CASE("IMP-9 invalid objective cycles are rejected") {
    MissionRuntime runtime{};
    auto mission = make_mission(MissionId{40});
    mission.objectives = {{ObjectiveId{1}, {ObjectiveId{2}}, true, false},
                          {ObjectiveId{2}, {ObjectiveId{1}}, true, false}};
    const auto result = runtime.add_mission(std::move(mission));
    REQUIRE_FALSE(result);
    REQUIRE(result.error() == MissionError::InvalidObjectiveGraph);
}
