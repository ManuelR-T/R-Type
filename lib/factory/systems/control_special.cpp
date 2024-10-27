/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "systems/control_special.hpp"
#include "InputManager.hpp"
#include "KeyBind.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/position.hpp"
#include "udp/UDPClient.hpp"
#include "shared_entity.hpp"

static void spawnMissile(ntw::UDPClient &udp, ecs::component::Position playerPos)
{
    rt::UDPPacket<rt::UDPBody::NEW_ENTITY_MISSILE> msg(
        rt::UDPCommand::NEW_ENTITY_MISSILE, ecs::generateSharedEntityId()
    );
    msg.body.pos = {playerPos.x + 30, playerPos.y};
    msg.body.vel = {250, 0};
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::controlSpecial(
    ecs::Registry &reg,
    ecs::InputManager &input,
    ntw::UDPClient &udp,
    const ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind,
    int spawnRate
)
{
    auto &controllables = reg.getComponents<ecs::component::Controllable>();
    auto &positions = reg.getComponents<ecs::component::Position>();

    ecs::Zipper<ecs::component::Controllable, ecs::component::Position> zipControl(controllables, positions);

    static auto lastTime = std::chrono::high_resolution_clock::now();
    for (auto [_, pos] : zipControl) {
        if (input.isKeyPressed(keyBind.getActionKey(rt::PlayerAction::SHOOT_MISSILE))) {
            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() < spawnRate) {
                continue;
            }
            lastTime = now;
            spawnMissile(udp, pos);
        }
    }
}
