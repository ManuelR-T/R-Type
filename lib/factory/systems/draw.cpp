/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw
*/

#include <SFML/Graphics.hpp>
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/drawable.hpp"
#include "components/parallax.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"

namespace ecs::systems {

void draw(Registry &reg, sf::RenderWindow &window)
{
    auto &sprites = reg.getComponents<component::Sprite>();
    auto &parallaxes = reg.getComponents<ecs::component::Parallax>();

    ecs::Zipper<ecs::component::Parallax, component::Sprite> zipParallax(parallaxes, sprites);

    size_t maxLayer = 0;
    for (auto [parallax, sprite] : zipParallax) {
        if (parallax.layer > maxLayer) {
            maxLayer = parallax.layer;
        }
    }

    for (size_t currentLayer = 0; currentLayer <= maxLayer; ++currentLayer) {
        for (auto [parallax, sprite] : zipParallax) {
            if (parallax.layer == currentLayer) {
                window.draw(sprite.spriteObj);
            }
        }
    }

    for (size_t i = 0; i < sprites.size(); ++i) {
        if (sprites.has(i) && sprites[i] && !reg.hasComponent<component::Parallax>(i)) {
            window.draw(sprites[i]->spriteObj);
        }
    }

    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &drawables = reg.getComponents<ecs::component::Drawable>();

    ecs::Zipper<ecs::component::Position, ecs::component::Drawable> zipDrawables(positions, drawables);

    for (auto [pos, draw] : zipDrawables) {
        draw.shape.setPosition(pos.x, pos.y);
        window.draw(draw.shape);
    }
}

} // namespace ecs::systems
