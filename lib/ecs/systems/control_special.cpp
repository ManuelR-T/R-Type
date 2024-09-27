/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "control_special.hpp"
#include "GameProtocol.hpp"

#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/SpriteManager.hpp"
#include "core/zipper.hpp"
#include "components/share_movement.hpp"

static void spawn_missile(
    ecs::registry &reg,
    client::UDPClient &udp,
    ecs::component::position playerPos,
    SpriteManager &sprite_manager
)
{
    auto missile = reg.spawn_shared_entity(ecs::generate_shared_entity_id());

    reg.add_component(missile, ecs::component::position{playerPos.x + 10, playerPos.y + 10});
    reg.add_component(missile, ecs::component::velocity{50.f, 0});

    ecs::component::sprite sprite;
    sprite.texture_id = "assets/typesheets/r-typesheet1.gif";
    sprite.sprite_obj.setTexture(sprite_manager.get_texture(sprite.texture_id));
    sprite.sprite_obj.setPosition({playerPos.x + 10, playerPos.y + 10});
    sprite.sprite_obj.setTextureRect(sf::IntRect(0, 0, 16, 16));
    reg.add_component(missile, std::move(sprite));

    ecs::component::animation anim;
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
    reg.add_component(missile, std::move(anim));

    // reg.add_component(player, component::hitbox{50.f, 50.f});
    reg.add_component(missile, ecs::component::missile{});
    reg.add_component(missile, ecs::component::share_movement{});

    rt::udp_packet msg = {
        .cmd = rt::udp_command::NEW_ENTITY,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(missile).value().shared_entity_id};
    msg.body.share_movement = {.pos = {playerPos.x + 10, playerPos.y + 10}, .vel = {.vx = 50.f, .vy = 0}};
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::control_special(
    registry &reg,
    ecs::input_manager &input,
    client::UDPClient &udp,
    SpriteManager &sprite_manager
)
{
    auto &controllables = reg.get_components<ecs::component::controllable>();
    auto &positions = reg.get_components<ecs::component::position>();

    zipper<ecs::component::controllable, ecs::component::position> zip_control(controllables, positions);

    for (auto [_, pos] : zip_control) {
        if (input.is_key_pressed(sf::Keyboard::Space)) {
            spawn_missile(reg, udp, pos, sprite_manager);
        }
    }
}
