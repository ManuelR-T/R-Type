/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "RTypeClient.hpp"
#include "RTypeUDPProtol.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "components/missile.hpp"
#include "core/Registry.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"

void rtc::createPlayer(ecs::Registry &reg, ntw::UDPClient &udpClient, ecs::SpriteManager &spriteManager)
{
    auto player = reg.spawnSharedEntity(ecs::generateSharedEntityId());
    reg.addComponent(player, ecs::component::Position{400.f, 300.f});
    reg.addComponent(player, ecs::component::Velocity{0.f, 0.f});
    reg.addComponent(player, ecs::component::Controllable{});

    ecs::component::Sprite playerSprite;
    playerSprite.textureId = "assets/typesheets/r-typesheet1.gif";
    playerSprite.spriteObj.setTexture(spriteManager.getTexture(playerSprite.textureId));
    playerSprite.spriteObj.setPosition(400.f, 300.f);

    playerSprite.spriteObj.setTextureRect(sf::IntRect(0, 0, 32, 16));
    ecs::component::Animation playerAnimation;
    playerAnimation.frames["up"] = {{135, 2, 32, 16}};
    playerAnimation.frames["top"] = {{102, 2, 32, 16}};
    playerAnimation.frames["neutral"] = {{168, 2, 32, 16}};
    playerAnimation.frames["down"] = {{201, 2, 32, 16}};
    playerAnimation.frames["bottom"] = {{234, 2, 32, 16}};
    playerAnimation.frameTime = 0.1f;
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

    rt::UDPClientPacket msg = {
        .header = {.magic = 4242, .cmd = rt::UDPCommand::NEW_PLAYER},
        .body = {.sharedEntityId = reg.getComponent<ecs::component::SharedEntity>(player).value().sharedEntityId}
    };
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void rtc::createStatic(ecs::Registry &reg, ecs::SpriteManager &spriteManager, float x, float y)
{
    auto entity = reg.spawnEntity();
    reg.addComponent(entity, ecs::component::Position{x, y});

    ecs::component::Sprite entitySprite;
    entitySprite.textureId = "assets/typesheets/r-typesheet5.gif";
    entitySprite.spriteObj.setTexture(spriteManager.getTexture(entitySprite.textureId));
    entitySprite.spriteObj.setPosition(x, y);

    entitySprite.spriteObj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::Animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}
    };

    reg.addComponent(entity, std::move(entityAnimation));
    reg.addComponent(entity, std::move(entitySprite));
    reg.addComponent(entity, ecs::component::Hitbox{32.f, 32.f});
}

void rtc::createAi(ecs::Registry &reg, ecs::SpriteManager &spriteManager, float x, float y)
{
    auto entity = reg.spawnEntity();

    reg.addComponent(entity, ecs::component::Position{x, y});

    ecs::component::Sprite entitySprite;
    entitySprite.textureId = "assets/typesheets/r-typesheet5.gif";
    entitySprite.spriteObj.setTexture(spriteManager.getTexture(entitySprite.textureId));
    entitySprite.spriteObj.setPosition(x, y);

    entitySprite.spriteObj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::Animation entityAnimation;
    entityAnimation.frames["neutral"] = {
        {0, 0, 32, 32},
        {32, 0, 32, 32},
        {64, 0, 32, 32},
        {96, 0, 32, 32},
        {128, 0, 32, 32},
        {160, 0, 32, 32},
        {192, 0, 32, 32},
        {224, 0, 32, 32}
    };

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

void rtc::createMissile(
    ecs::Registry &reg,
    ecs::SpriteManager &spriteManager,
    shared_entity_t sharedEntityId,
    float x,
    float y
)
{
    auto missile = reg.spawnSharedEntity(sharedEntityId);

    reg.addComponent(missile, ecs::component::Position{x + 55, y + 8});
    reg.addComponent(missile, ecs::component::Velocity{50.f, 0});
    reg.addComponent(missile, ecs::component::Hitbox{16.0, 16.0});

    ecs::component::Sprite sprite;
    sprite.textureId = "assets/typesheets/r-typesheet1.gif";
    sprite.spriteObj.setTexture(spriteManager.getTexture(sprite.textureId));
    sprite.spriteObj.setPosition({x + 10, y + 10});
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
}
