#include "starforge/ai/ai.hpp"

#include <algorithm>
#include <limits>
#include <queue>
#include <utility>

namespace starforge::ai {

bool NavigationSpace::add_node(NavigationNode node) {
    if (!node.id || nodes_.contains(node.id)) return false;
    nodes_.emplace(node.id, node);
    ++revision_;
    return true;
}

bool NavigationSpace::add_edge(NavigationEdge edge) {
    if (!edge.from || !edge.to || edge.cost == 0 || !nodes_.contains(edge.from) || !nodes_.contains(edge.to)) return false;
    edges_.push_back(edge);
    std::ranges::sort(edges_, {}, [](const NavigationEdge& value) { return std::pair{value.from, value.to}; });
    ++revision_;
    return true;
}

bool NavigationSpace::set_node_enabled(NavNodeId id, bool enabled) {
    const auto it = nodes_.find(id);
    if (it == nodes_.end() || it->second.enabled == enabled) return false;
    it->second.enabled = enabled;
    ++revision_;
    return true;
}

std::optional<std::vector<NavNodeId>> NavigationSpace::find_path(
    NavNodeId start, NavNodeId goal, const TraversalProfile& profile) const {
    const auto traversable = [&](NavNodeId id) {
        const auto it = nodes_.find(id);
        return it != nodes_.end() && it->second.enabled &&
               (it->second.required_capabilities & ~profile.capability_mask) == 0 &&
               (it->second.required_access & ~profile.access_mask) == 0;
    };
    if (!traversable(start) || !traversable(goal)) return std::nullopt;
    using QueueEntry = std::pair<std::uint64_t, NavNodeId>;
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, std::greater<>> queue;
    std::map<NavNodeId, std::uint64_t> distance;
    std::map<NavNodeId, NavNodeId> previous;
    distance[start] = 0;
    queue.emplace(0, start);
    while (!queue.empty()) {
        const auto [cost, node] = queue.top();
        queue.pop();
        if (distance[node] != cost) continue;
        if (node == goal) break;
        for (const auto& edge : edges_) {
            if (!edge.enabled || edge.from != node || !traversable(edge.to)) continue;
            const auto candidate = cost + edge.cost;
            const auto found = distance.find(edge.to);
            if (found == distance.end() || candidate < found->second ||
                (candidate == found->second && node < previous[edge.to])) {
                distance[edge.to] = candidate;
                previous[edge.to] = node;
                queue.emplace(candidate, edge.to);
            }
        }
    }
    if (!distance.contains(goal)) return std::nullopt;
    std::vector<NavNodeId> path{goal};
    while (path.back() != start) path.push_back(previous.at(path.back()));
    std::ranges::reverse(path);
    return path;
}

PathResult AsyncPathService::execute(const NavigationSpace& navigation, PathRequest request) const {
    PathResult result{request};
    if (request.navigation_revision != navigation.revision()) return result;
    const auto path = navigation.find_path(request.start, request.goal, request.profile);
    if (path) {
        result.path = *path;
        result.reachable = true;
    }
    return result;
}

bool AsyncPathService::fresh(const PathResult& result, std::uint64_t navigation_revision,
                             std::uint64_t actor_generation, std::uint64_t latest_sequence) const noexcept {
    return result.request.navigation_revision == navigation_revision &&
           result.request.actor_generation == actor_generation && result.request.sequence == latest_sequence;
}

bool KnowledgeStore::observe(KnowledgeFact fact) {
    if (!fact.id || fact.expires_tick < fact.observed_tick) return false;
    const auto it = facts_.find(fact.id);
    if (it != facts_.end() && it->second.observed_tick > fact.observed_tick) return false;
    facts_[fact.id] = fact;
    return true;
}

void KnowledgeStore::expire(std::uint64_t tick) {
    std::erase_if(facts_, [tick](const auto& entry) { return entry.second.expires_tick < tick; });
}

std::optional<KnowledgeFact> KnowledgeStore::fact(FactId id) const {
    const auto it = facts_.find(id);
    return it == facts_.end() ? std::nullopt : std::optional{it->second};
}

std::uint64_t KnowledgeStore::digest() const noexcept {
    std::uint64_t value = 1469598103934665603ULL;
    for (const auto& [id, fact] : facts_) {
        for (const auto part : {id.value(), fact.subject.value(), fact.observed_tick, fact.expires_tick,
                                static_cast<std::uint64_t>(fact.confidence)}) {
            value ^= part;
            value *= 1099511628211ULL;
        }
    }
    return value;
}

bool AIRuntime::add_actor(ActorState actor) {
    return actor.id && actors_.emplace(actor.id, actor).second;
}

bool AIRuntime::add_task(Task task) {
    return task.id && tasks_.emplace(task.id, task).second;
}

bool AIRuntime::assign_task(ActorId actor_id, TaskId task_id) {
    const auto actor_it = actors_.find(actor_id);
    if (actor_it == actors_.end() || !tasks_.contains(task_id)) return false;
    actor_it->second.task = task_id;
    return true;
}

bool AIRuntime::set_robot_command(ActorId actor_id, NavNodeId destination, RulesOfEngagement roe) {
    auto it = actors_.find(actor_id);
    if (it == actors_.end() || it->second.kind != ActorKind::Robot) return false;
    const TaskId task_id{0x8000000000000000ULL | actor_id.value()};
    tasks_[task_id] = Task{task_id, destination, 255, false};
    it->second.task = task_id;
    it->second.roe = roe;
    return true;
}

bool AIRuntime::form_squad(SquadId squad, const std::vector<ActorId>& members) {
    if (!squad || members.empty()) return false;
    std::set<ActorId> validated;
    for (const auto member : members) {
        if (!actors_.contains(member)) return false;
        validated.insert(member);
    }
    squads_[squad] = validated;
    for (const auto member : validated) actors_.at(member).squad = squad;
    return true;
}

void AIRuntime::observe(ActorId observer, KnowledgeFact fact) {
    if (!actors_.contains(observer)) return;
    knowledge_.observe(fact);
}

void AIRuntime::advance(const NavigationSpace& navigation, std::uint64_t ticks, SimulationMode mode,
                        std::uint32_t worker_partitions) {
    (void)mode;
    (void)worker_partitions;
    for (std::uint64_t step = 0; step < ticks; ++step) {
        ++tick_;
        knowledge_.expire(tick_);
        for (auto& [id, state] : actors_) {
            (void)id;
            if (state.kind == ActorKind::Enemy && knowledge_.digest() != 1469598103934665603ULL) {
                state.tactical = TacticalState::Engage;
            }
            if (!state.task) continue;
            auto task_it = tasks_.find(*state.task);
            if (task_it == tasks_.end() || task_it->second.complete) continue;
            const auto path = navigation.find_path(state.location, task_it->second.destination, state.profile);
            if (!path || path->empty()) continue;
            if (path->size() > 1) state.location = (*path)[1];
            if (state.location == task_it->second.destination) task_it->second.complete = true;
        }
    }
}

const ActorState* AIRuntime::actor(ActorId id) const {
    const auto it = actors_.find(id);
    return it == actors_.end() ? nullptr : &it->second;
}

std::uint64_t AIRuntime::authoritative_digest() const noexcept {
    std::uint64_t value = knowledge_.digest() ^ tick_;
    for (const auto& [id, state] : actors_) {
        value = (value * 1099511628211ULL) ^ id.value();
        value = (value * 1099511628211ULL) ^ state.location.value();
        value = (value * 1099511628211ULL) ^ static_cast<std::uint64_t>(state.tactical);
        value = (value * 1099511628211ULL) ^ static_cast<std::uint64_t>(state.roe);
        value = (value * 1099511628211ULL) ^ (state.task ? state.task->value() : 0ULL);
    }
    return value;
}

}  // namespace starforge::ai
