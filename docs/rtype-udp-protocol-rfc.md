# RFC: UDP Protocol for Multiplayer Game Communication

## Table of Contents

- [1. Introduction](#1-introduction)
- [2. Objectives](#2-objectives)
- [3. Package Structure](#3-package-structure)
- [4. UDP Command](#4-udp-command)
  - [4.1 Command Descriptions](#41-command-descriptions)
    - [4.1.1 NEW_ENTITY_STATIC](#411-new_entity_static)
    - [4.1.2 NEW_ENTITY_PLAYER](#412-new_entity_player)
    - [4.1.3 NEW_ENTITY_MISSILE](#413-new_entity_missile)
    - [4.1.4 NEW_ENTITY_MISSILE_BALL](#414-new_entity_missile_ball)
    - [4.1.5 NEW_ENTITY_BYDOS_WAVE](#415-new_entity_bydos_wave)
    - [4.1.6 NEW_ENTITY_ROBOT_GROUND](#416-new_entity_robot_ground)
    - [4.1.7 MOVE_ENTITY](#417-move_entity)
    - [4.1.8 MOD_ENTITY](#418-mod_entity)
    - [4.1.9 MOD_ENTITIES](#419-mod_entities)
    - [4.1.10 DEL_ENTITY](#4110-del_entity)
- [5. Package Integrity](#5-package-integrity)
- [6. Packet Loss Management](#6-packet-loss-management)
- [7. Usage Scenarios](#7-usage-scenarios)
  - [7.1 Creating a Player Entity](#71-creating-a-player-entity)
  - [7.2 Updating an Entities Position](#72-updating-an-entities-position)
  - [7.3 Deleting an Entity](#73-deleting-an-entity)
- [8. Conclusion](#8-conclusion)

## 1. Introduction

This RFC describes the UDP protocol for real-time communications in a multiplayer game.
The protocol is optimized for an environment where speed and low latency are crucial. UDP is chosen for its simplicity and low overhead, allowing for rapid transmission of game data between clients and the server, even at the expense of strict reliability.

## 2. Objectives

The main objectives of the protocol are:

- To provide a fast, low-latency communication channel between clients and the server.
- To efficiently synchronize the states of game entities among multiple clients.
- To manage real-time events such as movements, creation, and deletion of entities.
- To tolerate packet loss without causing critical malfunctions in the game.

## 3. Package structure

The UDP packets used in this protocol follow a fixed structure in the header, with a variable body depending on the type of command.

### 3.1 En-tête des paquets

| Field            | Size    | Description                                                     |
| ---------------- | ------- | --------------------------------------------------------------- |
| `magic`          | 8 bytes | Magic value to validate the integrity of the packet.           |
| `size`           | 8 bytes | Total size of the packet, including the header and body.       |
| `cmd`            | 1 byte  | Specifies the type of command (see section 4).                 |
| `sharedEntityId` | 4 bytes | Unique identifier of the entity or client associated with the message. |

This header is essential for identifying the nature of the packet, the size of the transported data, and the specific command associated with it.

### 3.2 Packet Body

The packet body contains data specific to the command defined in the `cmd` field. The structure of the data in the body varies depending on the type of command and can include information about game entities, such as their positions, velocities, or other attributes.

## 4. UDP Command (`UDPCommand`)

Packets contain a `cmd` field that specifies the command being sent. The available command types in the UDP protocol for this game are listed below:

| Command                  | Value  | Description                                               |
| ------------------------ | ------ | --------------------------------------------------------- |
| `NONE`                   | 0x00   | No operation, empty command, or placeholder.              |
| `NEW_ENTITY_STATIC`       | 0x01   | Create a new static entity.                               |
| `NEW_ENTITY_PLAYER`       | 0x02   | Create a new player entity.                               |
| `NEW_ENTITY_MISSILE`      | 0x03   | Create a missile entity.                                  |
| `NEW_ENTITY_MISSILE_BALL` | 0x04   | Create a missile entity in the form of a ball.            |
| `NEW_ENTITY_BYDOS_WAVE`   | 0x05   | Create a Bydos wave entity.                               |
| `NEW_ENTITY_ROBOT_GROUND` | 0x06   | Create a ground robot entity.                             |
| `MOVE_ENTITY`             | 0x07   | Update an entity's position and velocity.                 |
| `MOD_ENTITY`              | 0x08   | Modify an existing entity's attributes.                   |
| `MOD_ENTITIES`            | 0x09   | Modify multiple entities in one operation.                |
| `DEL_ENTITY`              | 0x0A   | Delete an existing entity.                                |

### 4.1 Command Descriptions

#### 4.1.1 `NEW_ENTITY_STATIC`

Creates a new static entity in the game, without movement, with the specified initial position and velocity attributes.

```cpp
struct NEW_ENTITY_STATIC {
    MOVE_ENTITY moveData;
};
```

#### 4.1.2 `NEW_ENTITY_PLAYER`

Creates a player with a unique identifier, a player name, and movement data.

```cpp
struct NEW_ENTITY_PLAYER {
    std::size_t playerId;
    std::size_t playerIndex;
    char playerName[MAX_USER_NAME_SIZE + 1];
    MOVE_ENTITY moveData;
};
```

#### 4.1.3 `NEW_ENTITY_MISSILE`

Creates a new missile entity with movement data.

```cpp
struct NEW_ENTITY_MISSILE {
    MOVE_ENTITY moveData;
};
```

#### 4.1.4 `NEW_ENTITY_MISSILE_BALL`

Creates a new missile entity in the shape of a ball with movement data.

```cpp
struct NEW_ENTITY_MISSILE_BALL {
    MOVE_ENTITY moveData;
};
```

#### 4.1.5 `NEW_ENTITY_BYDOS_WAVE`

Creates a new Bydos wave entity with movement data.

```cpp
struct NEW_ENTITY_BYDOS_WAVE {
    MOVE_ENTITY moveData;
};
```

#### 4.1.6 `NEW_ENTITY_ROBOT_GROUND`

Creates a new ground robot entity with movement data.

```cpp
struct NEW_ENTITY_ROBOT_GROUND {
    MOVE_ENTITY moveData;
};
```

#### 4.1.7 `MOVE_ENTITY`

Updates the position and speed of an existing entity.

```cpp
struct MOVE_ENTITY {
    ecs::component::Position pos;
    ecs::component::Velocity vel;
};
```

#### 4.1.8 `MOD_ENTITY`

Modifies one or more attributes of an existing entity in the game. This command is used when an entity's characteristics need to be updated without recreating the entity.
Modifications can include updates to position, speed, health, or other entity-specific attributes.

```cpp
struct MOD_ENTITY {
    shared_entity_t sharedEntityId;  // Unique ID of the entity to modify
    ecs::component::Position newPos; // (Optional) New position of the entity
    ecs::component::Velocity newVel; // (Optional) New velocity of the entity
    // Other attributes to modify can be added here
};
```

#### 4.1.9 `MOD_ENTITIES`

Modifies the attributes of multiple entities in a single operation.
This command is useful for synchronizing group updates of entities, such as simultaneously updating the positions and velocities of several moving objects in the game.

```cpp
struct MOD_ENTITIES {
    std::size_t entityCount;                 // Number of entities to modify
    std::vector<shared_entity_t> entityIds;  // List of IDs of entities to modify
    std::vector<ecs::component::Position> newPositions;  // (Optional) New positions of the entities
    std::vector<ecs::component::Velocity> newVelocities; // (Optional) New velocities of the entities
    // Other specific attributes may be included here
};
```

#### 4.1.10 `DEL_ENTITY`

Deletes a specific entity from the game.
When this command is sent, the entity identified by its unique ID is removed from the game state.
No additional information other than the ID of the entity to be deleted is required.

```cpp
struct DEL_ENTITY {
    shared_entity_t sharedEntityId;  // Unique ID of the entity to delete
};
```

### 5. Package Integrity

All UDP packets can be verified via a crc32 field that must be calculated from the packet's header and body. This allows for the detection of potential errors in data transmission.

### 6. Packet Loss Management

Since UDP does not guarantee packet delivery, the protocol implements mechanisms that are tolerant to data loss. Entity updates are periodically retransmitted, allowing clients to correct their local state in case of packet loss.

### 7. Usage Scenarios

#### 7.1 Creating a Player Entity

The server sends a packet with the NEW_ENTITY_PLAYER command to create a new player entity. The client receives the packet and updates its game state with the new entity.

#### 7.2 Updating an Entities Position

A MOVE_ENTITY packet is sent to update the position and velocity of an entity. The receiving client or server applies the new coordinates and velocity to the affected entity.

#### 7.3 Deleting an Entity

The server sends a DEL_ENTITY command to indicate that an entity has been removed from the game. The client receives the packet and removes the corresponding entity from its game state.

### 8. Conclusion

This UDP protocol for multiplayer games provides an efficient, low-latency solution for synchronizing game states between multiple clients and a central server. It allows for great flexibility in managing game entities while tolerating packet loss.
