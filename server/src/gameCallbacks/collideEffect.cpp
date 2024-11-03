/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** collideEffect
*/

#include "collideEffect.hpp"
#include <cstddef>
#include <cstdio>

#include "RTypeUDPProtol.hpp"
#include "components/health.hpp"
#include "components/missile.hpp"
#include "components/tag.hpp"
#include "entity.hpp"
#include "components/game_tag.hpp"
#include "components/health_xp.hpp"

// NOLINTBEGIN(bugprone-unchecked-optional-access)

static void tagEffectKill(ecs::Registry &reg, entity_t entity, std::list<std::vector<char>> &datasToSend)
{
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::DEL_ENTITY>(rt::UDPCommand::DEL_ENTITY, sharedId, true).serialize()
        );
    }
    reg.killEntity(entity);
}

static void tagEffectDamage(ecs::Registry &reg, entity_t entity, int damage, std::list<std::vector<char>> &datasToSend)
{
    if (!reg.hasComponent<ecs::component::Health>(entity)) {
        tagEffectKill(reg, entity, datasToSend);
        return;
    }
    auto &health = reg.getComponents<ecs::component::Health>();

    health[entity]->currHp -= damage;
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;
        datasToSend.push_back(rt::UDPPacket<rt::UDPBody::TAKE_DAMAGE>(
                                  rt::UDPCommand::TAKE_DAMAGE, sharedId, rt::UDPBody::TAKE_DAMAGE{.damage = damage}
        )
                                  .serialize());
    }
}

static void resolveDamage(ecs::Registry &reg, entity_t ally, entity_t enemy, std::list<std::vector<char>> &datasToSend)
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();

    bool isAllyMissile = missiles.has(ally);
    bool isEnemyMissile = missiles.has(enemy);

    if (isAllyMissile && isEnemyMissile) {
        return;
    }
    if (isAllyMissile && !isEnemyMissile) {
        tagEffectDamage(reg, enemy, missiles[ally]->damage, datasToSend);
        tagEffectKill(reg, ally, datasToSend);
        return;
    }
    if (isEnemyMissile && !isAllyMissile) {
        tagEffectDamage(reg, ally, missiles[enemy]->damage, datasToSend);
        tagEffectKill(reg, enemy, datasToSend);
        return;
    }
    tagEffectDamage(reg, ally, 1, datasToSend);
}

static void tagEffectPowerUp(ecs::Registry &reg, entity_t entity, int value, std::list<std::vector<char>> &datasToSend)
{
    if (!reg.hasComponent<ecs::component::Health>(entity)) {
        tagEffectKill(reg, entity, datasToSend);
        return;
    }
    auto &health = reg.getComponents<ecs::component::Health>();

    if (health[entity]->currHp + value > health[entity]->maxHp) {
        value = health[entity]->maxHp - health[entity]->currHp;
    }
    health[entity]->currHp += value;
    if (reg.hasComponent<ecs::component::SharedEntity>(entity)) {
        auto sharedId = reg.getComponent<ecs::component::SharedEntity>(entity)->sharedEntityId;

        datasToSend.push_back(
            rt::UDPPacket<rt::UDPBody::INCREASE_HEALTH>(
                rt::UDPCommand::INCREASE_HEALTH, sharedId, rt::UDPBody::INCREASE_HEALTH{.health = value}
            )
                .serialize()
        );
    }
}

static void resolvePowerUp(ecs::Registry &reg, entity_t ally, entity_t enemy, std::list<std::vector<char>> &datasToSend)
{
    auto &healthsXp = reg.getComponents<ecs::component::HealthXP>();

    bool isAllyPowerUp = healthsXp.has(ally);
    bool isEnemyPowerUp = healthsXp.has(enemy);

    if (isAllyPowerUp && isEnemyPowerUp) {
        return;
    }
    if (isAllyPowerUp && !isEnemyPowerUp) {
        tagEffectPowerUp(reg, enemy, healthsXp[ally]->value, datasToSend);
        tagEffectDamage(reg, enemy, 1, datasToSend);
        return;
    }
    if (isEnemyPowerUp && !isAllyPowerUp) {
        tagEffectPowerUp(reg, ally, healthsXp[enemy]->value, datasToSend);
        tagEffectDamage(reg, enemy, 1, datasToSend);
        return;
    }
    tagEffectPowerUp(reg, ally, 1, datasToSend);
}

void rts::collideEffect(
    ecs::Registry &reg,
    entity_t entityA,
    entity_t entityB,
    std::list<std::vector<char>> &datasToSend
)
{
    auto &tags = reg.getComponents<ecs::component::Tag<ecs::component::EntityTag>>();

    if (!tags.has(entityA) || !tags.has(entityB)) { // check again if it can happen
        return;
    }

    auto [tagA] = *tags[entityA];
    auto [tagB] = *tags[entityB];

    if (tagA == ecs::component::EntityTag::NONE || tagB == ecs::component::EntityTag::NONE) {
        return;
    }
    if (tagA == ecs::component::EntityTag::ALLY && tagB == ecs::component::EntityTag::ENEMY) {
        resolveDamage(reg, entityA, entityB, datasToSend);
    }
    if (tagB == ecs::component::EntityTag::ALLY && tagA == ecs::component::EntityTag::ENEMY) {
        resolveDamage(reg, entityB, entityA, datasToSend);
    }
    if (tagA == ecs::component::EntityTag::ALLY && tagB == ecs::component::EntityTag::POWER_UP) {
        resolvePowerUp(reg, entityA, entityB, datasToSend);
    }
    if (tagB == ecs::component::EntityTag::ALLY && tagA == ecs::component::EntityTag::POWER_UP) {
        resolvePowerUp(reg, entityB, entityA, datasToSend);
    }
}

void rts::resolveTagEffect(
    ecs::Registry &reg,
    size_t entityA,
    size_t entityB,
    std::list<std::vector<char>> &datasToSend
) // Use tag here now !
{
    auto &missiles = reg.getComponents<ecs::component::Missile>();
    auto &health = reg.getComponents<ecs::component::Health>();

    if (missiles.has(entityA) && !missiles.has(entityB)) {
        if (health.has(entityB)) {
            tagEffectDamage(reg, entityB, missiles[entityA]->damage, datasToSend);
            health[entityB]->currHp -= missiles[entityA]->damage;
        } else {
            tagEffectKill(reg, entityB, datasToSend);
        }
        tagEffectKill(reg, entityA, datasToSend);
    }
    if (missiles.has(entityB) && !missiles.has(entityA)) {
        if (health.has(entityA)) {
            tagEffectDamage(reg, entityA, missiles[entityB]->damage, datasToSend);
            health[entityA]->currHp -= missiles[entityB]->damage;
        } else {
            tagEffectKill(reg, entityA, datasToSend);
        }
        tagEffectKill(reg, entityB, datasToSend);
    }
}

// NOLINTEND(bugprone-unchecked-optional-access)
