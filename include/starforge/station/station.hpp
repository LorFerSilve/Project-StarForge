#pragma once

#include "starforge/core/strong_id.hpp"

#include <cstdint>
#include <map>
#include <optional>
#include <vector>

namespace starforge::station {

using StationModuleId = core::StrongId<struct StationModuleIdTag>;
using StructuralEdgeId = core::StrongId<struct StructuralEdgeIdTag>;
using PressureCellId = core::StrongId<struct PressureCellIdTag>;
using UtilityLinkId = core::StrongId<struct UtilityLinkIdTag>;
using PowerNodeId = core::StrongId<struct PowerNodeIdTag>;
using StorageId = core::StrongId<struct StorageIdTag>;
using ResourceId = core::StrongId<struct ResourceIdTag>;
using ReservationId = core::StrongId<struct ReservationIdTag>;
using WorkOrderId = core::StrongId<struct WorkOrderIdTag>;
using ProductionJobId = core::StrongId<struct ProductionJobIdTag>;
using ConstructionSiteId = core::StrongId<struct ConstructionSiteIdTag>;
using RepairTaskId = core::StrongId<struct RepairTaskIdTag>;
using AutomationRuleId = core::StrongId<struct AutomationRuleIdTag>;

enum class SimulationMode : std::uint8_t { Active, Offscreen };

enum class PowerPriority : std::uint8_t { P0 = 0, P1 = 1, P2 = 2, P3 = 3, P4 = 4 };

struct StationModule {
    StationModuleId id{};
    bool is_command_core{false};
};

struct StructuralEdge {
    StructuralEdgeId id{};
    StationModuleId a{};
    StationModuleId b{};
    bool intact{true};
};

class StationTopologyStore {
public:
    bool add_module(StationModule module);
    bool add_structural_edge(StructuralEdge edge);
    bool set_edge_intact(StructuralEdgeId edge, bool intact);
    [[nodiscard]] bool structurally_reachable(StationModuleId module) const;
    [[nodiscard]] std::uint64_t topology_revision() const noexcept { return revision_; }
    [[nodiscard]] std::size_t module_count() const noexcept { return modules_.size(); }

private:
    [[nodiscard]] std::optional<StationModuleId> command_core() const;

