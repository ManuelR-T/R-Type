/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** parallax
*/

#include "parallax.hpp"
#include "Zipper.hpp"
#include "components/parallax.hpp"
#include "components/position.hpp"

namespace ecs::systems {

void parallax(Registry &reg)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &parallax = reg.getComponents<ecs::component::Parallax>();

    ecs::Zipper<ecs::component::Position, ecs::component::Parallax> zip(positions, parallax);

    for (auto [pos, plx] : zip) {
        if (pos.x < plx.repeatX) {
            pos.x = plx.respawnX;
        }
        if (pos.y < plx.repeatY) {
            pos.y = plx.respawnY;
        }
    }
}

} // namespace ecs::systems
