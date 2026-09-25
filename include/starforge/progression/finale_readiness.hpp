#pragma once

#include "starforge/core/operation_ids.hpp"
#include "starforge/core/result.hpp"
#include "starforge/core/revision.hpp"
#include "starforge/progression/campaign_progression.hpp"
#include "starforge/progression/progression.hpp"
#include "starforge/station/logistics_reservation_transaction.hpp"
#include "starforge/transactions/participant.hpp"

#include <array>
#include <cstdint>
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace starforge::progression {

enum class SupportChannel : std::uint8_t {
    NavigationRoute = 0,
    EngineeringRecovery,
    CombatSecurity,
    AnalysisContainment,
};

enum class SupportProviderKind : std::uint8_t {
    ExternalCommitment,
    SelfSufficient,
};

struct SupportProviderSelection final {
    SupportChannel channel{SupportChannel::NavigationRoute};
    SupportProviderKind kind{SupportProviderKind::ExternalCommitment};
    std::string provider_id{};
    bool committed_and_valid{false};

    friend bool operator==(const SupportProviderSelection&, const SupportProviderSelection&) = default;
};

struct FinaleReadinessView final {
    CampaignPhase campaign_phase{CampaignPhase::RecoveryP0};

    bool wake_pattern_success{false};
    bool second_cascade_crisis_active{false};
    bool crownless_route_known{false};
    bool mandatory_finale_knowledge_committed{false};
    bool ms_f02_terminal_resolution_committed{false};

    std::uint64_t selected_ship_id{0};
    bool ship_player_controlled{false};
    bool ship_operational{false};
    bool ship_reach_iv{false};
    bool ship_crownless_route_compatible{false};
    bool ship_power_sufficient{false};
    bool ship_thermal_sufficient{false};
    bool ship_navigation_and_sensors_operational{false};
    bool ship_life_support_sufficient{false};
    bool ship_cargo_capacity_sufficient{false};
    bool ship_docking_eva_access_operational{false};
    bool ship_fuel_propellant_sufficient{false};
    bool ship_has_no_mandatory_capability_damage{false};

    bool player_environmental_protection{false};
    bool player_suit_seal_operational{false};
    bool player_life_support_sufficient{false};
    bool player_engineering_capability{false};
    bool player_anomaly_scanner_capability{false};
    bool player_combat_capability{false};
    bool player_inventory_headroom{false};

    bool horizon_command_operational{false};
    bool horizon_long_range_communications{false};
    bool horizon_analysis_data_access{false};
    bool horizon_departure_service_interface{false};
    bool horizon_strategic_persistence{false};

    std::uint64_t selected_robot_squad_id{0};
    std::uint32_t base_field_tcc{0};
    std::uint32_t tactical_squad_command_load{0};
    bool robot_combat_role_present{false};
    bool robot_utility_role_present{false};
    bool robot_deployment_valid{false};

    std::set<SupportChannel> self_sufficient_support{};
};

enum class FinaleReadinessBlocker : std::uint8_t {
    CampaignPhase,
    MandatoryStoryState,
    FinaleAlreadyResolved,
    ShipSelection,
    ShipOwnership,
    ShipOperational,
    ShipReach,
    ShipRoute,
    ShipPower,
    ShipThermal,
    ShipNavigationSensors,
    ShipLifeSupport,
    ShipCargoCapacity,
    ShipDockingEva,
    ShipFuelPropellant,
    ShipDamage,
    PlayerEnvironmentalProtection,
    PlayerSuitSeal,
    PlayerLifeSupport,
    PlayerEngineering,
    PlayerScanner,
    PlayerCombat,
    PlayerInventoryHeadroom,
    HorizonCommand,
    HorizonCommunications,
    HorizonAnalysis,
    HorizonDepartureInterface,
    HorizonPersistence,
    RobotSelection,
    RobotFieldTcc,
    RobotSquadCommandLoad,
    RobotCombatRole,
    RobotUtilityRole,
    RobotDeployment,
    SupportNavigation,
    SupportEngineering,
    SupportCombat,
    SupportAnalysis,
    ManifestReservationLost,
    LockedSelectionChanged,
};

struct FinaleReadinessReport final {
    std::vector<FinaleReadinessBlocker> blockers{};

    [[nodiscard]] bool ready() const noexcept { return blockers.empty(); }
};

enum class FinaleReadinessStatus : std::uint8_t {
    Deployable,
    Invalidated,
    Deployed,
};

struct DepartureReadinessState final {
    std::uint64_t transaction_id{0};
    std::uint64_t selected_ship_id{0};
    std::uint64_t selected_robot_squad_id{0};
    std::array<SupportProviderSelection, 4> support_providers{};
    std::vector<station::ReservationId> manifest_reservations{};
    std::uint64_t deployment_transaction_id{0};
    std::uint64_t mission_instance_id{0};
    FinaleReadinessStatus status{FinaleReadinessStatus::Deployable};
    std::uint32_t schema_version{1U};
};

struct FinaleReadinessSnapshot final {
    std::optional<DepartureReadinessState> current{};
    std::set<std::uint64_t> committed_transactions{};
    std::uint64_t revision{0};
};

enum class FinaleReadinessError : std::uint8_t {
    NotReady,
    DuplicateTransaction,
    AlreadyCommitted,
    InvalidTransaction,
    RevisionExhausted,
    ManifestReservationFailed,
    InvalidSnapshot,
};

class FinaleReadinessStore;

class PreparedFinaleDeploymentActivation final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedFinaleDeploymentActivation(PreparedFinaleDeploymentActivation&&) noexcept = default;
    PreparedFinaleDeploymentActivation& operator=(PreparedFinaleDeploymentActivation&&) noexcept = default;
    PreparedFinaleDeploymentActivation(const PreparedFinaleDeploymentActivation&) = delete;
    PreparedFinaleDeploymentActivation& operator=(const PreparedFinaleDeploymentActivation&) = delete;

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    friend class FinaleReadinessStore;

    PreparedFinaleDeploymentActivation(
        FinaleReadinessStore& store,
        DepartureReadinessState next_state,
        std::set<std::uint64_t> committed_transactions,
        core::StateRevision expected_revision) noexcept;

    FinaleReadinessStore* store_{nullptr};
    DepartureReadinessState next_state_{};
    std::set<std::uint64_t> committed_transactions_{};
    core::StateRevision expected_revision_{};
};

