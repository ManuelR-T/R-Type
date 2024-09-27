/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** draw
*/

#include <SFML/Graphics.hpp>
#include "../core/registry.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "core/zipper.hpp"

namespace ecs::systems {

void draw(registry &reg, sf::RenderWindow &window)
{
    auto &sprites = reg.get_components<component::sprite>();

    for (size_t i = 0; i < sprites.size(); ++i) {
        if (sprites.has(i) && sprites[i]) {
            window.draw(sprites[i]->sprite_obj);
        }
    }

    auto &positions = reg.get_components<ecs::component::position>();
    auto &drawables = reg.get_components<ecs::component::drawable>();

    ecs::zipper<ecs::component::position, ecs::component::drawable> zip(positions, drawables);

    for (auto [pos, draw] : zip) {
        draw.shape.setPosition(pos.x, pos.y);
        window.draw(draw.shape);
    }
}

} // namespace ecs::systems
