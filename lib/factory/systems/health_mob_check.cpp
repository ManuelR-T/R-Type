/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** health_mob_check
*/

#include "health_mob_check.hpp"
#include "IndexedZipper.hpp"
#include "components/health.hpp"

void ecs::systems::healthMobCheck(Registry &reg, WaveManager &waveManager)
{
    auto &healths = reg.getComponents<ecs::component::Health>();

    ecs::IndexedZipper<component::Health> zipHealth(healths);

    for (auto [entityId, health] : zipHealth) {
        if (health.currHp <= 0 && waveManager.isMob(entityId)) {
            waveManager.removeEntity(entityId);
        }
    }
}
