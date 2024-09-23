/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_special
*/

#include "control_special.hpp"
#include "GameProtocol.hpp"

#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/missile.hpp"
#include "components/share_movement.hpp"
#include "core/zipper.hpp"

static void spawn_missile(
    ecs::registry &reg,
    client::UDPClient &udp,
    ecs::component::position playerPos
)
{
    auto missile = reg.spawn_shared_entity(ecs::generate_shared_entity_id());

    reg.add_component(missile, ecs::component::position{playerPos.x + 10, playerPos.y + 10});
    reg.add_component(missile, ecs::component::velocity{50.f, 0});

    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(20.f, 20.f));
    playerDrawable.shape.setFillColor(sf::Color::Blue);
    reg.add_component(missile, std::move(playerDrawable));

    // reg.add_component(player, component::hitbox{50.f, 50.f});
    reg.add_component(missile, ecs::component::missile{700.0, 700.0});
    reg.add_component(missile, ecs::component::share_movement{});

    ecs::protocol msg = {
        .action = ecs::ntw_action::NEW_ENTITY,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(missile).value().shared_entity_id,
        .data = ecs::ntw::movement{.pos = {playerPos.x, playerPos.y}, .vel = {.vx = 50.f, .vy = 0}}
    };
    udp.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
}

void ecs::systems::control_special(registry &reg, ecs::input_manager &input, client::UDPClient &udp)
{
    auto &controllables = reg.get_components<ecs::component::controllable>();
    auto &positions = reg.get_components<ecs::component::position>();

    zipper<ecs::component::controllable, ecs::component::position> zip_control(controllables, positions);

    for (auto [_, pos] : zip_control) {
        if (input.is_key_pressed(sf::Keyboard::Space)) {
            spawn_missile(reg, udp, pos);
        }
    }
}
