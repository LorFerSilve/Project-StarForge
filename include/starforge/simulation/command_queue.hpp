#pragma once

#include "starforge/simulation/contracts.hpp"

#include <algorithm>
#include <vector>

namespace starforge::simulation {

template <typename Command>
class DeterministicCommandQueue final {
public:
    void push(Command command) { commands_.push_back(std::move(command)); }

    [[nodiscard]] std::vector<Command> drain_ordered() {
        std::stable_sort(commands_.begin(), commands_.end(), [](const Command& lhs, const Command& rhs) {
            const auto& a = lhs.metadata;
            const auto& b = rhs.metadata;
            if (a.domain_priority != b.domain_priority) {
                return a.domain_priority < b.domain_priority;
            }
            if (a.originating_tick != b.originating_tick) {
                return a.originating_tick < b.originating_tick;
            }
            if (a.command_id != b.command_id) {
                return a.command_id < b.command_id;
            }
            return a.tie_breaker < b.tie_breaker;
        });

        std::vector<Command> ordered;
        ordered.swap(commands_);
        return ordered;
    }

    [[nodiscard]] bool empty() const noexcept { return commands_.empty(); }
    [[nodiscard]] std::size_t size() const noexcept { return commands_.size(); }

private:
    std::vector<Command> commands_;
};

}  // namespace starforge::simulation
