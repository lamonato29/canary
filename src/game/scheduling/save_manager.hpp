/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "lib/thread/thread_pool.hpp"

class KVStore;
class Logger;
class Game;
class Player;
class Guild;

/**
 * @brief Manages saving of game state.
 *
 * This class handles the periodic saving of players, guilds, the map,
 * and the key-value store (KVStore). It uses a thread pool to perform
 * save operations asynchronously where possible.
 */
class SaveManager {
public:
	explicit SaveManager(ThreadPool &threadPool, KVStore &kvStore, Logger &logger, Game &game);

	SaveManager(const SaveManager &) = delete;
	void operator=(const SaveManager &) = delete;

	/**
	 * @brief Gets the singleton instance of SaveManager.
	 *
	 * @return Reference to the SaveManager instance.
	 */
	static SaveManager &getInstance();

	/**
	 * @brief Triggers a global save of all game data.
	 */
	void saveAll();

	/**
	 * @brief Schedules a global save operation.
	 */
	void scheduleAll();

	/**
	 * @brief Saves a specific player.
	 *
	 * @param player The player to save.
	 * @return True if the save operation was initiated/successful, false otherwise.
	 */
	bool savePlayer(std::shared_ptr<Player> player);

	/**
	 * @brief Saves a specific guild.
	 *
	 * @param guild The guild to save.
	 */
	void saveGuild(std::shared_ptr<Guild> guild);

private:
	void saveMap();
	void saveKV();

	void schedulePlayer(std::weak_ptr<Player> player);
	bool doSavePlayer(std::shared_ptr<Player> player);

	std::atomic<std::chrono::steady_clock::time_point> m_scheduledAt;
	phmap::parallel_flat_hash_map<uint32_t, std::chrono::steady_clock::time_point> m_playerMap;

	ThreadPool &threadPool;
	KVStore &kv;
	Logger &logger;
	Game &game;
};

constexpr auto g_saveManager = SaveManager::getInstance;
