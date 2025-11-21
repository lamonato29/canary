/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "config_enums.hpp"

using ConfigValue = std::variant<std::string, int32_t, bool, float>;
using OTCFeatures = std::vector<uint8_t>;

/**
 * @brief Singleton class managing server configuration.
 *
 * This class loads configuration values from `config.lua` and provides methods
 * to access them safely. It supports string, boolean, integer, and float values.
 */
class ConfigManager {
public:
	ConfigManager() = default;

	// Singleton - ensures we don't accidentally copy it
	ConfigManager(const ConfigManager &) = delete;
	void operator=(const ConfigManager &) = delete;

	/**
	 * @brief Get the singleton instance of the ConfigManager.
	 *
	 * @return Reference to the ConfigManager instance.
	 */
	static ConfigManager &getInstance();

	/**
	 * @brief Loads the configuration from the file.
	 *
	 * @return True if the configuration was loaded successfully, false otherwise.
	 */
	bool load();

	/**
	 * @brief Reloads the configuration from the file.
	 *
	 * @return True if the configuration was reloaded successfully, false otherwise.
	 */
	bool reload();

	void missingConfigWarning(const char* identifier);

	/**
	 * @brief Set the name of the Lua configuration file.
	 *
	 * @param what The name of the configuration file.
	 * @return The name of the configuration file.
	 */
	const std::string &setConfigFileLua(const std::string &what) {
		configFileLua = { what };
		return configFileLua;
	};

	/**
	 * @brief Get the name of the Lua configuration file.
	 *
	 * @return The name of the configuration file.
	 */
	[[nodiscard]] const std::string &getConfigFileLua() const {
		return configFileLua;
	};

	/**
	 * @brief Retrieves a string configuration value.
	 *
	 * @param key The configuration key.
	 * @param location The source location of the call (for debugging/logging).
	 * @return The string value of the configuration.
	 */
	[[nodiscard]] const std::string &getString(const ConfigKey_t &key, const std::source_location &location = std::source_location::current()) const;

	/**
	 * @brief Retrieves a number (integer) configuration value.
	 *
	 * @param key The configuration key.
	 * @param location The source location of the call.
	 * @return The integer value of the configuration.
	 */
	[[nodiscard]] int32_t getNumber(const ConfigKey_t &key, const std::source_location &location = std::source_location::current()) const;

	/**
	 * @brief Retrieves a boolean configuration value.
	 *
	 * @param key The configuration key.
	 * @param location The source location of the call.
	 * @return The boolean value of the configuration.
	 */
	[[nodiscard]] bool getBoolean(const ConfigKey_t &key, const std::source_location &location = std::source_location::current()) const;

	/**
	 * @brief Retrieves a float configuration value.
	 *
	 * @param key The configuration key.
	 * @param location The source location of the call.
	 * @return The float value of the configuration.
	 */
	[[nodiscard]] float getFloat(const ConfigKey_t &key, const std::source_location &location = std::source_location::current()) const;

	OTCFeatures getEnabledFeaturesOTC() const;
	OTCFeatures getDisabledFeaturesOTC() const;

private:
	mutable std::unordered_map<ConfigKey_t, std::string> m_configString;
	mutable std::unordered_map<ConfigKey_t, bool> m_configBoolean;
	mutable std::unordered_map<ConfigKey_t, int32_t> m_configInteger;
	mutable std::unordered_map<ConfigKey_t, float> m_configFloat;

	std::unordered_map<ConfigKey_t, ConfigValue> configs;
	std::string loadStringConfig(lua_State* L, const ConfigKey_t &key, const char* identifier, const std::string &defaultValue);
	int32_t loadIntConfig(lua_State* L, const ConfigKey_t &key, const char* identifier, const int32_t &defaultValue);
	bool loadBoolConfig(lua_State* L, const ConfigKey_t &key, const char* identifier, const bool &defaultValue);
	float loadFloatConfig(lua_State* L, const ConfigKey_t &key, const char* identifier, const float &defaultValue);

	std::string configFileLua = { "config.lua" };
	bool loaded = false;
	OTCFeatures enabledFeaturesOTC = {};
	OTCFeatures disabledFeaturesOTC = {};
	void loadLuaOTCFeatures(lua_State* L);
};

constexpr auto g_configManager = ConfigManager::getInstance;
