/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "systems/control_special.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/controllable.hpp"
#include "components/position.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "core/Zipper.hpp"
#include "udp/UDPClient.hpp"
#include "core/shared_entity.hpp"

static void spawnMissile(
    ecs::Registry &reg,
    ntw::UDPClient &udp,
    ecs::component::Position playerPos,
    ecs::SpriteManager &spriteManager
)
{
    // Create Missile in create_entities !
    rt::UDPClientPacket msg = {
        .header = {.cmd = rt::UDPCommand::NEW_ENTITY},
        .body = {.sharedEntityId = ecs::generateSharedEntityId()},
    };
    msg.body.b.newEntityData = {
        .type = 1, .moveData = {.pos = {playerPos.x + 10, playerPos.y + 10}, .vel = {.vx = 50.f, .vy = 0}}
    };
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::controlSpecial(
    ecs::Registry &reg,
    ecs::InputManager &input,
    ntw::UDPClient &udp,
    SpriteManager &spriteManager
)
{
    auto &controllables = reg.getComponents<ecs::component::Controllable>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    ecs::Zipper<ecs::component::Controllable, ecs::component::Position> zipControl(controllables, positions);

    for (auto [_, pos] : zipControl) {
        if (input.isKeyPressed(sf::Keyboard::Space)) {
            spawnMissile(reg, udp, pos, spriteManager);
        }
    }
}
