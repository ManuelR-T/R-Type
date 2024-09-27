/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "GameProtocol.hpp"
#include "rtype_client.hpp"

void rtc::create_player(ecs::registry &reg, client::UDPClient &udpClient, SpriteManager &sprite_manager)
{
    auto player = reg.spawn_shared_entity(ecs::generate_shared_entity_id());
    reg.add_component(player, ecs::component::position{400.f, 300.f});
    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    reg.add_component(player, ecs::component::controllable{});

    ecs::component::sprite playerSprite;
    playerSprite.texture_id = "assets/typesheets/r-typesheet1.gif";
    playerSprite.sprite_obj.setTexture(sprite_manager.get_texture(playerSprite.texture_id));
    playerSprite.sprite_obj.setPosition(400.f, 300.f);

    playerSprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 16));
    ecs::component::animation playerAnimation;
    playerAnimation.frames["up"] = {{135, 2, 32, 16}};
    playerAnimation.frames["top"] = {{102, 2, 32, 16}};
    playerAnimation.frames["neutral"] = {{168, 2, 32, 16}};
    playerAnimation.frames["down"] = {{201, 2, 32, 16}};
    playerAnimation.frames["bottom"] = {{234, 2, 32, 16}};
    playerAnimation.frame_time = 0.1f;
    playerAnimation.updateState = [](ecs::registry &reg, entity_t id, ecs::component::animation &anim) {
        auto vel_opt = reg.get_component<ecs::component::velocity>(id);
        if (!vel_opt) {
            return;
        }

        auto &vel = *vel_opt;
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

    reg.add_component(player, std::move(playerAnimation));
    reg.add_component(player, std::move(playerSprite));
    reg.add_component(player, ecs::component::hitbox{32.f, 16.f});
    reg.add_component(player, ecs::component::share_movement{});

    // ! will be changed with lobby
    rt::udp_packet msg = {
        .cmd = rt::udp_command::NEW_PLAYER,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(player).value().shared_entity_id
    };
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    // ! will be changed with lobby
}

void rtc::create_static(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(sprite_manager.get_texture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::animation entityAnimation;
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

    reg.add_component(entity, std::move(entityAnimation));
    reg.add_component(entity, std::move(entitySprite));
    reg.add_component(entity, ecs::component::hitbox{32.f, 32.f});
}

void rtc::create_ai(ecs::registry &reg, SpriteManager &sprite_manager, float x, float y)
{
    auto entity = reg.spawn_entity();

    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::sprite entitySprite;
    entitySprite.texture_id = "assets/typesheets/r-typesheet5.gif";
    entitySprite.sprite_obj.setTexture(sprite_manager.get_texture(entitySprite.texture_id));
    entitySprite.sprite_obj.setPosition(x, y);

    entitySprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 32, 32));
    ecs::component::animation entityAnimation;
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

    reg.add_component(entity, std::move(entityAnimation));
    reg.add_component(entity, std::move(entitySprite));

    auto func = [](ecs::registry &reg, entity_t e) {
        auto &pos = reg.get_component<ecs::component::position>(e);
        auto &val = reg.get_component<ecs::component::ai_actor>(e)->val;

        if (val) {
            pos->y += 20;
        } else {
            pos->y -= 20;
        }
        val = !val;
    };
    reg.add_component(entity, ecs::component::ai_actor{true, std::move(func)});
}
