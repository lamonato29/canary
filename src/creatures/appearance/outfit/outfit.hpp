/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

enum PlayerSex_t : uint8_t;
class Player;

/**
 * @brief Represents an entry for an outfit configuration.
 */
struct OutfitEntry {
	constexpr explicit OutfitEntry(uint16_t initLookType, uint8_t initAddons) :
		lookType(initLookType), addons(initAddons) { }

	uint16_t lookType;
	uint8_t addons;
};

/**
 * @brief Represents an Outfit in the game.
 */
struct Outfit {
	Outfit(std::string initName, uint16_t initLookType, bool initPremium, bool initUnlocked, std::string initFrom) :
		name(std::move(initName)), lookType(initLookType), premium(initPremium), unlocked(initUnlocked), from(std::move(initFrom)) { }

	std::string name;
	uint16_t lookType;
	bool premium;
	bool unlocked;
	std::string from;
};

struct ProtocolOutfit {
	ProtocolOutfit(const std::string &initName, uint16_t initLookType, uint8_t initAddons) :
		name(initName), lookType(initLookType), addons(initAddons) { }

	const std::string &name;
	uint16_t lookType;
	uint8_t addons;
};

/**
 * @brief Manages the collection of all available outfits.
 *
 * Loads outfit definitions from XML and allows retrieval by look type or name.
 */
class Outfits {
public:
	/**
	 * @brief Gets the singleton instance of Outfits.
	 *
	 * @return Reference to the Outfits instance.
	 */
	static Outfits &getInstance();

	/**
	 * @brief Reloads the outfits from the XML configuration.
	 *
	 * @return True if reload was successful, false otherwise.
	 */
	bool reload();

	/**
	 * @brief Loads the outfits from the XML configuration.
	 *
	 * @return True if loading was successful, false otherwise.
	 */
	bool loadFromXml();

	/**
	 * @brief Retrieves an outfit by its look type.
	 *
	 * @param player The player requesting the outfit (for gender/premium checks).
	 * @param lookType The look type ID.
	 * @param isOppositeOutfit Whether to look up the outfit for the opposite sex.
	 * @return A shared pointer to the Outfit, or nullptr if not found/accessible.
	 */
	[[nodiscard]] std::shared_ptr<Outfit> getOutfitByLookType(const std::shared_ptr<const Player> &player, uint16_t lookType, bool isOppositeOutfit = false) const;

	/**
	 * @brief Gets all outfits available for a specific sex.
	 *
	 * @param sex The sex (PLAYERSEX_FEMALE or PLAYERSEX_MALE).
	 * @return A reference to the vector of outfits.
	 */
	[[nodiscard]] const std::vector<std::shared_ptr<Outfit>> &getOutfits(PlayerSex_t sex) const;

	std::shared_ptr<Outfit> getOutfitByName(PlayerSex_t sex, const std::string &name) const;
};
