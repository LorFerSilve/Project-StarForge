#pragma once

#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <set>
#include <vector>

namespace starforge::ai {

using ActorId = core::StrongId<struct AIActorIdTag>;
using NavNodeId = core::StrongId<struct NavNodeIdTag>;
using PathRequestId = core::StrongId<struct PathRequestIdTag>;
using FactId = core::StrongId<struct KnowledgeFactIdTag>;
using TaskId = core::StrongId<struct AITaskIdTag>;
using SquadId = core::StrongId<struct AISquadIdTag>;

enum class Mobility : std::uint8_t { GroundSmall, GroundStandard, GroundHeavy, FreeFlight };
enum class ActorKind : std::uint8_t { Enemy, Crew, Robot };
enum class TacticalState : std::uint8_t { Idle, Investigate, Engage, Retreat };
enum class RulesOfEngagement : std::uint8_t { HoldFire, Defensive, FreeFire };
enum class SimulationMode : std::uint8_t { Active, Offscreen };

struct TraversalProfile {
    Mobility mobility{Mobility::GroundStandard};
    std::uint32_t capability_mask{0};
    std::uint32_t access_mask{0};
};

struct NavigationNode {
    NavNodeId id{};
    bool enabled{true};
    std::uint32_t required_capabilities{0};
    std::uint32_t required_access{0};
};

struct NavigationEdge {
    NavNodeId from{};
    NavNodeId to{};
    std::uint32_t cost{1};
    bool enabled{true};
};

class NavigationSpace {
public:
    bool add_node(NavigationNode node);
    bool add_edge(NavigationEdge edge);
    bool set_node_enabled(NavNodeId id, bool enabled);
    [[nodiscard]] std::optional<std::vector<NavNodeId>> find_path(
        NavNodeId start, NavNodeId goal, const TraversalProfile& profile) const;
    [[nodiscard]] std::uint64_t revision() const noexcept { return revision_; }

private:
    std::map<NavNodeId, NavigationNode> nodes_;
    std::vector<NavigationEdge> edges_;
    std::uint64_t revision_{0};
};

struct PathRequest {
    PathRequestId id{};
    ActorId actor{};
    NavNodeId start{};
    NavNodeId goal{};
    TraversalProfile profile{};
    std::uint64_t navigation_revision{0};
    std::uint64_t actor_generation{0};
    std::uint64_t sequence{0};
};

struct PathResult {
    PathRequest request{};
    std::vector<NavNodeId> path;
    bool reachable{false};
};

class AsyncPathService {
public:
    [[nodiscard]] PathResult execute(const NavigationSpace& navigation, PathRequest request) const;
    [[nodiscard]] bool fresh(const PathResult& result, std::uint64_t navigation_revision,
                             std::uint64_t actor_generation, std::uint64_t latest_sequence) const noexcept;
};

struct KnowledgeFact {
    FactId id{};
    ActorId subject{};
    std::uint64_t observed_tick{0};
    std::uint64_t expires_tick{0};
    std::uint32_t confidence{0};
};

class KnowledgeStore {
public:
    bool observe(KnowledgeFact fact);
    void expire(std::uint64_t tick);
    [[nodiscard]] std::optional<KnowledgeFact> fact(FactId id) const;
    [[nodiscard]] std::uint64_t digest() const noexcept;

private:
    std::map<FactId, KnowledgeFact> facts_;
};

struct ActorState {
    ActorId id{};
    ActorKind kind{ActorKind::Crew};
    NavNodeId location{};
    TraversalProfile profile{};
    TacticalState tactical{TacticalState::Idle};
    RulesOfEngagement roe{RulesOfEngagement::Defensive};
    std::optional<TaskId> task;
    std::optional<SquadId> squad;
    std::uint64_t generation{1};
};

struct Task {
    TaskId id{};
    NavNodeId destination{};
    std::uint8_t priority{0};
    bool complete{false};
};

class AIRuntime {
public:
    bool add_actor(ActorState actor);
    bool add_task(Task task);
    bool assign_task(ActorId actor, TaskId task);
    bool set_robot_command(ActorId actor, NavNodeId destination, RulesOfEngagement roe);
    bool form_squad(SquadId squad, const std::vector<ActorId>& members);
    void observe(ActorId observer, KnowledgeFact fact);
    void advance(const NavigationSpace& navigation, std::uint64_t ticks, SimulationMode mode,
                 std::uint32_t worker_partitions = 1);
    [[nodiscard]] const ActorState* actor(ActorId id) const;
    [[nodiscard]] std::uint64_t tick() const noexcept { return tick_; }
    [[nodiscard]] std::uint64_t authoritative_digest() const noexcept;

private:
    std::map<ActorId, ActorState> actors_;
    std::map<TaskId, Task> tasks_;
    std::map<SquadId, std::set<ActorId>> squads_;
    KnowledgeStore knowledge_;
    std::uint64_t tick_{0};
};

}  // namespace starforge::ai
