/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "account/account_repository.hpp"

enum class CoinType : uint8_t;
enum class CoinTransactionType : uint8_t;

/**
 * @brief Database implementation of the AccountRepository.
 *
 * This class handles loading and saving account data using a database connection.
 */
class AccountRepositoryDB final : public AccountRepository {
public:
	AccountRepositoryDB();

	/**
	 * @brief Loads account information by ID from the database.
	 *
	 * @param id The ID of the account to load.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	bool loadByID(const uint32_t &id, std::unique_ptr<AccountInfo> &acc) override;

	/**
	 * @brief Loads account information by email or name from the database.
	 *
	 * @param oldProtocol Whether to use the old protocol (search by name).
	 * @param emailOrName The email or name to search for.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	bool loadByEmailOrName(bool oldProtocol, const std::string &emailOrName, std::unique_ptr<AccountInfo> &acc) override;

	/**
	 * @brief Loads account information by session key from the database.
	 *
	 * @param esseionKey The session key to search for.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	bool loadBySession(const std::string &esseionKey, std::unique_ptr<AccountInfo> &acc) override;

	/**
	 * @brief Saves account information to the database.
	 *
	 * @param accInfo unique_ptr containing the account information to save.
	 * @return True if successful, false otherwise.
	 */
	bool save(const std::unique_ptr<AccountInfo> &accInfo) override;

	/**
	 * @brief Checks if a character belongs to an account by ID and name in the database.
	 *
	 * @param id The ID of the account.
	 * @param name The name of the character.
	 * @return True if the character belongs to the account, false otherwise.
	 */
	bool getCharacterByAccountIdAndName(const uint32_t &id, const std::string &name) override;

	/**
	 * @brief Gets the password for an account from the database.
	 *
	 * @param id The ID of the account.
	 * @param password String to store the retrieved password.
	 * @return True if successful, false otherwise.
	 */
	bool getPassword(const uint32_t &id, std::string &password) override;

	/**
	 * @brief Gets the amount of coins of a specific type for an account from the database.
	 *
	 * @param id The ID of the account.
	 * @param coinType The type of coin.
	 * @param coins Reference to store the coin amount.
	 * @return True if successful, false otherwise.
	 */
	bool getCoins(const uint32_t &id, CoinType coinType, uint32_t &coins) override;

	/**
	 * @brief Sets the amount of coins of a specific type for an account in the database.
	 *
	 * @param id The ID of the account.
	 * @param coinType The type of coin.
	 * @param amount The amount to set.
	 * @return True if successful, false otherwise.
	 */
	bool setCoins(const uint32_t &id, CoinType coinType, const uint32_t &amount) override;

	/**
	 * @brief Registers a coin transaction in the database.
	 *
	 * @param id The ID of the account.
	 * @param type The type of transaction (add/remove).
	 * @param coins The amount of coins involved.
	 * @param coinType The type of coin.
	 * @param description Description of the transaction.
	 * @return True if successful, false otherwise.
	 */
	bool registerCoinsTransaction(
		const uint32_t &id,
		CoinTransactionType type,
		uint32_t coins,
		CoinType coinType,
		const std::string &description
	) override;

private:
	std::unordered_map<CoinType, std::string> coinTypeToColumn {};

	bool load(const std::string &query, std::unique_ptr<AccountInfo> &acc);
	bool loadAccountPlayers(std::unique_ptr<AccountInfo> &acc) const;
	void setupLoyaltyInfo(std::unique_ptr<AccountInfo> &acc);
};
