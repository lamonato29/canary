/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

class Guild;
using GuildWarVector = std::vector<uint32_t>;

/**
 * @brief Handles database operations for guilds.
 *
 * This class is responsible for loading and saving guild data, resolving guild IDs,
 * and retrieving guild war information.
 */
class IOGuild {
public:
	/**
	 * @brief Loads a guild from the database.
	 *
	 * @param guildId The ID of the guild to load.
	 * @return A shared pointer to the loaded Guild, or nullptr if not found.
	 */
	static std::shared_ptr<Guild> loadGuild(uint32_t guildId);

	/**
	 * @brief Saves a guild to the database.
	 *
	 * @param guild The guild to save.
	 */
	static void saveGuild(const std::shared_ptr<Guild> &guild);

	/**
	 * @brief Gets the guild ID by name.
	 *
	 * @param name The name of the guild.
	 * @return The ID of the guild, or 0 if not found.
	 */
	static uint32_t getGuildIdByName(const std::string &name);

	/**
	 * @brief Retrieves the list of active wars for a guild.
	 *
	 * @param guildId The ID of the guild.
	 * @param guildWarVector Vector to store the war IDs.
	 */
	static void getWarList(uint32_t guildId, GuildWarVector &guildWarVector);
};
