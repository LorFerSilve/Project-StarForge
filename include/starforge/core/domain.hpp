#pragma once

#include <cstdint>

namespace starforge::core {

enum class DomainId : std::uint16_t {
    Core = 1,
    Diagnostics = 2,
    Simulation = 3,
    Transactions = 4,
    ReadModels = 5,
    Content = 10,
    World = 20,
    Player = 30,
    Combat = 31,
    Crew = 32,
    Robots = 33,
    Spacecraft = 34,
    Station = 40,
    Economy = 50,
    Progression = 51,
    Ai = 60,
    Missions = 70,
    Strategic = 80,
};

}  // namespace starforge::core
