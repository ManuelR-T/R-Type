/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** EntityFactory
*/

#pragma once

#include <climits>
#include <limits>
#include <nlohmann/json.hpp>
#include <string>
#include "Registry.hpp"
#include "SpriteManager.hpp"
#include "entity.hpp"
#include "imgui.h"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"
#include <imgui-SFML.h>

namespace ecs {

/**
 * @class EntityFactory
 * @brief A factory class for creating entities from JSON files.
 */
class EntityFactory {
    public:
    /**
     * @brief Creates a client entity from a JSON file.
     *
     * @param reg Reference to the registry.
     * @param spriteManager Reference to the sprite manager.
     * @param udpClient Reference to the UDP client.
     * @param jsonFilePath Path to the JSON file.
     * @param x X-coordinate of the entity (default is max int).
     * @param y Y-coordinate of the entity (default is max int).
     * @param sharedEntity Shared entity identifier (default is max size_t).
     * @param vx X-velocity of the entity (default is max float).
     * @param vy Y-velocity of the entity (default is max float).
     * @return The created entity.
     */
    static entity_t createClientEntityFromJSON(
        Registry &reg,
        SpriteManager &spriteManager,
        ntw::UDPClient &udpClient,
        const std::string &jsonFilePath,
        int x = std::numeric_limits<int>::max(),
        int y = std::numeric_limits<int>::max(),
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max(),
        float vx = std::numeric_limits<float>::max(),
        float vy = std::numeric_limits<float>::max(),
        std::shared_ptr<ImFont> font = nullptr
    );

    /**
     * @brief Creates a server entity from a JSON file.
     *
     * @param reg Reference to the registry.
     * @param jsonFilePath Path to the JSON file.
     * @param x X-coordinate of the entity (default is max int).
     * @param y Y-coordinate of the entity (default is max int).
     * @param sharedEntity Shared entity identifier (default is max size_t).
     * @param vx X-velocity of the entity (default is max float).
     * @param vy Y-velocity of the entity (default is max float).
     * @return The created entity.
     */
    static entity_t createServerEntityFromJSON(
        Registry &reg,
        const std::string &jsonFilePath,
        int x = std::numeric_limits<int>::max(),
        int y = std::numeric_limits<int>::max(),
        shared_entity_t sharedEntity = std::numeric_limits<size_t>::max(),
        float vx = std::numeric_limits<float>::max(),
        float vy = std::numeric_limits<float>::max()
    );

    /**
     * @brief Adds common components to an entity.
     *
     * @param reg Reference to the registry.
     * @param entity The entity to which components will be added.
     * @param componentsJson JSON object containing the components.
     * @param x X-coordinate of the entity.
     * @param y Y-coordinate of the entity.
     * @param vx X-velocity of the entity.
     * @param vy Y-velocity of the entity.
     */
    static void addCommonComponents(
        Registry &reg,
        entity_t entity,
        const nlohmann::json &componentsJson,
        int x,
        int y,
        float vx,
        float vy,
        std::shared_ptr<ImFont> font = nullptr
    );
};

} // namespace ecs
