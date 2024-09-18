/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** draw
*/

#include <SFML/Graphics.hpp>
#include "../components/drawable.hpp"
#include "../components/position.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"

namespace systems {

void draw(registry &reg, sf::RenderWindow &window)
{
    auto &positions = reg.get_components<ecs::component::position>();
    auto &drawables = reg.get_components<ecs::component::drawable>();

    zipper<ecs::component::position, ecs::component::drawable> zip(positions, drawables);

    for (auto [pos, draw] : zip) {
        draw.shape.setPosition(pos.x, pos.y);
        window.draw(draw.shape);
    }
}

} // namespace systems
