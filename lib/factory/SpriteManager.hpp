/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** sprite_system
*/

#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace ecs {

/**
 * @brief Manages loading and caching of textures to prevent duplication.
 *
 * The `SpriteManager` class handles the loading of textures from files and stores them
 * in a cache to avoid loading the same texture multiple times. It provides access to
 * textures by their identifiers.
 */
class SpriteManager {
    public:
    /**
     * @brief Retrieves a texture by its identifier, loading it if necessary.
     *
     * If the texture is not already loaded, it loads the texture from the file system
     * and stores it in the cache.
     *
     * @param texture_id The identifier or filename of the texture.
     * @return Reference to the SFML texture associated with the identifier.
     * @throws std::runtime_error If the texture fails to load.
     */
    sf::Texture &getTexture(const std::string &textureId)
    {
        auto it = _textures.find(textureId);
        if (it != _textures.end()) {
            return it->second;
        } else {
            sf::Texture texture;
            if (!texture.loadFromFile(textureId)) {
                throw std::runtime_error("Failed to load texture: " + textureId);
            }
            auto emplaceResult = _textures.emplace(textureId, std::move(texture));
            return emplaceResult.first->second;
        }
    }

    private:
    std::unordered_map<std::string, sf::Texture> _textures; /**< Cache of loaded textures. */
};

} // namespace ecs