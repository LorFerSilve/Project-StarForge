#include "starforge/progression/finale_readiness.hpp"

#include <algorithm>
#include <array>
#include <utility>

namespace starforge::progression {

namespace {

constexpr std::array<SupportChannel, 4> kSupportChannels{
    SupportChannel::NavigationRoute,
    SupportChannel::EngineeringRecovery,
    SupportChannel::CombatSecurity,
    SupportChannel::AnalysisContainment,
};

void add_if(FinaleReadinessReport& report, bool condition, FinaleReadinessBlocker blocker) {
    if (!condition) {
        report.blockers.push_back(blocker);
    }
}

FinaleReadinessBlocker support_blocker(SupportChannel channel) {
    switch (channel) {
    case SupportChannel::NavigationRoute:
        return FinaleReadinessBlocker::SupportNavigation;
    case SupportChannel::EngineeringRecovery:
        return FinaleReadinessBlocker::SupportEngineering;
    case SupportChannel::CombatSecurity:
        return FinaleReadinessBlocker::SupportCombat;
    case SupportChannel::AnalysisContainment:
        return FinaleReadinessBlocker::SupportAnalysis;
    }
    return FinaleReadinessBlocker::SupportNavigation;
}

bool provider_matches(const SupportProviderSelection& lhs,
                      const SupportProviderSelection& rhs) {
    return lhs.channel == rhs.channel && lhs.kind == rhs.kind &&
           lhs.provider_id == rhs.provider_id;
}

} // namespace

PreparedFinaleReadinessCommit::PreparedFinaleReadinessCommit(
    FinaleReadinessStore& store,
    DepartureReadinessState next_state,
    std::set<std::uint64_t> committed_transactions,
    core::StateRevision expected_revision) noexcept
    : store_(&store),
      next_state_(std::move(next_state)),
      committed_transactions_(std::move(committed_transactions)),
      expected_revision_(expected_revision) {}

transactions::DomainCommitKey PreparedFinaleReadinessCommit::commit_key() const noexcept {
    return {.domain = transactions::DomainCommitOrder::Progression, .stable_ordinal = 10U};
}

core::StateRevision PreparedFinaleReadinessCommit::expected_revision() const noexcept {
    return expected_revision_;
}

core::StateRevision PreparedFinaleReadinessCommit::current_revision() const noexcept {
    return store_->revision_;
}

void PreparedFinaleReadinessCommit::commit() noexcept {
    store_->current_ = std::move(next_state_);
    store_->committed_transactions_ = std::move(committed_transactions_);
    static_cast<void>(store_->revision_.advance());
}

void PreparedFinaleReadinessCommit::publish_committed_events() noexcept {}

bool FinaleReadinessStore::transaction_seen(core::TransactionId transaction_id) const noexcept {
    return !transaction_id.valid() || committed_transactions_.contains(transaction_id.raw());
}

bool FinaleReadinessStore::valid_support_channel(SupportChannel channel) noexcept {
    return std::ranges::find(kSupportChannels, channel) != kSupportChannels.end();
}

bool FinaleReadinessStore::valid_support_set(
    const std::array<SupportProviderSelection, 4>& providers) noexcept {
    std::set<SupportChannel> seen;
    for (const auto& provider : providers) {
        if (!valid_support_channel(provider.channel) || provider.provider_id.empty() ||
            !seen.insert(provider.channel).second) {
            return false;
        }
    }
    return seen.size() == kSupportChannels.size();
}

FinaleReadinessReport FinaleReadinessStore::validate(
    const FinaleReadinessView& view,
    const std::vector<SupportProviderSelection>& support_providers) {
    FinaleReadinessReport report;

    add_if(report, view.campaign_phase == CampaignPhase::VergeExpeditionP4,
           FinaleReadinessBlocker::CampaignPhase);
    add_if(report,
           view.wake_pattern_success && view.second_cascade_crisis_active &&
               view.crownless_route_known && view.mandatory_finale_knowledge_committed,
           FinaleReadinessBlocker::MandatoryStoryState);
    add_if(report, !view.ms_f02_terminal_resolution_committed,
           FinaleReadinessBlocker::FinaleAlreadyResolved);

    add_if(report, view.selected_ship_id != 0, FinaleReadinessBlocker::ShipSelection);
    add_if(report, view.ship_player_controlled, FinaleReadinessBlocker::ShipOwnership);
    add_if(report, view.ship_operational, FinaleReadinessBlocker::ShipOperational);
    add_if(report, view.ship_reach_iv, FinaleReadinessBlocker::ShipReach);
    add_if(report, view.ship_crownless_route_compatible, FinaleReadinessBlocker::ShipRoute);
    add_if(report, view.ship_power_sufficient, FinaleReadinessBlocker::ShipPower);
    add_if(report, view.ship_thermal_sufficient, FinaleReadinessBlocker::ShipThermal);
    add_if(report, view.ship_navigation_and_sensors_operational,
           FinaleReadinessBlocker::ShipNavigationSensors);
    add_if(report, view.ship_life_support_sufficient, FinaleReadinessBlocker::ShipLifeSupport);
    add_if(report, view.ship_cargo_capacity_sufficient,
           FinaleReadinessBlocker::ShipCargoCapacity);
    add_if(report, view.ship_docking_eva_access_operational,
           FinaleReadinessBlocker::ShipDockingEva);
    add_if(report, view.ship_fuel_propellant_sufficient,
           FinaleReadinessBlocker::ShipFuelPropellant);
    add_if(report, view.ship_has_no_mandatory_capability_damage,
           FinaleReadinessBlocker::ShipDamage);

    add_if(report, view.player_environmental_protection,
           FinaleReadinessBlocker::PlayerEnvironmentalProtection);
    add_if(report, view.player_suit_seal_operational,
           FinaleReadinessBlocker::PlayerSuitSeal);
    add_if(report, view.player_life_support_sufficient,
           FinaleReadinessBlocker::PlayerLifeSupport);
    add_if(report, view.player_engineering_capability,
           FinaleReadinessBlocker::PlayerEngineering);
    add_if(report, view.player_anomaly_scanner_capability,
           FinaleReadinessBlocker::PlayerScanner);
    add_if(report, view.player_combat_capability,
           FinaleReadinessBlocker::PlayerCombat);
    add_if(report, view.player_inventory_headroom,
           FinaleReadinessBlocker::PlayerInventoryHeadroom);

    add_if(report, view.horizon_command_operational,
           FinaleReadinessBlocker::HorizonCommand);
    add_if(report, view.horizon_long_range_communications,
           FinaleReadinessBlocker::HorizonCommunications);
    add_if(report, view.horizon_analysis_data_access,
           FinaleReadinessBlocker::HorizonAnalysis);
    add_if(report, view.horizon_departure_service_interface,
           FinaleReadinessBlocker::HorizonDepartureInterface);
    add_if(report, view.horizon_strategic_persistence,
           FinaleReadinessBlocker::HorizonPersistence);

    add_if(report, view.selected_robot_squad_id != 0,
           FinaleReadinessBlocker::RobotSelection);
    add_if(report, view.base_field_tcc >= 6U, FinaleReadinessBlocker::RobotFieldTcc);
    add_if(report, view.tactical_squad_command_load >= 4U,
           FinaleReadinessBlocker::RobotSquadCommandLoad);
    add_if(report, view.robot_combat_role_present,
           FinaleReadinessBlocker::RobotCombatRole);
    add_if(report, view.robot_utility_role_present,
           FinaleReadinessBlocker::RobotUtilityRole);
    add_if(report, view.robot_deployment_valid,
           FinaleReadinessBlocker::RobotDeployment);

    for (const auto channel : kSupportChannels) {
        const auto count = std::ranges::count_if(
            support_providers,
            [channel](const SupportProviderSelection& provider) {
                return provider.channel == channel;
            });
        if (count != 1) {
            report.blockers.push_back(support_blocker(channel));
            continue;
        }

        const auto provider = std::ranges::find_if(
            support_providers,
            [channel](const SupportProviderSelection& candidate) {
                return candidate.channel == channel;
            });
        const bool syntactically_valid =
            provider != support_providers.end() && !provider->provider_id.empty();
        const bool semantically_valid =
            syntactically_valid &&
            ((provider->kind == SupportProviderKind::ExternalCommitment &&
              provider->committed_and_valid) ||
             (provider->kind == SupportProviderKind::SelfSufficient &&
              view.self_sufficient_support.contains(channel)));
        if (!semantically_valid) {
            report.blockers.push_back(support_blocker(channel));
        }
    }

    return report;
}

core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>
FinaleReadinessStore::prepare_commit(
    const FinaleReadinessView& view,
    std::vector<SupportProviderSelection> support_providers,
    const std::vector<station::Reservation>& manifest_reservations,
    core::TransactionId transaction_id) {
    if (!transaction_id.valid()) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::InvalidTransaction);
    }
    if (transaction_seen(transaction_id)) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::DuplicateTransaction);
    }
    if (current_.has_value()) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::AlreadyCommitted);
    }
    if (!revision_.can_advance()) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::RevisionExhausted);
    }

    const auto report = validate(view, support_providers);
    if (!report.ready()) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::NotReady);
    }

    if (support_providers.size() != kSupportChannels.size()) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::NotReady);
    }

    std::ranges::sort(support_providers, {}, &SupportProviderSelection::channel);
    std::array<SupportProviderSelection, 4> locked_support{
        support_providers[0],
        support_providers[1],
        support_providers[2],
        support_providers[3],
    };
    if (!valid_support_set(locked_support)) {
        return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::failure(
            FinaleReadinessError::NotReady);
    }

    std::vector<station::ReservationId> reservation_ids;
    reservation_ids.reserve(manifest_reservations.size());
    for (const auto& reservation : manifest_reservations) {
        reservation_ids.push_back(reservation.id);
    }

    DepartureReadinessState next{
        .transaction_id = transaction_id.raw(),
        .selected_ship_id = view.selected_ship_id,
        .selected_robot_squad_id = view.selected_robot_squad_id,
        .support_providers = std::move(locked_support),
        .manifest_reservations = std::move(reservation_ids),
        .status = FinaleReadinessStatus::Deployable,
        .schema_version = 1U,
    };

    auto transactions = committed_transactions_;
    transactions.insert(transaction_id.raw());

    return core::Result<PreparedFinaleReadinessCommit, FinaleReadinessError>::success(
        PreparedFinaleReadinessCommit{
            *this, std::move(next), std::move(transactions), revision_});
}

