#pragma once

#include <cstdint>
#include <string_view>

namespace starforge::core {

inline constexpr std::string_view kImplementationBaseline{"TA16-V1"};
inline constexpr std::uint32_t kAuthoritativeSimulationHz{60U};
inline constexpr std::uint32_t kRequiredCppStandard{23U};

[[nodiscard]] std::string_view build_contract_name() noexcept;

}  // namespace starforge::core
