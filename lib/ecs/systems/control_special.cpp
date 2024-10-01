/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "systems/control_special.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "core/Zipper.hpp"
#include "udp/UDPClient.hpp"
#include "components/share_movement.hpp"
#include "core/shared_entity.hpp"

static void spawnMissile(
    ecs::Registry &reg,
    ntw::UDPClient &udp,
    ecs::component::Position playerPos,
    ecs::SpriteManager &spriteManager
)
{
    auto missile = reg.spawnSharedEntity(ecs::generateSharedEntityId());

    reg.addComponent(missile, ecs::component::Position{playerPos.x  + 55, playerPos.y + 8});
    reg.addComponent(missile, ecs::component::Velocity{50.f, 0});
    reg.addComponent(missile, ecs::component::Hitbox{16.0, 16.0});

    ecs::component::Sprite sprite;
    sprite.textureId = "assets/typesheets/r-typesheet1.gif";
    sprite.spriteObj.setTexture(spriteManager.getTexture(sprite.textureId));
    sprite.spriteObj.setPosition({playerPos.x + 10, playerPos.y + 10});
    sprite.spriteObj.setTextureRect(sf::IntRect(0, 0, 16, 16));
    reg.addComponent(missile, std::move(sprite));

    ecs::component::Animation anim;
    anim.frames["neutral"] = {
        {182, 248, 16, 16},
        {200, 240, 16, 16},
        {216, 240, 16, 16},
        {232, 240, 16, 16},
        {248, 240, 16, 16},
        {268, 240, 16, 16},
        {284, 240, 16, 16},
        {300, 240, 16, 16},
        {316, 240, 16, 16},
        {334, 248, 16, 16},
        {316, 256, 16, 16},
        {300, 256, 16, 16},
        {284, 256, 16, 16},
        {268, 256, 16, 16},
        {248, 256, 16, 16},
        {232, 256, 16, 16},
        {216, 256, 16, 16},
        {200, 256, 16, 16},
    };
    anim.frames["right"] = {{334, 248, 16, 16}};
    anim.state = "right";
    reg.addComponent(missile, std::move(anim));

    // reg.addComponent(player, component::hitbox{50.f, 50.f});
    reg.addComponent(missile, ecs::component::Missile{});
    reg.addComponent(missile, ecs::component::ShareMovement{});

    rt::UDPPacket msg = {
        .cmd = rt::UDPCommand::NEW_ENTITY,
        .sharedEntityId = reg.getComponent<ecs::component::SharedEntity>(missile).value().sharedEntityId
    };
    msg.body.shareMovement = {.pos = {playerPos.x + 10, playerPos.y + 10}, .vel = {.vx = 50.f, .vy = 0}};
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