FinaleReadinessReport FinaleReadinessStore::refresh_status(
    const FinaleReadinessView& view,
    const std::vector<SupportProviderSelection>& support_providers,
    const station::LogisticsStore& logistics) {
    auto report = validate(view, support_providers);
    if (!current_.has_value()) {
        report.blockers.push_back(FinaleReadinessBlocker::LockedSelectionChanged);
        return report;
    }

    const auto& locked = *current_;
    if (view.selected_ship_id != locked.selected_ship_id ||
        view.selected_robot_squad_id != locked.selected_robot_squad_id ||
        support_providers.size() != locked.support_providers.size()) {
        report.blockers.push_back(FinaleReadinessBlocker::LockedSelectionChanged);
    } else {
        for (const auto& provider : locked.support_providers) {
            const auto match = std::ranges::find_if(
                support_providers,
                [&provider](const SupportProviderSelection& candidate) {
                    return provider_matches(candidate, provider);
                });
            if (match == support_providers.end()) {
                report.blockers.push_back(FinaleReadinessBlocker::LockedSelectionChanged);
                break;
            }
        }
    }

    for (const auto reservation_id : locked.manifest_reservations) {
        if (!logistics.has_reservation(reservation_id)) {
            report.blockers.push_back(FinaleReadinessBlocker::ManifestReservationLost);
            break;
        }
    }

    const auto next_status =
        report.ready() ? FinaleReadinessStatus::Deployable : FinaleReadinessStatus::Invalidated;
    if (current_->status != next_status && revision_.can_advance()) {
        current_->status = next_status;
        static_cast<void>(revision_.advance());
    }
    return report;
}

