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

class InputManager {
    public:
    void update(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed) {
            _keyState[event.key.code] = true;
        } else if (event.type == sf::Event::KeyReleased) {
            _keyState[event.key.code] = false;
        }
    }

    bool isKeyPressed(sf::Keyboard::Key key) const
    {
        if (_keyState.contains(key)) {
            return _keyState.at(key);
        }
        return false;
    }

    private:
    std::unordered_map<sf::Keyboard::Key, bool> _keyState;
};

} // namespace ecs
