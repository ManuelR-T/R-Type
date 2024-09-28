/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "systems/control_special.hpp"
#include "GameProtocol.hpp"
#include "UDPClient.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/InputManager.hpp"
#include "core/Registry.hpp"
#include "core/SpriteManager.hpp"
#include "core/Zipper.hpp"
#include "components/share_movement.hpp"
#include "core/shared_entity.hpp"

static void spawnMissile(
    ecs::Registry &reg,
    client::UDPClient &udp,
    ecs::component::Position playerPos,
    SpriteManager &spriteManager
)
{
    auto missile = reg.spawnSharedEntity(ecs::generateSharedEntityId());

    reg.addComponent(missile, ecs::component::Position{playerPos.x + 10, playerPos.y + 10});
    reg.addComponent(missile, ecs::component::Velocity{50.f, 0});

    ecs::component::Sprite sprite;
    sprite.texture_id = "assets/typesheets/r-typesheet1.gif";
    sprite.sprite_obj.setTexture(spriteManager.getTexture(sprite.texture_id));
    sprite.sprite_obj.setPosition({playerPos.x + 10, playerPos.y + 10});
    sprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 16, 16));
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

    rt::UdpPacket msg = {
        .cmd = rt::UdpCommand::NEW_ENTITY,
        .shared_entity_id = reg.getComponent<ecs::component::SharedEntity>(missile).value().shared_entity_id
    };
    msg.body.share_movement = {.pos = {playerPos.x + 10, playerPos.y + 10}, .vel = {.vx = 50.f, .vy = 0}};
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::controlSpecial(
    ecs::Registry &reg,
    ecs::InputManager &input,
    client::UDPClient &udp,
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
