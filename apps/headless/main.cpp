#include "starforge/core/build_contract.hpp"

#include <iostream>

int main() {
    std::cout << "Project StarForge headless bootstrap\n"
              << "implementation_baseline=" << starforge::core::build_contract_name() << '\n'
              << "authoritative_simulation_hz=" << starforge::core::kAuthoritativeSimulationHz << '\n';
    return 0;
}
