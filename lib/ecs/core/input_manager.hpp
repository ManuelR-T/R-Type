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

class input_manager {
    public:
    void update(const sf::Event &event)
    {
        if (event.type == sf::Event::KeyPressed) {
            _key_state[event.key.code] = true;
        } else if (event.type == sf::Event::KeyReleased) {
            _key_state[event.key.code] = false;
        }
    }

    bool is_key_pressed(sf::Keyboard::Key key) const
    {
        if (_key_state.contains(key)) {
            return _key_state.at(key);
        }
        return false;
    }

    private:
    std::unordered_map<sf::Keyboard::Key, bool> _key_state;
};

} // namespace ecs
