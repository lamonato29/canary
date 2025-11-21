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
enum class AccountErrors_t : uint8_t;
enum AccountType : uint8_t;

/**
 * @brief Represents a player account.
 *
 * This class manages the account information, including ID, coins, premium time, and associated players.
 */
class Account {
public:
	/**
	 * @brief Constructs an Account object with a given ID.
	 *
	 * @param id The ID of the account.
	 */
	explicit Account(const uint32_t &id);

	/**
	 * @brief Constructs an Account object with a given descriptor (name or email).
	 *
	 * @param descriptor The descriptor (name or email) of the account.
	 */
	explicit Account(std::string descriptor);

	~Account() = default;

	/** Coins
	 * @brief Get the amount of coins that the account has from database.
	 *
	 * @param type Type of the coin
	 *
	 * @return uint32_t Number of coins
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	[[nodiscard]] std::tuple<uint32_t, AccountErrors_t> getCoins(CoinType type) const;

	/**
	 * @brief Add coins to the account.
	 *
	 * @param type Type of the coin
	 * @param amount Amount of coins to be added
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t addCoins(CoinType type, const uint32_t &amount, const std::string &detail = "ADD Coins");

	/**
	 * @brief Removes coins from the account.
	 *
	 * @param type Type of the coin
	 * @param amount Amount of coins to be removed
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t removeCoins(CoinType type, const uint32_t &amount, const std::string &detail = "REMOVE Coins");

	/**
	 * @brief Registers a coin transaction.
	 *
	 * @param type Type of the coin
	 * @param amount Amount of coins to be added
	 * @param detail Detail of the transaction
	 */
	void registerCoinTransaction(CoinTransactionType transactionType, CoinType type, const uint32_t &amount, const std::string &detail);

	/***************************************************************************
	 * Account Load/Save
	 **************************************************************************/

	/**
	 * @brief Save Account.
	 *
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t save() const;

	/**
	 * @brief Load Account Information.
	 *
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t load();

	/**
	 * @brief Re-Load Account Information to get update information(mainly the
	 * players list).
	 *
	 * @return AccountErrors_t AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t reload();

	/***************************************************************************
	 * Setters and Getters
	 **************************************************************************/

	/**
	 * @brief Gets the account ID.
	 *
	 * @return The account ID.
	 */
	[[nodiscard]] uint32_t getID() const;

	/**
	 * @brief Get the Descriptor object
	 * @warning Descriptors are credentials that may be used to login into the account. DO NOT BPUBLISH THIS INFORMATION.
	 *
	 * @return std::string
	 */
	std::string getDescriptor() const;

	/**
	 * @brief Gets the account password.
	 *
	 * @return The account password.
	 */
	std::string getPassword();

	/**
	 * @brief Adds premium days to the account.
	 *
	 * @param days The number of days to add.
	 */
	void addPremiumDays(const int32_t &days);

	/**
	 * @brief Sets the number of premium days.
	 *
	 * @param days The number of days to set.
	 */
	void setPremiumDays(const int32_t &days);

	/**
	 * @brief Gets the remaining premium days.
	 *
	 * @return The number of remaining premium days.
	 */
	[[nodiscard]] uint32_t getPremiumRemainingDays() const;

	/**
	 * @brief Gets the total number of premium days purchased.
	 *
	 * @return The total number of premium days purchased.
	 */
	[[nodiscard]] uint32_t getPremiumDaysPurchased() const;

	/**
	 * @brief Gets the age of the account in days.
	 *
	 * @return The age of the account in days.
	 */
	[[nodiscard]] uint32_t getAccountAgeInDays() const;

	/**
	 * @brief Gets the last day of the premium subscription.
	 *
	 * @return The timestamp of the last premium day.
	 */
	[[nodiscard]] time_t getPremiumLastDay() const;

	/**
	 * @brief Sets the account type.
	 *
	 * @param accountType The type of the account.
	 * @return AccountErrors_t::Ok(0) Success, otherwise Fail.
	 */
	AccountErrors_t setAccountType(AccountType accountType);

	/**
	 * @brief Gets the account type.
	 *
	 * @return The account type.
	 */
	[[nodiscard]] AccountType getAccountType() const;

	/**
	 * @brief Updates the premium time based on current time.
	 */
	void updatePremiumTime();

	/**
	 * @brief Gets the list of players associated with the account.
	 *
	 * @return A tuple containing a map of player names and their IDs, and an error code.
	 */
	std::tuple<phmap::flat_hash_map<std::string, uint64_t>, AccountErrors_t> getAccountPlayers() const;

	/**
	 * @brief Sets the ID of the house bid the account is involved in.
	 *
	 * @param houseId The ID of the house.
	 */
	void setHouseBidId(uint32_t houseId);

	/**
	 * @brief Gets the ID of the house bid the account is involved in.
	 *
	 * @return The ID of the house.
	 */
	uint32_t getHouseBidId() const;

	// Old protocol compat
	/**
	 * @brief Sets whether the account uses old protocol compatibility.
	 *
	 * @param toggle True to enable compatibility, false otherwise.
	 */
	void setProtocolCompat(bool toggle);

	/**
	 * @brief Checks if the account uses old protocol compatibility.
	 *
	 * @return True if compatibility is enabled, false otherwise.
	 */
	bool getProtocolCompat() const;

	/**
	 * @brief Authenticates the account.
	 *
	 * @return True if authentication is successful, false otherwise.
	 */
	bool authenticate();

	/**
	 * @brief Authenticates the account using a secret.
	 *
	 * @param secret The secret to use for authentication.
	 * @return True if authentication is successful, false otherwise.
	 */
	bool authenticate(const std::string &secret);

	/**
	 * @brief Authenticates the account session.
	 *
	 * @return True if the session is valid, false otherwise.
	 */
	bool authenticateSession();

	/**
	 * @brief Authenticates the account using a password.
	 *
	 * @param password The password to verify.
	 * @return True if the password is correct, false otherwise.
	 */
	bool authenticatePassword(const std::string &password);

private:
	std::string m_descriptor;
	std::unique_ptr<AccountInfo> m_account;
	bool m_accLoaded = false;
};