FinaleReadinessSnapshot FinaleReadinessStore::snapshot() const {
    return FinaleReadinessSnapshot{
        .current = current_,
        .committed_transactions = committed_transactions_,
        .revision = revision_.raw(),
    };
}

core::Result<FinaleReadinessStore, FinaleReadinessError> FinaleReadinessStore::restore(
    FinaleReadinessSnapshot snapshot) {
    if (snapshot.committed_transactions.contains(0)) {
        return core::Result<FinaleReadinessStore, FinaleReadinessError>::failure(
            FinaleReadinessError::InvalidSnapshot);
    }

    if (snapshot.current.has_value()) {
        const auto& current = *snapshot.current;
        if (current.transaction_id == 0 || current.selected_ship_id == 0 ||
            current.selected_robot_squad_id == 0 || current.schema_version != 1U ||
            !snapshot.committed_transactions.contains(current.transaction_id) ||
            !valid_support_set(current.support_providers)) {
            return core::Result<FinaleReadinessStore, FinaleReadinessError>::failure(
                FinaleReadinessError::InvalidSnapshot);
        }

        std::set<station::ReservationId> unique;
        for (const auto id : current.manifest_reservations) {
            if (!id || !unique.insert(id).second) {
                return core::Result<FinaleReadinessStore, FinaleReadinessError>::failure(
                    FinaleReadinessError::InvalidSnapshot);
            }
        }
    }

    FinaleReadinessStore restored;
    restored.current_ = std::move(snapshot.current);
    restored.committed_transactions_ = std::move(snapshot.committed_transactions);
    restored.revision_ = core::StateRevision{snapshot.revision};
    return core::Result<FinaleReadinessStore, FinaleReadinessError>::success(
        std::move(restored));
}