    std::map<StationModuleId, StationModule> modules_;
    std::map<StructuralEdgeId, StructuralEdge> edges_;
    std::uint64_t revision_{0};
};

struct PowerProducer {
    PowerNodeId id{};
    std::int64_t capacity_w{0};
    bool enabled{true};
};

struct PowerConsumer {
    PowerNodeId id{};
    std::int64_t demand_w{0};
    PowerPriority priority{PowerPriority::P4};
    bool enabled{true};
};

struct PowerStorage {
    PowerNodeId id{};
    std::int64_t energy_j{0};
    std::int64_t capacity_j{0};
    std::int64_t max_discharge_w{0};
};

struct PowerAllocationSnapshot {
    std::int64_t produced_w{0};
    std::int64_t storage_discharge_w{0};
    std::int64_t allocated_w{0};
    std::map<PowerNodeId, std::int64_t> consumer_allocation_w;
};

class StationPowerStore {
public:
    bool add_producer(PowerProducer producer);
    bool add_consumer(PowerConsumer consumer);
    bool add_storage(PowerStorage storage);
    [[nodiscard]] PowerAllocationSnapshot solve_one_second();
    [[nodiscard]] const std::map<PowerNodeId, PowerStorage>& storages() const noexcept { return storages_; }

private:
    std::map<PowerNodeId, PowerProducer> producers_;
    std::map<PowerNodeId, PowerConsumer> consumers_;
    std::map<PowerNodeId, PowerStorage> storages_;
};

struct AtmosphereCell {
    PressureCellId id{};
    std::int64_t oxygen_mmol{0};
    std::int64_t inert_mmol{0};
};

class AtmosphereStore {
public:
    bool add_cell(AtmosphereCell cell);
    bool transfer(PressureCellId from, PressureCellId to, std::int64_t oxygen_mmol,
                  std::int64_t inert_mmol);
    bool vent_to_vacuum(PressureCellId from, std::int64_t oxygen_mmol, std::int64_t inert_mmol);
    [[nodiscard]] std::int64_t total_gas_mmol() const noexcept;
    [[nodiscard]] std::int64_t vented_gas_mmol() const noexcept { return vented_mmol_; }
    [[nodiscard]] const std::map<PressureCellId, AtmosphereCell>& cells() const noexcept { return cells_; }

private:
    std::map<PressureCellId, AtmosphereCell> cells_;
    std::int64_t vented_mmol_{0};
};

struct EnvironmentalNode {
    StationModuleId module{};
    std::int64_t thermal_energy_j{0};
    std::int64_t clean_water_ml{0};
    std::int64_t waste_water_ml{0};
};

class EnvironmentalStore {
public:
    bool add_node(EnvironmentalNode node);
    bool transfer_water(StationModuleId from, StationModuleId to, std::int64_t clean_ml,
                        std::int64_t waste_ml);
    bool transfer_heat(StationModuleId from, StationModuleId to, std::int64_t energy_j);
    [[nodiscard]] std::int64_t total_water_ml() const noexcept;
    [[nodiscard]] std::int64_t total_thermal_energy_j() const noexcept;

private:
    std::map<StationModuleId, EnvironmentalNode> nodes_;
};

struct Reservation {
    ReservationId id{};
    StorageId source{};
    ResourceId resource{};
    std::int64_t quantity{0};
};

class LogisticsStore {
public:
    bool add_storage(StorageId storage);
    bool deposit(StorageId storage, ResourceId resource, std::int64_t quantity);
    bool reserve(Reservation reservation);
    bool commit_transfer(ReservationId reservation, StorageId destination);
    bool release(ReservationId reservation);
    [[nodiscard]] std::int64_t quantity(StorageId storage, ResourceId resource) const;
    [[nodiscard]] std::int64_t reserved(StorageId storage, ResourceId resource) const;
    [[nodiscard]] std::int64_t total(ResourceId resource) const;

private:
    std::map<StorageId, std::map<ResourceId, std::int64_t>> inventory_;
    std::map<ReservationId, Reservation> reservations_;
};

struct WorkOrder {
    WorkOrderId id{};
    std::uint8_t priority{0};
    std::int64_t work_remaining{0};
};

class WorkOrderStore {
public:
    bool add(WorkOrder order);
    [[nodiscard]] std::optional<WorkOrderId> next() const;
    bool apply_work(WorkOrderId id, std::int64_t amount);
    [[nodiscard]] bool complete(WorkOrderId id) const;

private:
    std::map<WorkOrderId, WorkOrder> orders_;
};

struct ProductionJob {
    ProductionJobId id{};
    StorageId input_storage{};
    StorageId output_storage{};
    ResourceId input_resource{};
    ResourceId output_resource{};
    std::int64_t input_quantity{0};
    std::int64_t output_quantity{0};
    std::int64_t work_required{0};
    std::int64_t work_done{0};
    ReservationId reservation{};
    bool inputs_committed{false};
    bool output_committed{false};
};

class ProductionStore {
public:
    bool add_job(ProductionJob job, LogisticsStore& logistics);
    bool apply_work(ProductionJobId id, std::int64_t amount, LogisticsStore& logistics);
    [[nodiscard]] const ProductionJob* find(ProductionJobId id) const;

private:
    std::map<ProductionJobId, ProductionJob> jobs_;
};

struct ConstructionSite {
    ConstructionSiteId id{};
    StationModule module{};
    std::int64_t work_required{0};
    std::int64_t work_done{0};
    bool committed{false};
};

struct RepairTask {
    RepairTaskId id{};
    StructuralEdgeId edge{};
    std::int64_t work_required{0};
    std::int64_t work_done{0};
    bool committed{false};
};

struct StationGeometryDelta {
    std::vector<StationModuleId> added_modules;
    std::vector<StructuralEdgeId> repaired_edges;
};

class ConstructionStore {
public:
    bool add_site(ConstructionSite site);
    bool add_repair(RepairTask task);
    StationGeometryDelta apply_work(ConstructionSiteId id, std::int64_t amount,
                                    StationTopologyStore& topology);
    StationGeometryDelta apply_repair_work(RepairTaskId id, std::int64_t amount,
                                           StationTopologyStore& topology);

private:
    std::map<ConstructionSiteId, ConstructionSite> sites_;
    std::map<RepairTaskId, RepairTask> repairs_;
};

enum class AutomationSensor : std::uint8_t { OxygenMmol, AvailablePowerW };
enum class AutomationAction : std::uint8_t { RaiseAlarm, ClearAlarm };

struct AutomationRule {
    AutomationRuleId id{};
    AutomationSensor sensor{AutomationSensor::OxygenMmol};
    std::int64_t threshold{0};
    AutomationAction action{AutomationAction::RaiseAlarm};
};

class AutomationController {
public:
    bool add_rule(AutomationRule rule);
    void evaluate(std::int64_t oxygen_mmol, std::int64_t available_power_w);
    [[nodiscard]] bool alarm_active() const noexcept { return alarm_active_; }

private:
    std::map<AutomationRuleId, AutomationRule> rules_;
    bool alarm_active_{false};
};

struct StationSnapshot {
    std::uint64_t tick{0};
    std::uint64_t topology_revision{0};
    std::int64_t gas_mmol{0};
    std::int64_t vented_mmol{0};
    std::int64_t water_ml{0};
    std::int64_t thermal_energy_j{0};
    std::int64_t allocated_power_w{0};
    bool alarm_active{false};

    friend bool operator==(const StationSnapshot&, const StationSnapshot&) = default;
};

class HorizonStationSimulation {
public:
    StationTopologyStore topology;
    StationPowerStore power;
    AtmosphereStore atmosphere;
    EnvironmentalStore environment;
    LogisticsStore logistics;
    WorkOrderStore work_orders;
    ProductionStore production;
    ConstructionStore construction;
    AutomationController automation;

    void advance(std::uint64_t ticks, SimulationMode mode);
    [[nodiscard]] StationSnapshot snapshot() const;

private:
    std::uint64_t tick_{0};
    PowerAllocationSnapshot last_power_{};
};

}  // namespace starforge::station
