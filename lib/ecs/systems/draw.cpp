/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** draw
*/

#include <SFML/Graphics.hpp>
#include "../core/registry.hpp"
#include "components/sprite.hpp"

namespace ecs::systems {

void draw(registry &reg, sf::RenderWindow &window)
{
    auto &sprites = reg.get_components<component::sprite>();

    for (size_t i = 0; i < sprites.size(); ++i) {
        if (sprites.has(i) && sprites[i]) {
            window.draw(sprites[i]->sprite_obj);
        }
    }
}

} // namespace ecs::systems
