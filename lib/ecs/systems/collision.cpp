/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** collision
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "components/ally.hpp"
#include "components/controllable.hpp"
#include "components/enemy.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
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

namespace ecs::systems {

void collision(registry &reg)
{
    auto &positions = reg.get_components<ecs::component::position>();
    auto &hitboxes = reg.get_components<ecs::component::hitbox>();
    auto &velocities = reg.get_components<ecs::component::velocity>();
    auto &controllables = reg.get_components<ecs::component::controllable>();

    size_t maxSize = std::max(positions.size(), hitboxes.size());

    for (size_t i = 0; i < maxSize; ++i) {
        if (!positions.has(i) || !hitboxes.has(i)) {
            continue;
        }
        size_t entityA = i;
        auto &posA = *positions[entityA];
        auto &hbA = *hitboxes[entityA];
        sf::FloatRect rectA(posA.x, posA.y, hbA.width, hbA.height);

        for (size_t j = i + 1; j < maxSize; ++j) {
            if (!positions.has(j) || !hitboxes.has(j)) {
                continue;
            }
            size_t entityB = j;
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
            }
        }
    }
}

} // namespace ecs::systems
