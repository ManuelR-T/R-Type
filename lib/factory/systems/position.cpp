/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** position
*/

#include "components/position.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/velocity.hpp"

namespace ecs::systems {

void position(Registry &reg, float dt)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();

    ecs::Zipper<ecs::component::Position, ecs::component::Velocity> zip(positions, velocities);

    for (auto [pos, vel] : zip) {
        pos.x += vel.vx * dt;
        pos.y += vel.vy * dt;
    }
}

} // namespace ecs::systems