class PreparedFinaleReadinessCommit final : public transactions::IPreparedTransactionParticipant {
public:
    PreparedFinaleReadinessCommit(PreparedFinaleReadinessCommit&&) noexcept = default;
    PreparedFinaleReadinessCommit& operator=(PreparedFinaleReadinessCommit&&) noexcept = default;
    PreparedFinaleReadinessCommit(const PreparedFinaleReadinessCommit&) = delete;
    PreparedFinaleReadinessCommit& operator=(const PreparedFinaleReadinessCommit&) = delete;

    [[nodiscard]] transactions::DomainCommitKey commit_key() const noexcept override;
    [[nodiscard]] core::StateRevision expected_revision() const noexcept override;
    [[nodiscard]] core::StateRevision current_revision() const noexcept override;
    void commit() noexcept override;
    void publish_committed_events() noexcept override;

private:
    friend class FinaleReadinessStore;

    PreparedFinaleReadinessCommit(FinaleReadinessStore& store,
                                  DepartureReadinessState next_state,
                                  std::set<std::uint64_t> committed_transactions,
                                  core::StateRevision expected_revision) noexcept;

    FinaleReadinessStore* store_{nullptr};
    DepartureReadinessState next_state_{};
    std::set<std::uint64_t> committed_transactions_{};
    core::StateRevision expected_revision_{};
};

class FinaleReadinessStore final {
public:
    [[nodiscard]] static FinaleReadinessReport validate(
        const FinaleReadinessView& view,
        const std::vector<SupportProviderSelection>& support_providers);

    [[nodiscard]] core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>
    prepare_commit(const FinaleReadinessView& view,
                   std::vector<SupportProviderSelection> support_providers,
                   const std::vector<station::Reservation>& manifest_reservations,
                   core::TransactionId transaction_id);

    [[nodiscard]] FinaleReadinessReport refresh_status(
        const FinaleReadinessView& view,
        const std::vector<SupportProviderSelection>& support_providers,
        const station::LogisticsStore& logistics);

    [[nodiscard]] core::Result<PreparedFinaleDeploymentActivation, FinaleReadinessError>
    prepare_deployment_activation(core::TransactionId transaction_id,
                                  std::uint64_t mission_instance_id);

    [[nodiscard]] const std::optional<DepartureReadinessState>& current() const noexcept {
        return current_;
    }
    [[nodiscard]] core::StateRevision revision() const noexcept { return revision_; }

    [[nodiscard]] FinaleReadinessSnapshot snapshot() const;
    [[nodiscard]] static core::Result<FinaleReadinessStore, FinaleReadinessError> restore(
        FinaleReadinessSnapshot snapshot);

private:
    friend class PreparedFinaleReadinessCommit;
    friend class PreparedFinaleDeploymentActivation;

    [[nodiscard]] bool transaction_seen(core::TransactionId transaction_id) const noexcept;
    [[nodiscard]] static bool valid_support_channel(SupportChannel channel) noexcept;
    [[nodiscard]] static bool valid_support_set(
        const std::array<SupportProviderSelection, 4>& providers) noexcept;

    std::optional<DepartureReadinessState> current_{};
    std::set<std::uint64_t> committed_transactions_{};
    core::StateRevision revision_{};
};

struct PreparedFinaleDeparture final {
    station::PreparedLogisticsReservation manifest;
    PreparedFinaleReadinessCommit readiness;

    [[nodiscard]] std::array<transactions::IPreparedTransactionParticipant*, 2> participants() noexcept {
        return {&manifest, &readiness};
    }
};

[[nodiscard]] core::Result<PreparedFinaleDeparture, FinaleReadinessError>
prepare_finale_departure(FinaleReadinessStore& readiness,
                         station::LogisticsStore& logistics,
                         const FinaleReadinessView& view,
                         std::vector<SupportProviderSelection> support_providers,
                         std::vector<station::Reservation> manifest_reservations,
                         core::TransactionId transaction_id);

} // namespace starforge::progression
