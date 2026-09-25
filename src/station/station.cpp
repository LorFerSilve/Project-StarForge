#include "starforge/station/station.hpp"

#include <algorithm>
#include <deque>
#include <limits>
#include <set>
#include <tuple>

namespace starforge::station {

namespace {

bool positive(std::int64_t value) { return value > 0; }

}  // namespace

bool StationTopologyStore::add_module(StationModule module) {
    if (!module.id || modules_.contains(module.id)) {
        return false;
    }
    if (module.is_command_core && command_core().has_value()) {
        return false;
    }
    modules_.emplace(module.id, module);
    ++revision_;
    return true;
}

bool StationTopologyStore::add_structural_edge(StructuralEdge edge) {
    if (!edge.id || !edge.a || !edge.b || edge.a == edge.b || edges_.contains(edge.id) ||
        !modules_.contains(edge.a) || !modules_.contains(edge.b)) {
        return false;
    }
    edges_.emplace(edge.id, edge);
    ++revision_;
    return true;
}

bool StationTopologyStore::set_edge_intact(StructuralEdgeId edge, bool intact) {
    const auto it = edges_.find(edge);
    if (it == edges_.end() || it->second.intact == intact) {
        return false;
    }
    it->second.intact = intact;
    ++revision_;
    return true;
}

std::optional<StationModuleId> StationTopologyStore::command_core() const {
    for (const auto& [id, module] : modules_) {
        if (module.is_command_core) {
            return id;
        }
    }
    return std::nullopt;
}

bool StationTopologyStore::structurally_reachable(StationModuleId module) const {
    const auto root = command_core();
    if (!root || !modules_.contains(module)) {
        return false;
    }
    std::set<StationModuleId> visited;
    std::deque<StationModuleId> pending;
    pending.push_back(*root);
    visited.insert(*root);
    while (!pending.empty()) {
        const auto current = pending.front();
        pending.pop_front();
        if (current == module) {
            return true;
        }
        for (const auto& [_, edge] : edges_) {
            if (!edge.intact) {
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
    return false;
}

bool StationPowerStore::add_producer(PowerProducer producer) {
    if (!producer.id || producer.capacity_w < 0 || producers_.contains(producer.id) ||
        consumers_.contains(producer.id) || storages_.contains(producer.id)) {
        return false;
    }
    producers_.emplace(producer.id, producer);
    return true;
}

bool StationPowerStore::add_consumer(PowerConsumer consumer) {
    if (!consumer.id || consumer.demand_w < 0 || producers_.contains(consumer.id) ||
        consumers_.contains(consumer.id) || storages_.contains(consumer.id)) {
        return false;
    }
    consumers_.emplace(consumer.id, consumer);
    return true;
}

bool StationPowerStore::add_storage(PowerStorage storage) {
    if (!storage.id || storage.energy_j < 0 || storage.capacity_j < storage.energy_j ||
        storage.max_discharge_w < 0 || producers_.contains(storage.id) ||
        consumers_.contains(storage.id) || storages_.contains(storage.id)) {
        return false;
    }
    storages_.emplace(storage.id, storage);
    return true;
}

PowerAllocationSnapshot StationPowerStore::solve_one_second() {
    PowerAllocationSnapshot result;
    for (const auto& [_, producer] : producers_) {
        if (producer.enabled) {
            result.produced_w += producer.capacity_w;
        }
    }

    std::int64_t available = result.produced_w;
    std::vector<std::reference_wrapper<const PowerConsumer>> ordered;
    for (const auto& [_, consumer] : consumers_) {
        if (consumer.enabled) {
            ordered.emplace_back(std::cref(consumer));
        }
    }
    std::sort(ordered.begin(), ordered.end(), [](const auto& lhs, const auto& rhs) {
        const auto& a = lhs.get();
        const auto& b = rhs.get();
        return std::tie(a.priority, a.id) < std::tie(b.priority, b.id);
    });

    std::int64_t total_demand = 0;
    for (const auto& ref : ordered) {
        total_demand += ref.get().demand_w;
    }
    std::int64_t deficit = std::max<std::int64_t>(0, total_demand - available);
    for (auto& [_, storage] : storages_) {
        if (deficit == 0) {
            break;
        }
        const auto discharge = std::min({deficit, storage.max_discharge_w, storage.energy_j});
        storage.energy_j -= discharge;
        result.storage_discharge_w += discharge;
        available += discharge;
        deficit -= discharge;
    }

    for (const auto& ref : ordered) {
        const auto& consumer = ref.get();
        const auto allocation = std::min(available, consumer.demand_w);
        result.consumer_allocation_w.emplace(consumer.id, allocation);
        result.allocated_w += allocation;
        available -= allocation;
    }
    return result;
}

bool AtmosphereStore::add_cell(AtmosphereCell cell) {
    if (!cell.id || cell.oxygen_mmol < 0 || cell.inert_mmol < 0 || cells_.contains(cell.id)) {
        return false;
    }
    cells_.emplace(cell.id, cell);
    return true;
}

bool AtmosphereStore::transfer(PressureCellId from, PressureCellId to, std::int64_t oxygen_mmol,
                               std::int64_t inert_mmol) {
    if (from == to || oxygen_mmol < 0 || inert_mmol < 0) {
        return false;
    }
    const auto source = cells_.find(from);
    const auto destination = cells_.find(to);
    if (source == cells_.end() || destination == cells_.end() ||
        source->second.oxygen_mmol < oxygen_mmol || source->second.inert_mmol < inert_mmol) {
        return false;
    }
    source->second.oxygen_mmol -= oxygen_mmol;
    source->second.inert_mmol -= inert_mmol;
    destination->second.oxygen_mmol += oxygen_mmol;
    destination->second.inert_mmol += inert_mmol;
    return true;
}

bool AtmosphereStore::vent_to_vacuum(PressureCellId from, std::int64_t oxygen_mmol,
                                     std::int64_t inert_mmol) {
    if (oxygen_mmol < 0 || inert_mmol < 0) {
        return false;
    }
    const auto source = cells_.find(from);
    if (source == cells_.end() || source->second.oxygen_mmol < oxygen_mmol ||
        source->second.inert_mmol < inert_mmol) {
        return false;
    }
    source->second.oxygen_mmol -= oxygen_mmol;
    source->second.inert_mmol -= inert_mmol;
    vented_mmol_ += oxygen_mmol + inert_mmol;
    return true;
}

std::int64_t AtmosphereStore::total_gas_mmol() const noexcept {
    std::int64_t result = 0;
    for (const auto& [_, cell] : cells_) {
        result += cell.oxygen_mmol + cell.inert_mmol;
    }
    return result;
}

bool EnvironmentalStore::add_node(EnvironmentalNode node) {
    if (!node.module || node.thermal_energy_j < 0 || node.clean_water_ml < 0 ||
        node.waste_water_ml < 0 || nodes_.contains(node.module)) {
        return false;
    }
    nodes_.emplace(node.module, node);
    return true;
}

bool EnvironmentalStore::transfer_water(StationModuleId from, StationModuleId to,
                                        std::int64_t clean_ml, std::int64_t waste_ml) {
    if (from == to || clean_ml < 0 || waste_ml < 0) {
        return false;
    }
    const auto source = nodes_.find(from);
    const auto destination = nodes_.find(to);
    if (source == nodes_.end() || destination == nodes_.end() ||
        source->second.clean_water_ml < clean_ml || source->second.waste_water_ml < waste_ml) {
        return false;
    }
    source->second.clean_water_ml -= clean_ml;
    source->second.waste_water_ml -= waste_ml;
    destination->second.clean_water_ml += clean_ml;
    destination->second.waste_water_ml += waste_ml;
    return true;
}

bool EnvironmentalStore::transfer_heat(StationModuleId from, StationModuleId to,
                                       std::int64_t energy_j) {
    if (from == to || energy_j < 0) {
        return false;
    }
    const auto source = nodes_.find(from);
    const auto destination = nodes_.find(to);
    if (source == nodes_.end() || destination == nodes_.end() ||
        source->second.thermal_energy_j < energy_j) {
        return false;
    }
    source->second.thermal_energy_j -= energy_j;
    destination->second.thermal_energy_j += energy_j;
    return true;
}

std::int64_t EnvironmentalStore::total_water_ml() const noexcept {
    std::int64_t result = 0;
    for (const auto& [_, node] : nodes_) {
        result += node.clean_water_ml + node.waste_water_ml;
    }
    return result;
}

std::int64_t EnvironmentalStore::total_thermal_energy_j() const noexcept {
    std::int64_t result = 0;
    for (const auto& [_, node] : nodes_) {
        result += node.thermal_energy_j;
    }
    return result;
}

bool LogisticsStore::add_storage(StorageId storage) {
    if (!storage || inventory_.contains(storage) || !revision_.can_advance()) {
        return false;
    }
    inventory_.emplace(storage, std::map<ResourceId, std::int64_t>{});
    static_cast<void>(revision_.advance());
    return true;
}

bool LogisticsStore::deposit(StorageId storage, ResourceId resource, std::int64_t quantity_value) {
    if (!resource || !positive(quantity_value) || !revision_.can_advance()) {
        return false;
    }
    const auto it = inventory_.find(storage);
    if (it == inventory_.end()) {
        return false;
    }
    const auto current_it = it->second.find(resource);
    const auto current = current_it == it->second.end() ? 0 : current_it->second;
    if (quantity_value > std::numeric_limits<std::int64_t>::max() - current) {
        return false;
    }
    it->second[resource] = current + quantity_value;
    static_cast<void>(revision_.advance());
    return true;
}

std::int64_t LogisticsStore::quantity(StorageId storage, ResourceId resource) const {
    const auto store = inventory_.find(storage);
    if (store == inventory_.end()) {
        return 0;
    }
    const auto item = store->second.find(resource);
    return item == store->second.end() ? 0 : item->second;
}

std::int64_t LogisticsStore::reserved(StorageId storage, ResourceId resource) const {
    std::int64_t result = 0;
    for (const auto& [_, reservation] : reservations_) {
        if (reservation.source == storage && reservation.resource == resource) {
            result += reservation.quantity;
        }
    }
    return result;
}

bool LogisticsStore::reserve(Reservation reservation) {
    if (!reservation.id || !reservation.source || !reservation.resource ||
        !positive(reservation.quantity) || reservations_.contains(reservation.id) ||
        !revision_.can_advance() ||
        quantity(reservation.source, reservation.resource) -
                reserved(reservation.source, reservation.resource) <
            reservation.quantity) {
        return false;
    }
    reservations_.emplace(reservation.id, reservation);
    static_cast<void>(revision_.advance());
    return true;
}

bool LogisticsStore::commit_transfer(ReservationId reservation_id, StorageId destination) {
    const auto reservation_it = reservations_.find(reservation_id);
    const auto destination_it = inventory_.find(destination);
    if (reservation_it == reservations_.end() || destination_it == inventory_.end() ||
        !revision_.can_advance()) {
        return false;
    }
    const auto reservation = reservation_it->second;
    auto source_it = inventory_.find(reservation.source);
    if (source_it == inventory_.end()) {
        return false;
    }
    const auto source_resource = source_it->second.find(reservation.resource);
    if (source_resource == source_it->second.end() ||
        source_resource->second < reservation.quantity) {
        return false;
    }
    const auto destination_resource = destination_it->second.find(reservation.resource);
    const auto destination_quantity =
        destination_resource == destination_it->second.end() ? 0 : destination_resource->second;
    if (reservation.quantity >
        std::numeric_limits<std::int64_t>::max() - destination_quantity) {
        return false;
    }

    source_resource->second -= reservation.quantity;
    destination_it->second[reservation.resource] =
        destination_quantity + reservation.quantity;
    reservations_.erase(reservation_it);
    static_cast<void>(revision_.advance());
    return true;
}

bool LogisticsStore::release(ReservationId reservation) {
    if (!revision_.can_advance()) {
        return false;
    }
    const auto erased = reservations_.erase(reservation);
    if (erased != 1U) {
        return false;
    }
    static_cast<void>(revision_.advance());
    return true;
}

bool LogisticsStore::has_reservation(ReservationId reservation) const noexcept {
    return reservation && reservations_.contains(reservation);
}

std::int64_t LogisticsStore::total(ResourceId resource) const {
    std::int64_t result = 0;
    for (const auto& [_, storage] : inventory_) {
        const auto item = storage.find(resource);
        if (item != storage.end()) {
            if (item->second > std::numeric_limits<std::int64_t>::max() - result) {
                return std::numeric_limits<std::int64_t>::max();
            }
            result += item->second;
        }
    }
    return result;
}

bool WorkOrderStore::add(WorkOrder order) {
    if (!order.id || order.work_remaining <= 0 || orders_.contains(order.id)) {
        return false;
    }
    orders_.emplace(order.id, order);
    return true;
}

std::optional<WorkOrderId> WorkOrderStore::next() const {
    const WorkOrder* selected = nullptr;
    for (const auto& [_, order] : orders_) {
        if (order.work_remaining <= 0) {
            continue;
        }
        if (selected == nullptr || std::tie(order.priority, order.id) <
                                       std::tie(selected->priority, selected->id)) {
            selected = &order;
        }
    }
    return selected == nullptr ? std::nullopt : std::optional<WorkOrderId>{selected->id};
}

bool WorkOrderStore::apply_work(WorkOrderId id, std::int64_t amount) {
    const auto it = orders_.find(id);
    if (it == orders_.end() || amount <= 0 || it->second.work_remaining <= 0) {
        return false;
    }
    it->second.work_remaining = std::max<std::int64_t>(0, it->second.work_remaining - amount);
    return true;
}

bool WorkOrderStore::complete(WorkOrderId id) const {
    const auto it = orders_.find(id);
    return it != orders_.end() && it->second.work_remaining == 0;
}

bool ProductionStore::add_job(ProductionJob job, LogisticsStore& logistics) {
    if (!job.id || !job.input_storage || !job.output_storage || !job.input_resource ||
        !job.output_resource || !job.reservation || job.input_quantity <= 0 ||
        job.output_quantity <= 0 || job.work_required <= 0 || jobs_.contains(job.id)) {
        return false;
    }
    if (!logistics.reserve({job.reservation, job.input_storage, job.input_resource, job.input_quantity})) {
        return false;
    }
    jobs_.emplace(job.id, job);
    return true;
}

bool ProductionStore::apply_work(ProductionJobId id, std::int64_t amount, LogisticsStore& logistics) {
    const auto it = jobs_.find(id);
    if (it == jobs_.end() || amount <= 0 || it->second.output_committed) {
        return false;
    }
    auto& job = it->second;
    if (!job.inputs_committed) {
        if (!logistics.commit_transfer(job.reservation, job.output_storage)) {
            return false;
        }
        // Inputs now have exactly one owner at the machine/output storage. Consume them exactly once.
        const auto consume_id = ReservationId{job.reservation.raw() + std::numeric_limits<std::uint64_t>::max() / 2U};
        if (!logistics.reserve({consume_id, job.output_storage, job.input_resource, job.input_quantity})) {
            return false;
        }
        StorageId sink{std::numeric_limits<std::uint64_t>::max()};
        if (!logistics.add_storage(sink) || !logistics.commit_transfer(consume_id, sink)) {
            return false;
        }
        job.inputs_committed = true;
    }
    job.work_done = std::min(job.work_required, job.work_done + amount);
    if (job.work_done == job.work_required && !job.output_committed) {
        if (!logistics.deposit(job.output_storage, job.output_resource, job.output_quantity)) {
            return false;
        }
        job.output_committed = true;
    }
    return true;
}

const ProductionJob* ProductionStore::find(ProductionJobId id) const {
    const auto it = jobs_.find(id);
    return it == jobs_.end() ? nullptr : &it->second;
}

bool ConstructionStore::add_site(ConstructionSite site) {
    if (!site.id || !site.module.id || site.work_required <= 0 || sites_.contains(site.id)) {
        return false;
    }
    sites_.emplace(site.id, site);
    return true;
}

bool ConstructionStore::add_repair(RepairTask task) {
    if (!task.id || !task.edge || task.work_required <= 0 || repairs_.contains(task.id)) {
        return false;
    }
    repairs_.emplace(task.id, task);
    return true;
}

StationGeometryDelta ConstructionStore::apply_work(ConstructionSiteId id, std::int64_t amount,
                                                   StationTopologyStore& topology) {
    StationGeometryDelta delta;
    const auto it = sites_.find(id);
    if (it == sites_.end() || amount <= 0 || it->second.committed) {
        return delta;
    }
    auto& site = it->second;
    site.work_done = std::min(site.work_required, site.work_done + amount);
    if (site.work_done == site.work_required && topology.add_module(site.module)) {
        site.committed = true;
        delta.added_modules.push_back(site.module.id);
    }
    return delta;
}

StationGeometryDelta ConstructionStore::apply_repair_work(RepairTaskId id, std::int64_t amount,
                                                          StationTopologyStore& topology) {
    StationGeometryDelta delta;
    const auto it = repairs_.find(id);
    if (it == repairs_.end() || amount <= 0 || it->second.committed) {
        return delta;
    }
    auto& task = it->second;
    task.work_done = std::min(task.work_required, task.work_done + amount);
    if (task.work_done == task.work_required && topology.set_edge_intact(task.edge, true)) {
        task.committed = true;
        delta.repaired_edges.push_back(task.edge);
    }
    return delta;
}

bool AutomationController::add_rule(AutomationRule rule) {
    if (!rule.id || rules_.contains(rule.id)) {
        return false;
    }
    rules_.emplace(rule.id, rule);
    return true;
}

void AutomationController::evaluate(std::int64_t oxygen_mmol, std::int64_t available_power_w) {
    for (const auto& [_, rule] : rules_) {
        const auto observed = rule.sensor == AutomationSensor::OxygenMmol ? oxygen_mmol : available_power_w;
        if (observed < rule.threshold) {
            alarm_active_ = rule.action == AutomationAction::RaiseAlarm;
        }
    }
}

void HorizonStationSimulation::advance(std::uint64_t ticks, SimulationMode mode) {
    (void)mode;  // Fidelity changes projections, never canonical station rules.
    for (std::uint64_t i = 0; i < ticks; ++i) {
        last_power_ = power.solve_one_second();
        automation.evaluate(atmosphere.total_gas_mmol(),
                            last_power_.produced_w + last_power_.storage_discharge_w);
        ++tick_;
    }
}

StationSnapshot HorizonStationSimulation::snapshot() const {
    return StationSnapshot{tick_,
                           topology.topology_revision(),
                           atmosphere.total_gas_mmol(),
                           atmosphere.vented_gas_mmol(),
                           environment.total_water_ml(),
                           environment.total_thermal_energy_j(),
                           last_power_.allocated_w,
                           automation.alarm_active()};
}

}  // namespace starforge::station
