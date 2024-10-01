/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** collision
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cstddef>
#include <iostream>
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/missile.hpp"
#include "core/registry.hpp"
#include "core/sparse_array.hpp"

static void resolve_collision(
    ecs::registry &reg,
    ecs::component::position &pos,
    size_t entity,
    const sf::FloatRect &intersection,
    std::optional<ecs::component::velocity> &vel
)
{
    if (!vel) {
        return;
    }

    if (intersection.width < intersection.height) {
        if (vel->vx > 0) {
            pos.x -= intersection.width;
        } else if (vel->vx < 0) {
            pos.x += intersection.width;
        }
        vel->vx = 0;
    } else {
        if (vel->vy > 0) {
            pos.y -= intersection.height;
        } else if (vel->vy < 0) {
            pos.y += intersection.height;
        }
        vel->vy = 0;
    }
}

static void resolve_tag_effect(ecs::registry &reg, size_t entityA, size_t entityB)
{
    auto &missiles = reg.get_components<ecs::component::missile>();

    if (missiles.has(entityA) && !missiles.has(entityB)) {
        std::cout << "Entity B is dead => " << entityB << std::endl;
        reg.kill_entity(entityB);
    }
    if (missiles.has(entityB) && !missiles.has(entityA)) {
        std::cout << "Entity A is dead => " << entityA << std::endl;
        reg.kill_entity(entityA);
    }
}

namespace ecs::systems {

void collision(registry &reg)
{
    auto &positions = reg.get_components<ecs::component::position>();
    auto &hitboxes = reg.get_components<ecs::component::hitbox>();
    auto &velocities = reg.get_components<ecs::component::velocity>();
    auto &controllables = reg.get_components<ecs::component::controllable>();

    size_t maxEntity = std::max(positions.size(), hitboxes.size());

    for (size_t entityA = 0; entityA < maxEntity; ++entityA) {
        if (!positions.has(entityA) || !hitboxes.has(entityA)) {
            continue;
        }
        auto &posA = *positions[entityA];
        auto &hbA = *hitboxes[entityA];
        sf::FloatRect rectA(posA.x, posA.y, hbA.width, hbA.height);

        for (size_t entityB = entityA + 1; entityB < maxEntity; ++entityB) {
            if (!positions.has(entityB) || !hitboxes.has(entityB)) {
                continue;
            }
            auto &posB = *positions[entityB];
            auto &hbB = *hitboxes[entityB];
            sf::FloatRect rectB(posB.x, posB.y, hbB.width, hbB.height);

            sf::FloatRect intersection;
            if (rectA.intersects(rectB, intersection)) {
                bool entityAControllable = controllables.has(entityA);
                bool entityBControllable = controllables.has(entityB);

                if (entityAControllable && !entityBControllable) {
                    resolve_collision(reg, posA, entityA, intersection, velocities[entityA]);
                } else if (!entityAControllable && entityBControllable) {
                    resolve_collision(reg, posB, entityB, intersection, velocities[entityB]);
                }
                // TODO: If both entities are controllable or both are non-controllable
                resolve_tag_effect(reg, entityA, entityB);
            }
        }
    }
}

} // namespace ecs::systems
