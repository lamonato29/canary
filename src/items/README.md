# Items

This directory contains definitions and logic for game items.

## Key Components

- `Item`: Represents an individual item instance.
- `ItemType`: Defines the static properties of an item type (loaded from items.xml/protobuf).
- `Items`: Manages the collection of all item types.
- `Thing`: Base class for all objects on the map (items and creatures).
- `Cylinder`: Interface for objects that can hold other objects (containers, players).
