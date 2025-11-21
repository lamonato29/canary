/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

struct AccountInfo;

enum class CoinType : uint8_t;
enum class CoinTransactionType : uint8_t;

/**
 * @brief Abstract base class for account repositories.
 *
 * This class defines the interface for loading and saving account data.
 */
class AccountRepository {
public:
	AccountRepository() = default;
	virtual ~AccountRepository() = default;

	// Singleton - ensures we don't accidentally copy it
	AccountRepository(const AccountRepository &) = delete;
	void operator=(const AccountRepository &) = delete;

	/**
	 * @brief Gets the singleton instance of the AccountRepository.
	 *
	 * @return The singleton instance.
	 */
	static AccountRepository &getInstance();

	/**
	 * @brief Loads account information by ID.
	 *
	 * @param id The ID of the account to load.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	virtual bool loadByID(const uint32_t &id, std::unique_ptr<AccountInfo> &acc) = 0;

	/**
	 * @brief Loads account information by email or name.
	 *
	 * @param oldProtocol Whether to use the old protocol (search by name).
	 * @param emailOrName The email or name to search for.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	virtual bool loadByEmailOrName(bool oldProtocol, const std::string &emailOrName, std::unique_ptr<AccountInfo> &acc) = 0;

	/**
	 * @brief Loads account information by session key.
	 *
	 * @param email The session key/email associated with the session.
	 * @param acc unique_ptr to store the loaded account information.
	 * @return True if successful, false otherwise.
	 */
	virtual bool loadBySession(const std::string &email, std::unique_ptr<AccountInfo> &acc) = 0;

	/**
	 * @brief Saves account information.
	 *
	 * @param accInfo unique_ptr containing the account information to save.
	 * @return True if successful, false otherwise.
	 */
	virtual bool save(const std::unique_ptr<AccountInfo> &accInfo) = 0;

	/**
	 * @brief Checks if a character belongs to an account by ID and name.
	 *
	 * @param id The ID of the account.
	 * @param name The name of the character.
	 * @return True if the character belongs to the account, false otherwise.
	 */
	virtual bool getCharacterByAccountIdAndName(const uint32_t &id, const std::string &name) = 0;

	/**
	 * @brief Gets the password for an account.
	 *
	 * @param id The ID of the account.
	 * @param password String to store the retrieved password.
	 * @return True if successful, false otherwise.
	 */
	virtual bool getPassword(const uint32_t &id, std::string &password) = 0;

	/**
	 * @brief Gets the amount of coins of a specific type for an account.
	 *
	 * @param id The ID of the account.
	 * @param coinType The type of coin.
	 * @param coins Reference to store the coin amount.
	 * @return True if successful, false otherwise.
	 */
	virtual bool getCoins(const uint32_t &id, CoinType coinType, uint32_t &coins) = 0;

	/**
	 * @brief Sets the amount of coins of a specific type for an account.
	 *
	 * @param id The ID of the account.
	 * @param coinType The type of coin.
	 * @param amount The amount to set.
	 * @return True if successful, false otherwise.
	 */
	virtual bool setCoins(const uint32_t &id, CoinType coinType, const uint32_t &amount) = 0;

	/**
	 * @brief Registers a coin transaction.
	 *
	 * @param id The ID of the account.
	 * @param type The type of transaction (add/remove).
	 * @param coins The amount of coins involved.
	 * @param coinType The type of coin.
	 * @param description Description of the transaction.
	 * @return True if successful, false otherwise.
	 */
	virtual bool registerCoinsTransaction(
		const uint32_t &id,
		CoinTransactionType type,
		uint32_t coins,
		CoinType coinType,
		const std::string &description
	) = 0;
};

constexpr auto g_accountRepository = AccountRepository::getInstance;
