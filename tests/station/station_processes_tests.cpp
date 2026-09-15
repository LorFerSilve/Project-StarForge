#include "starforge/station/station_processes.hpp"

#include <catch2/catch_test_macros.hpp>

using namespace starforge::station;

TEST_CASE("traversal and control connectivity are separate deterministic graphs", "[station][topology][control]") {
    StationConnectivityStore connectivity;
    const StationModuleId core{1};
    const StationModuleId habitat{2};
    const StationModuleId workshop{3};

    REQUIRE(connectivity.add_module(core, true));
    REQUIRE(connectivity.add_module(habitat));
    REQUIRE(connectivity.add_module(workshop));
    REQUIRE(connectivity.add_traversal_edge({TraversalEdgeId{1}, core, habitat, true}));
    REQUIRE(connectivity.add_traversal_edge({TraversalEdgeId{2}, habitat, workshop, true}));
    REQUIRE(connectivity.add_control_link({ControlLinkId{1}, core, workshop, true}));

    CHECK(connectivity.traversal_reachable(core, workshop));
    CHECK(connectivity.centrally_controlled(workshop));

    const auto traversal_revision = connectivity.traversal_revision();
    const auto control_revision = connectivity.control_revision();
    REQUIRE(connectivity.set_traversal_passable(TraversalEdgeId{2}, false));
    CHECK(connectivity.traversal_revision() == traversal_revision + 1);
    CHECK(connectivity.control_revision() == control_revision);
    CHECK_FALSE(connectivity.traversal_reachable(core, workshop));
    CHECK(connectivity.centrally_controlled(workshop));

    REQUIRE(connectivity.set_control_online(ControlLinkId{1}, false));
    CHECK_FALSE(connectivity.centrally_controlled(workshop));
}

TEST_CASE("farming advances only with explicit power water and nutrients and harvests once",
          "[station][farming][conservation]") {
    LogisticsStore logistics;
    REQUIRE(logistics.add_storage(StorageId{10}));

    FarmingStore farming;
    REQUIRE(farming.add_plot({FarmPlotId{1}, StorageId{10}, ResourceId{20}, 5, 0, 4, 2, 1, 1, 3,
                              true, false, false}));

    REQUIRE(farming.advance(FarmPlotId{1}, 10));
    const auto* plot = farming.find(FarmPlotId{1});
    REQUIRE(plot != nullptr);
    CHECK(plot->growth == 2);
    CHECK(plot->fresh_water_ml == 2);
    CHECK(plot->nutrient_units == 0);
    CHECK_FALSE(plot->harvest_ready);

    CHECK_FALSE(farming.advance(FarmPlotId{1}, 1));
    REQUIRE(farming.supply(FarmPlotId{1}, 3, 3));
    REQUIRE(farming.set_powered(FarmPlotId{1}, false));
    CHECK_FALSE(farming.advance(FarmPlotId{1}, 3));
    REQUIRE(farming.set_powered(FarmPlotId{1}, true));
    REQUIRE(farming.advance(FarmPlotId{1}, 3));

    plot = farming.find(FarmPlotId{1});
    REQUIRE(plot != nullptr);
    CHECK(plot->growth == 5);
    CHECK(plot->harvest_ready);

    REQUIRE(farming.harvest(FarmPlotId{1}, logistics));
    CHECK(logistics.quantity(StorageId{10}, ResourceId{20}) == 3);
    CHECK_FALSE(farming.harvest(FarmPlotId{1}, logistics));
    CHECK(logistics.quantity(StorageId{10}, ResourceId{20}) == 3);
}
