/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** collision
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/registry.hpp"

static void resolveCollision(
    ecs::Registry &reg,
    ecs::component::Position &pos,
    size_t entity,
    const sf::FloatRect &intersection,
    std::optional<ecs::component::Velocity> &vel
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

void collision(Registry &reg)
{
    auto &positions = reg.getComponents<ecs::component::Position>();
    auto &hitboxes = reg.getComponents<ecs::component::Hitbox>();
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &controllables = reg.getComponents<ecs::component::Controllable>();

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
            size_t entityB = j;
            auto &posB = *positions[entityB];
            auto &hbB = *hitboxes[entityB];
            sf::FloatRect rectB(posB.x, posB.y, hbB.width, hbB.height);

            sf::FloatRect intersection;
            if (rectA.intersects(rectB, intersection)) {
                bool entityAControllable = controllables.has(entityA);
                bool entityBControllable = controllables.has(entityB);

                if (entityAControllable && !entityBControllable) {
                    resolveCollision(reg, posA, entityA, intersection, velocities[entityA]);
                } else if (!entityAControllable && entityBControllable) {
                    resolveCollision(reg, posB, entityB, intersection, velocities[entityB]);
                }
                // TODO: If both entities are controllable or both are non-controllable
            }
        }
    }
}

} // namespace ecs::systems
