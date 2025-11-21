/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "database/database.hpp"
#include "declarations.hpp"
#include "lib/di/container.hpp"

/**
 * @brief Handles database operations for the market system.
 *
 * This class manages market offers, history, statistics, and expiration processing.
 */
class IOMarket {
public:
	IOMarket() = default;

	/**
	 * @brief Gets the singleton instance of IOMarket.
	 *
	 * @return Reference to the IOMarket instance.
	 */
	static IOMarket &getInstance() {
		return inject<IOMarket>();
	}

	static MarketOfferList getActiveOffers(MarketAction_t action);
	static MarketOfferList getActiveOffers(MarketAction_t action, uint16_t itemId, uint8_t tier);
	static MarketOfferList getOwnOffers(MarketAction_t action, uint32_t playerId);
	static HistoryMarketOfferList getOwnHistory(MarketAction_t action, uint32_t playerId);

	static void processExpiredOffers(const DBResult_ptr &result, bool);
	static void checkExpiredOffers();

	static uint32_t getPlayerOfferCount(uint32_t playerId);
	static MarketOfferEx getOfferByCounter(uint32_t timestamp, uint16_t counter);

	static void createOffer(uint32_t playerId, MarketAction_t action, uint32_t itemId, uint16_t amount, uint64_t price, uint8_t tier, bool anonymous);
	static void acceptOffer(uint32_t offerId, uint16_t amount);
	static void deleteOffer(uint32_t offerId);

	static void appendHistory(uint32_t playerId, MarketAction_t type, uint16_t itemId, uint16_t amount, uint64_t price, time_t timestamp, uint8_t tier, MarketOfferState_t state);
	static bool moveOfferToHistory(uint32_t offerId, MarketOfferState_t state);

	void updateStatistics();

	using StatisticsMap = std::map<uint16_t, std::map<uint8_t, MarketStatistics>>;
	const StatisticsMap &getPurchaseStatistics() const {
		return purchaseStatistics;
	}
	const StatisticsMap &getSaleStatistics() const {
		return saleStatistics;
	}

	static uint8_t getTierFromDatabaseTable(const std::string &string);

private:
	// [uint16_t = item id, [uint8_t = item tier, MarketStatistics = structure of the statistics]]
	StatisticsMap purchaseStatistics;
	StatisticsMap saleStatistics;
};
