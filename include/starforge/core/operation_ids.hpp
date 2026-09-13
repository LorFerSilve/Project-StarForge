#pragma once

#include "starforge/core/strong_id.hpp"

namespace starforge::core {

struct CommandIdTag final {};
struct TransactionIdTag final {};
struct JobIdTag final {};
struct EventSequenceTag final {};
struct ReadModelGenerationTag final {};

using CommandId = StrongId<CommandIdTag>;
using TransactionId = StrongId<TransactionIdTag>;
using JobId = StrongId<JobIdTag>;
using EventSequence = StrongId<EventSequenceTag>;
using ReadModelGeneration = StrongId<ReadModelGenerationTag>;

}  // namespace starforge::core
