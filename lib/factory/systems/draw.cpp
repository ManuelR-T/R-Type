/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** draw
*/

#include <SFML/Graphics.hpp>
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"

namespace ecs::systems {

void draw(Registry &reg, sf::RenderWindow &window)
{
    auto &sprites = reg.getComponents<component::Sprite>();

    for (size_t i = 0; i < sprites.size(); ++i) {
        if (sprites.has(i) && sprites[i]) {
            window.draw(sprites[i]->spriteObj);
        }
    }

    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &drawables = reg.getComponents<ecs::component::Drawable>();

    ecs::Zipper<ecs::component::Position, ecs::component::Drawable> zip(positions, drawables);

    for (auto [pos, draw] : zip) {
        draw.shape.setPosition(pos.x, pos.y);
        window.draw(draw.shape);
    }
}

} // namespace ecs::systems
