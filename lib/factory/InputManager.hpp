/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** input_manager
*/

#pragma once

#include <SFML/Window/Event.hpp>
#include <unordered_map>

namespace ecs {

/**
 * @class InputManager
 * @brief Manages input events and tracks the state of keyboard keys.
 */
class InputManager {
    public:
    /**
     * @brief Updates the state of the keys based on the received event.
     *
     * @param event The event to process.
     */
    void update(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed) {
            _keyState[event.key.code] = true;
        } else if (event.type == sf::Event::KeyReleased) {
            _keyState[event.key.code] = false;
        }
    }

    /**
     * @brief Checks if a specific key is currently pressed.
     *
     * @param key The key to check.
     * @return true if the key is pressed, false otherwise.
     */
    bool isKeyPressed(sf::Keyboard::Key key) const
    {
        if (_keyState.contains(key)) {
            return _keyState.at(key);
        }
        return false;
    }

    private:
    /**
     * @brief Stores the state of each key (pressed or not).
     */
    std::unordered_map<sf::Keyboard::Key, bool> _keyState;
};

} // namespace ecs
