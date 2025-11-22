/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "lua/scripts/luascript.hpp"

/**
 * @brief Manages the loading and execution of Lua scripts.
 */
class Scripts {
public:
	// non-copyable
	Scripts(const Scripts &) = delete;
	Scripts &operator=(const Scripts &) = delete;

	/**
	 * @brief Gets the singleton instance of Scripts.
	 *
	 * @return Reference to the Scripts instance.
	 */
	static Scripts &getInstance();

	/**
	 * @brief Clears all loaded scripts.
	 */
	void clearAllScripts() const;

	/**
	 * @brief Loads event scheduler scripts from a file.
	 *
	 * @param fileName The file name to load.
	 * @return True if successful, false otherwise.
	 */
	bool loadEventSchedulerScripts(const std::string &fileName);

	/**
	 * @brief Loads scripts from a specified folder.
	 *
	 * @param folderName The folder to load scripts from.
	 * @param isLib Whether the scripts are libraries.
	 * @param reload Whether this is a reload operation.
	 * @return True if successful, false otherwise.
	 */
	bool loadScripts(std::string_view folderName, bool isLib, bool reload);

	/**
	 * @brief Gets the Lua script interface.
	 *
	 * @return Reference to the LuaScriptInterface.
	 */
	LuaScriptInterface &getScriptInterface() {
		return scriptInterface;
	}

private:
	LuaScriptInterface scriptInterface;
	Scripts();
};

constexpr auto g_scripts = Scripts::getInstance;
