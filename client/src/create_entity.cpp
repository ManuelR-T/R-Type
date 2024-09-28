/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "GameProtocol.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/Registry.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "rtype_client.hpp"

void rtc::createPlayer(ecs::Registry &reg, client::UDPClient &udpClient, SpriteManager &spriteManager)
{
    auto player = reg.spawnSharedEntity(ecs::generateSharedEntityId());
    reg.addComponent(player, ecs::component::Position{400.f, 300.f});
    reg.addComponent(player, ecs::component::Velocity{0.f, 0.f});
    reg.addComponent(player, ecs::component::Controllable{});

    ecs::component::Sprite playerSprite;
    playerSprite.texture_id = "assets/typesheets/r-typesheet1.gif";
    playerSprite.sprite_obj.setTexture(spriteManager.getTexture(playerSprite.texture_id));
    playerSprite.sprite_obj.setPosition(400.f, 300.f);

    playerSprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 16));
    ecs::component::Animation playerAnimation;
    playerAnimation.frames["up"] = {{135, 2, 32, 16}};
    playerAnimation.frames["top"] = {{102, 2, 32, 16}};
    playerAnimation.frames["neutral"] = {{168, 2, 32, 16}};
    playerAnimation.frames["down"] = {{201, 2, 32, 16}};
    playerAnimation.frames["bottom"] = {{234, 2, 32, 16}};
    playerAnimation.frame_time = 0.1f;
    playerAnimation.updateState = [](ecs::Registry &reg, entity_t id, ecs::component::Animation &anim) {
        auto velOpt = reg.getComponent<ecs::component::Velocity>(id);
        if (!velOpt) {
            return;
        }

        auto &vel = *velOpt;
        float vy = vel.vy;
        std::string &state = anim.state;

        if (vy > 0) {
            if (state == "up" || state == "top") {
                state = "top";
            } else if (state == "neutral") {
                state = "up";
            } else {
                state = "neutral";
            }
        } else if (vy < 0) {
            if (state == "down" || state == "bottom") {
                state = "bottom";
            } else if (state == "neutral") {
                state = "down";
            } else {
                state = "neutral";
            }
        } else {
            if (state == "top") {
                state = "up";
            } else if (state == "bottom") {
                state = "down";
            } else {
                state = "neutral";
            }
        }
    };

    reg.addComponent(player, std::move(playerAnimation));
    reg.addComponent(player, std::move(playerSprite));
    reg.addComponent(player, ecs::component::Hitbox{32.f, 16.f});
    reg.addComponent(player, ecs::component::ShareMovement{});

    rt::UdpPacket msg = {
        .cmd = rt::UdpCommand::NEW_PLAYER,
        .shared_entity_id = reg.getComponent<ecs::component::SharedEntity>(player).value().shared_entity_id};
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void rtc::createStatic(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y)
{
    auto entity = reg.spawnEntity();
    reg.addComponent(entity, ecs::component::Position{x, y});

    ecs::component::Sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(spriteManager.getTexture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::Animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}};

    reg.addComponent(entity, std::move(entityAnimation));
    reg.addComponent(entity, std::move(entitySprite));
    reg.addComponent(entity, ecs::component::Hitbox{32.f, 32.f});
}

void rtc::createAi(ecs::Registry &reg, SpriteManager &spriteManager, float x, float y)
{
    auto entity = reg.spawnEntity();

    reg.addComponent(entity, ecs::component::Position{x, y});

    ecs::component::Sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(spriteManager.getTexture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::Animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}};

    reg.addComponent(entity, std::move(entityAnimation));
    reg.addComponent(entity, std::move(entitySprite));

    auto func = [](ecs::Registry &reg, entity_t e) {
        auto &pos = reg.getComponent<ecs::component::Position>(e);
        auto &val = reg.getComponent<ecs::component::AiActor>(e)->val;

        if (val) {
            pos->y += 20;
        } else {
            pos->y -= 20;
        }
        val = !val;
    };
    reg.addComponent(entity, ecs::component::AiActor{true, std::move(func)});
}
