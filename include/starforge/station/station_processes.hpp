#pragma once

#include "starforge/station/station.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <set>

namespace starforge::station {

using TraversalEdgeId = core::StrongId<struct TraversalEdgeIdTag>;
using ControlLinkId = core::StrongId<struct ControlLinkIdTag>;
using FarmPlotId = core::StrongId<struct FarmPlotIdTag>;

struct TraversalEdge {
    TraversalEdgeId id{};
    StationModuleId a{};
    StationModuleId b{};
    bool passable{true};
};

struct ControlLink {
    ControlLinkId id{};
    StationModuleId a{};
    StationModuleId b{};
    bool online{true};
};

class StationConnectivityStore {
public:
    bool add_module(StationModuleId module, bool is_command_core = false);
    bool add_traversal_edge(TraversalEdge edge);
    bool set_traversal_passable(TraversalEdgeId edge, bool passable);
    bool add_control_link(ControlLink link);
    bool set_control_online(ControlLinkId link, bool online);

    [[nodiscard]] bool traversal_reachable(StationModuleId from, StationModuleId to) const;
    [[nodiscard]] bool centrally_controlled(StationModuleId module) const;
    [[nodiscard]] std::uint64_t traversal_revision() const noexcept { return traversal_revision_; }
    [[nodiscard]] std::uint64_t control_revision() const noexcept { return control_revision_; }

private:
    [[nodiscard]] bool reachable(StationModuleId from, StationModuleId to, bool control) const;

    std::set<StationModuleId> modules_;
    std::optional<StationModuleId> command_core_;
    std::map<TraversalEdgeId, TraversalEdge> traversal_edges_;
    std::map<ControlLinkId, ControlLink> control_links_;
    std::uint64_t traversal_revision_{0};
    std::uint64_t control_revision_{0};
};

struct FarmPlot {
    FarmPlotId id{};
    StorageId output_storage{};
    ResourceId crop_resource{};
    std::int64_t growth_required{0};
    std::int64_t growth{0};
    std::int64_t fresh_water_ml{0};
    std::int64_t nutrient_units{0};
    std::int64_t water_per_tick_ml{0};
    std::int64_t nutrients_per_tick{0};
    std::int64_t harvest_quantity{0};
    bool powered{true};
    bool harvest_ready{false};
    bool harvested{false};
};

class FarmingStore {
public:
    bool add_plot(FarmPlot plot);
    bool set_powered(FarmPlotId id, bool powered);
    bool supply(FarmPlotId id, std::int64_t fresh_water_ml, std::int64_t nutrient_units);
    bool advance(FarmPlotId id, std::int64_t ticks);
    bool harvest(FarmPlotId id, LogisticsStore& logistics);
    [[nodiscard]] const FarmPlot* find(FarmPlotId id) const;

private:
    std::map<FarmPlotId, FarmPlot> plots_;
};

}  // namespace starforge::station
