/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** colision
*/

#include <SFML/Graphics.hpp>
#include <algorithm>
#include "../components/controllable.hpp"
#include "../components/hitbox.hpp"
#include "../components/position.hpp"
#include "../components/velocity.hpp"
#include "../core/registry.hpp"
#include "../core/sparse_array.hpp"

static void resolve_collision(
    ecs::component::position &pos,
    ecs::sparse_array<ecs::component::velocity> &velocities,
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

    std::vector<size_t> entities;

    size_t maxSize = std::max(positions.size(), hitboxes.size());
    for (size_t i = 0; i < maxSize; ++i) {
        if (i < positions.size() && positions[i] && i < hitboxes.size() && hitboxes[i]) {
            entities.push_back(i);
        }
    }

    for (size_t i = 0; i < entities.size(); ++i) {
        size_t entityA = entities[i];
        auto &posA = *positions[entityA];
        auto &hbA = *hitboxes[entityA];
        sf::FloatRect rectA(posA.x, posA.y, hbA.width, hbA.height);

        for (size_t j = i + 1; j < entities.size(); ++j) {
            size_t entityB = entities[j];
            auto &posB = *positions[entityB];
            auto &hbB = *hitboxes[entityB];
            sf::FloatRect rectB(posB.x, posB.y, hbB.width, hbB.height);

            sf::FloatRect intersection;
            if (rectA.intersects(rectB, intersection)) {
                bool entityAControllable = entityA < controllables.size() && controllables[entityA];
                bool entityBControllable = entityB < controllables.size() && controllables[entityB];

                if (entityAControllable && !entityBControllable) {
                    resolve_collision(posA, velocities, entityA, intersection, velocities[entityA]);
                } else if (!entityAControllable && entityBControllable) {
                    resolve_collision(posB, velocities, entityB, intersection, velocities[entityB]);
                }
                // TODO: If both entities are controllable or both are non-controllable
            }
        }
    }
}

} // namespace ecs::systems
