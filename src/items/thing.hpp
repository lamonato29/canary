/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "game/movement/position.hpp"

class Tile;
class Cylinder;
class Item;
class Creature;
class Container;
class Player;

/**
 * @brief Base class for all game objects that can be placed on the map (Creatures and Items).
 */
class Thing {
public:
	constexpr Thing() = default;
	virtual ~Thing() = default;

	// non-copyable
	Thing(const Thing &) = delete;
	Thing &operator=(const Thing &) = delete;

	/**
	 * @brief Get the description of the thing.
	 *
	 * @param lookDistance The distance from which the thing is being looked at.
	 * @return A string containing the description.
	 */
	virtual std::string getDescription(int32_t lookDistance) = 0;

	/**
	 * @brief Get the parent cylinder of this thing.
	 *
	 * @return A shared pointer to the parent Cylinder, or nullptr if it has no parent.
	 */
	virtual std::shared_ptr<Cylinder> getParent() {
		return nullptr;
	}

	/**
	 * @brief Get the real parent cylinder of this thing.
	 * This might differ from getParent() in cases where the immediate parent is a temporary or virtual container.
	 *
	 * @return A shared pointer to the real parent Cylinder.
	 */
	virtual std::shared_ptr<Cylinder> getRealParent() {
		return getParent();
	}

	/**
	 * @brief Set the parent of this thing.
	 *
	 * @param cylinder A weak pointer to the new parent Cylinder.
	 */
	virtual void setParent(std::weak_ptr<Cylinder>) {
		//
	}

	/**
	 * @brief Get the tile where this thing is located.
	 *
	 * @return A shared pointer to the Tile, or nullptr if it's not on a tile.
	 */
	virtual std::shared_ptr<Tile> getTile() {
		return nullptr;
	}

	/**
	 * @brief Get the tile where this thing is located (const version).
	 *
	 * @return A shared pointer to the Tile, or nullptr if it's not on a tile.
	 */
	virtual std::shared_ptr<Tile> getTile() const {
		return nullptr;
	}

	/**
	 * @brief Get the position of this thing.
	 *
	 * @return The position of the thing.
	 */
	virtual const Position &getPosition();

	/**
	 * @brief Get the throw range of this thing.
	 *
	 * @return The throw range in tiles.
	 */
	virtual int32_t getThrowRange() const = 0;

	/**
	 * @brief Check if the thing can be pushed.
	 *
	 * @return True if it can be pushed, false otherwise.
	 */
	virtual bool isPushable() = 0;

	/**
	 * @brief Cast to Player if this thing is a player.
	 *
	 * @return A shared pointer to Player, or nullptr.
	 */
	virtual std::shared_ptr<Player> getPlayer() {
		return nullptr;
	}

	/**
	 * @brief Cast to Container if this thing is a container.
	 *
	 * @return A shared pointer to Container, or nullptr.
	 */
	virtual std::shared_ptr<Container> getContainer() {
		return nullptr;
	}

	/**
	 * @brief Cast to Container if this thing is a container (const version).
	 *
	 * @return A shared pointer to Container, or nullptr.
	 */
	virtual std::shared_ptr<const Container> getContainer() const {
		return nullptr;
	}

	/**
	 * @brief Cast to Item if this thing is an item.
	 *
	 * @return A shared pointer to Item, or nullptr.
	 */
	virtual std::shared_ptr<Item> getItem() {
		return nullptr;
	}

	/**
	 * @brief Cast to Item if this thing is an item (const version).
	 *
	 * @return A shared pointer to Item, or nullptr.
	 */
	virtual std::shared_ptr<const Item> getItem() const {
		return nullptr;
	}

	/**
	 * @brief Cast to Creature if this thing is a creature.
	 *
	 * @return A shared pointer to Creature, or nullptr.
	 */
	virtual std::shared_ptr<Creature> getCreature() {
		return nullptr;
	}

	/**
	 * @brief Cast to Creature if this thing is a creature (const version).
	 *
	 * @return A shared pointer to Creature, or nullptr.
	 */
	virtual std::shared_ptr<const Creature> getCreature() const {
		return nullptr;
	}

	/**
	 * @brief Cast to Cylinder if this thing is a cylinder (container/creature that can hold items).
	 *
	 * @return A shared pointer to Cylinder, or nullptr.
	 */
	virtual std::shared_ptr<Cylinder> getCylinder() {
		return nullptr;
	}

	/**
	 * @brief Check if the thing is removed from the game.
	 *
	 * @return True if removed, false otherwise.
	 */
	virtual bool isRemoved() {
		return true;
	}
};
