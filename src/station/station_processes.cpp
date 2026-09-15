#include "starforge/station/station_processes.hpp"

#include <algorithm>
#include <deque>

namespace starforge::station {

bool StationConnectivityStore::add_module(StationModuleId module, bool is_command_core) {
    if (!module || modules_.contains(module) || (is_command_core && command_core_.has_value())) {
        return false;
    }
    modules_.insert(module);
    if (is_command_core) {
        command_core_ = module;
    }
    ++traversal_revision_;
    ++control_revision_;
    return true;
}

bool StationConnectivityStore::add_traversal_edge(TraversalEdge edge) {
    if (!edge.id || !edge.a || !edge.b || edge.a == edge.b || traversal_edges_.contains(edge.id) ||
        !modules_.contains(edge.a) || !modules_.contains(edge.b)) {
        return false;
    }
    traversal_edges_.emplace(edge.id, edge);
    ++traversal_revision_;
    return true;
}

bool StationConnectivityStore::set_traversal_passable(TraversalEdgeId edge, bool passable) {
    const auto it = traversal_edges_.find(edge);
    if (it == traversal_edges_.end() || it->second.passable == passable) {
        return false;
    }
    it->second.passable = passable;
    ++traversal_revision_;
    return true;
}

bool StationConnectivityStore::add_control_link(ControlLink link) {
    if (!link.id || !link.a || !link.b || link.a == link.b || control_links_.contains(link.id) ||
        !modules_.contains(link.a) || !modules_.contains(link.b)) {
        return false;
    }
    control_links_.emplace(link.id, link);
    ++control_revision_;
    return true;
}

bool StationConnectivityStore::set_control_online(ControlLinkId link, bool online) {
    const auto it = control_links_.find(link);
    if (it == control_links_.end() || it->second.online == online) {
        return false;
    }
    it->second.online = online;
    ++control_revision_;
    return true;
}

bool StationConnectivityStore::reachable(StationModuleId from, StationModuleId to, bool control) const {
    if (!modules_.contains(from) || !modules_.contains(to)) {
        return false;
    }

    std::set<StationModuleId> visited{from};
    std::deque<StationModuleId> pending{from};
    while (!pending.empty()) {
        const auto current = pending.front();
        pending.pop_front();
        if (current == to) {
            return true;
        }

        if (control) {
            for (const auto& [_, link] : control_links_) {
                if (!link.online) {
                    continue;
                }
                StationModuleId next{};
                if (link.a == current) {
                    next = link.b;
                } else if (link.b == current) {
                    next = link.a;
                }
                if (next && visited.insert(next).second) {
                    pending.push_back(next);
                }
            }
        } else {
            for (const auto& [_, edge] : traversal_edges_) {
                if (!edge.passable) {
                    continue;
                }
                StationModuleId next{};
                if (edge.a == current) {
                    next = edge.b;
                } else if (edge.b == current) {
                    next = edge.a;
                }
                if (next && visited.insert(next).second) {
                    pending.push_back(next);
                }
            }
        }
    }
    return false;
}

bool StationConnectivityStore::traversal_reachable(StationModuleId from, StationModuleId to) const {
    return reachable(from, to, false);
}

bool StationConnectivityStore::centrally_controlled(StationModuleId module) const {
    return command_core_.has_value() && reachable(*command_core_, module, true);
}

bool FarmingStore::add_plot(FarmPlot plot) {
    if (!plot.id || !plot.output_storage || !plot.crop_resource || plot.growth_required <= 0 ||
        plot.growth < 0 || plot.growth > plot.growth_required || plot.fresh_water_ml < 0 ||
        plot.nutrient_units < 0 || plot.water_per_tick_ml < 0 || plot.nutrients_per_tick < 0 ||
        plot.harvest_quantity <= 0 || plots_.contains(plot.id)) {
        return false;
    }
    plot.harvest_ready = plot.growth == plot.growth_required;
    plot.harvested = false;
    plots_.emplace(plot.id, plot);
    return true;
}

bool FarmingStore::set_powered(FarmPlotId id, bool powered) {
    const auto it = plots_.find(id);
    if (it == plots_.end() || it->second.powered == powered || it->second.harvested) {
        return false;
    }
    it->second.powered = powered;
    return true;
}

bool FarmingStore::supply(FarmPlotId id, std::int64_t fresh_water_ml, std::int64_t nutrient_units) {
    const auto it = plots_.find(id);
    if (it == plots_.end() || fresh_water_ml < 0 || nutrient_units < 0 ||
        (fresh_water_ml == 0 && nutrient_units == 0) || it->second.harvested) {
        return false;
    }
    it->second.fresh_water_ml += fresh_water_ml;
    it->second.nutrient_units += nutrient_units;
    return true;
}

bool FarmingStore::advance(FarmPlotId id, std::int64_t ticks) {
    const auto it = plots_.find(id);
    if (it == plots_.end() || ticks <= 0 || it->second.harvest_ready || it->second.harvested ||
        !it->second.powered) {
        return false;
    }

    auto& plot = it->second;
    const auto remaining_growth = plot.growth_required - plot.growth;
    auto advance_ticks = std::min(ticks, remaining_growth);
    if (plot.water_per_tick_ml > 0) {
        advance_ticks = std::min(advance_ticks, plot.fresh_water_ml / plot.water_per_tick_ml);
    }
    if (plot.nutrients_per_tick > 0) {
        advance_ticks = std::min(advance_ticks, plot.nutrient_units / plot.nutrients_per_tick);
    }
    if (advance_ticks <= 0) {
        return false;
    }

    plot.fresh_water_ml -= advance_ticks * plot.water_per_tick_ml;
    plot.nutrient_units -= advance_ticks * plot.nutrients_per_tick;
    plot.growth += advance_ticks;
    plot.harvest_ready = plot.growth == plot.growth_required;
    return true;
}

bool FarmingStore::harvest(FarmPlotId id, LogisticsStore& logistics) {
    const auto it = plots_.find(id);
    if (it == plots_.end() || !it->second.harvest_ready || it->second.harvested) {
        return false;
    }
    auto& plot = it->second;
    if (!logistics.deposit(plot.output_storage, plot.crop_resource, plot.harvest_quantity)) {
        return false;
    }
    plot.harvested = true;
    return true;
}

const FarmPlot* FarmingStore::find(FarmPlotId id) const {
    const auto it = plots_.find(id);
    return it == plots_.end() ? nullptr : &it->second;
}

}  // namespace starforge::station
