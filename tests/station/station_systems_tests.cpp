#include "starforge/station/station.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::station;

TEST_CASE("station topology revisions and disconnected branches remain explicit", "[station][topology]") {
    StationTopologyStore topology;
    const StationModuleId core{1};
    const StationModuleId habitat{2};
    const StationModuleId workshop{3};

    REQUIRE(topology.add_module({core, true}));
    REQUIRE(topology.add_module({habitat, false}));
    REQUIRE(topology.add_module({workshop, false}));
    REQUIRE(topology.add_structural_edge({StructuralEdgeId{1}, core, habitat, true}));
    REQUIRE(topology.add_structural_edge({StructuralEdgeId{2}, habitat, workshop, true}));
    CHECK(topology.structurally_reachable(workshop));
    const auto before_sever = topology.topology_revision();

    REQUIRE(topology.set_edge_intact(StructuralEdgeId{1}, false));
    CHECK(topology.topology_revision() == before_sever + 1);
    CHECK_FALSE(topology.structurally_reachable(habitat));
    CHECK_FALSE(topology.structurally_reachable(workshop));
    CHECK(topology.module_count() == 3);
}

TEST_CASE("power allocation is deterministic by priority and conserves stored energy", "[station][power]") {
    StationPowerStore power;
    REQUIRE(power.add_producer({PowerNodeId{1}, 100, true}));
    REQUIRE(power.add_storage({PowerNodeId{2}, 50, 50, 30}));
    REQUIRE(power.add_consumer({PowerNodeId{10}, 80, PowerPriority::P0, true}));
    REQUIRE(power.add_consumer({PowerNodeId{11}, 70, PowerPriority::P3, true}));

    const auto first = power.solve_one_second();
    CHECK(first.produced_w == 100);
    CHECK(first.storage_discharge_w == 30);
    CHECK(first.allocated_w == 130);
    CHECK(first.consumer_allocation_w.at(PowerNodeId{10}) == 80);
    CHECK(first.consumer_allocation_w.at(PowerNodeId{11}) == 50);
    CHECK(power.storages().at(PowerNodeId{2}).energy_j == 20);

    const auto second = power.solve_one_second();
    CHECK(second.storage_discharge_w == 20);
    CHECK(second.allocated_w == 120);
    CHECK(power.storages().at(PowerNodeId{2}).energy_j == 0);
}

