/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#ifndef USE_PRECOMPILED_HEADERS
	#include <parallel_hashmap/phmap.h>
	#include <cstdint>
#endif

#include "enums/account_type.hpp"

/**
 * @brief Structure holding account information.
 */
struct AccountInfo {
	~AccountInfo() = default;

	uint32_t id = 0; /**< The unique ID of the account. */
	uint32_t premiumRemainingDays = 0; /**< Number of remaining premium days. */
	time_t premiumLastDay = 0; /**< Timestamp of the last premium day. */
	AccountType accountType = ACCOUNT_TYPE_NONE; /**< The type of the account (e.g., normal, gamemaster). */
	phmap::flat_hash_map<std::string, uint64_t> players; /**< Map of player names to their IDs. */
	bool oldProtocol = false; /**< Flag indicating if the account uses the old protocol. */
	time_t sessionExpires = 0; /**< Timestamp when the session expires. */
	uint32_t premiumDaysPurchased = 0; /**< Total premium days purchased. */
	uint32_t creationTime = 0; /**< Timestamp when the account was created. */
	uint32_t houseBidId = 0; /**< ID of the house currently bid on by the account. */
};