core::Result<PreparedFinaleDeparture, FinaleReadinessError>
prepare_finale_departure(
    FinaleReadinessStore& readiness,
    station::LogisticsStore& logistics,
    const FinaleReadinessView& view,
    std::vector<SupportProviderSelection> support_providers,
    std::vector<station::Reservation> manifest_reservations,
    core::TransactionId transaction_id) {
    const auto report = FinaleReadinessStore::validate(view, support_providers);
    if (!report.ready()) {
        return core::Result<PreparedFinaleDeparture, FinaleReadinessError>::failure(
            FinaleReadinessError::NotReady);
    }

    auto prepared_manifest =
        station::PreparedLogisticsReservation::prepare(logistics, manifest_reservations);
    if (!prepared_manifest) {
        return core::Result<PreparedFinaleDeparture, FinaleReadinessError>::failure(
            FinaleReadinessError::ManifestReservationFailed);
    }

    auto prepared_readiness = readiness.prepare_commit(
        view, std::move(support_providers), manifest_reservations, transaction_id);
    if (!prepared_readiness) {
        return core::Result<PreparedFinaleDeparture, FinaleReadinessError>::failure(
            prepared_readiness.error());
    }

    return core::Result<PreparedFinaleDeparture, FinaleReadinessError>::success(
        PreparedFinaleDeparture{
            .manifest = std::move(prepared_manifest).value(),
            .readiness = std::move(prepared_readiness).value(),
        });
}

} // namespace starforge::progression