TEST_CASE("atmosphere water and thermal transfers conserve canonical quantities", "[station][conservation]") {
    AtmosphereStore atmosphere;
    REQUIRE(atmosphere.add_cell({PressureCellId{1}, 100, 400}));
    REQUIRE(atmosphere.add_cell({PressureCellId{2}, 50, 200}));
    const auto initial_gas = atmosphere.total_gas_mmol();
    REQUIRE(atmosphere.transfer(PressureCellId{1}, PressureCellId{2}, 20, 80));
    CHECK(atmosphere.total_gas_mmol() == initial_gas);
    REQUIRE(atmosphere.vent_to_vacuum(PressureCellId{2}, 10, 40));
    CHECK(atmosphere.total_gas_mmol() + atmosphere.vented_gas_mmol() == initial_gas);

    EnvironmentalStore environment;
    REQUIRE(environment.add_node({StationModuleId{1}, 1'000, 500, 100}));
    REQUIRE(environment.add_node({StationModuleId{2}, 500, 100, 50}));
    const auto initial_water = environment.total_water_ml();
    const auto initial_heat = environment.total_thermal_energy_j();
    REQUIRE(environment.transfer_water(StationModuleId{1}, StationModuleId{2}, 150, 25));
    REQUIRE(environment.transfer_heat(StationModuleId{1}, StationModuleId{2}, 250));
    CHECK(environment.total_water_ml() == initial_water);
    CHECK(environment.total_thermal_energy_j() == initial_heat);
}

TEST_CASE("logistics reservations prevent double spend and transfers preserve ownership", "[station][logistics]") {
    LogisticsStore logistics;
    const StorageId source{1};
    const StorageId destination{2};
    const ResourceId ore{1};
    REQUIRE(logistics.add_storage(source));
    REQUIRE(logistics.add_storage(destination));
    REQUIRE(logistics.deposit(source, ore, 100));
    REQUIRE(logistics.reserve({ReservationId{1}, source, ore, 70}));
    CHECK_FALSE(logistics.reserve({ReservationId{2}, source, ore, 40}));
    const auto before = logistics.total(ore);
    REQUIRE(logistics.commit_transfer(ReservationId{1}, destination));
    CHECK(logistics.quantity(source, ore) == 30);
    CHECK(logistics.quantity(destination, ore) == 70);
    CHECK(logistics.total(ore) == before);
}

TEST_CASE("work orders are stable and production commits output exactly once", "[station][work][production]") {
    WorkOrderStore work;
    REQUIRE(work.add({WorkOrderId{2}, 2, 5}));
    REQUIRE(work.add({WorkOrderId{1}, 0, 3}));
    REQUIRE(work.next() == WorkOrderId{1});
    REQUIRE(work.apply_work(WorkOrderId{1}, 3));
    CHECK(work.complete(WorkOrderId{1}));
    REQUIRE(work.next() == WorkOrderId{2});

    LogisticsStore logistics;
    REQUIRE(logistics.add_storage(StorageId{1}));
    REQUIRE(logistics.add_storage(StorageId{2}));
    REQUIRE(logistics.deposit(StorageId{1}, ResourceId{1}, 10));

    ProductionStore production;
    ProductionJob job{ProductionJobId{1}, StorageId{1}, StorageId{2}, ResourceId{1}, ResourceId{2},
                      10, 4, 5, 0, ReservationId{10}, false, false};
    REQUIRE(production.add_job(job, logistics));
    REQUIRE(production.apply_work(ProductionJobId{1}, 2, logistics));
    CHECK_FALSE(production.find(ProductionJobId{1})->output_committed);
    REQUIRE(production.apply_work(ProductionJobId{1}, 3, logistics));
    CHECK(production.find(ProductionJobId{1})->output_committed);
    CHECK(logistics.quantity(StorageId{2}, ResourceId{2}) == 4);
    CHECK_FALSE(production.apply_work(ProductionJobId{1}, 1, logistics));
    CHECK(logistics.quantity(StorageId{2}, ResourceId{2}) == 4);
}

TEST_CASE("construction and repair mutate topology only at committed work boundaries", "[station][construction]") {
    StationTopologyStore topology;
    REQUIRE(topology.add_module({StationModuleId{1}, true}));
    REQUIRE(topology.add_module({StationModuleId{2}, false}));
    REQUIRE(topology.add_structural_edge({StructuralEdgeId{1}, StationModuleId{1}, StationModuleId{2}, false}));

    ConstructionStore construction;
    REQUIRE(construction.add_site({ConstructionSiteId{1}, {StationModuleId{3}, false}, 10, 0, false}));
    CHECK(construction.apply_work(ConstructionSiteId{1}, 9, topology).added_modules.empty());
    const auto completed = construction.apply_work(ConstructionSiteId{1}, 1, topology);
    REQUIRE(completed.added_modules.size() == 1);
    CHECK(completed.added_modules.front() == StationModuleId{3});

    REQUIRE(construction.add_repair({RepairTaskId{1}, StructuralEdgeId{1}, 2, 0, false}));
    CHECK(construction.apply_repair_work(RepairTaskId{1}, 1, topology).repaired_edges.empty());
    const auto repaired = construction.apply_repair_work(RepairTaskId{1}, 1, topology);
    REQUIRE(repaired.repaired_edges.size() == 1);
    CHECK(topology.structurally_reachable(StationModuleId{2}));
}

TEST_CASE("automation and station evolution are equivalent active and offscreen", "[station][determinism]") {
    auto build = [] {
        HorizonStationSimulation station;
        REQUIRE(station.topology.add_module({StationModuleId{1}, true}));
        REQUIRE(station.atmosphere.add_cell({PressureCellId{1}, 20, 80}));
        REQUIRE(station.environment.add_node({StationModuleId{1}, 1'000, 1'000, 0}));
        REQUIRE(station.power.add_producer({PowerNodeId{1}, 100, true}));
        REQUIRE(station.power.add_consumer({PowerNodeId{2}, 75, PowerPriority::P0, true}));
        REQUIRE(station.automation.add_rule(
            {AutomationRuleId{1}, AutomationSensor::AvailablePowerW, 120, AutomationAction::RaiseAlarm}));
        return station;
    };

    auto active = build();
    auto offscreen = build();
    active.advance(10, SimulationMode::Active);
    offscreen.advance(4, SimulationMode::Offscreen);
    offscreen.advance(6, SimulationMode::Offscreen);
    CHECK(active.snapshot() == offscreen.snapshot());
    CHECK(active.snapshot().alarm_active);
}
