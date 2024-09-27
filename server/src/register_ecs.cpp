/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include "GameProtocol.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/shared_entity.hpp"
#include "components/share_movement.hpp"
#include "core/response_handler.hpp"
#include "rtype_server.hpp"
#include "systems/missiles_stop.hpp"

void rts::register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
    reg.register_component<ecs::component::shared_entity>();
    reg.register_component<ecs::component::missile>();
    reg.register_component<ecs::component::share_movement>();
}

void rts::register_systems(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &window]() { // ! for debug
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.add_system([&reg]() { ecs::systems::missiles_stop(reg); });
}

void rts::register_response(ecs::registry &reg, ecs::response_handler &response_handler)
{
    response_handler.register_handler(ecs::ntw_action::NEW_PLAYER, [&reg](ecs::protocol &msg) {
        rts::create_player(reg, msg.shared_entity_id);
    });

    response_handler.register_handler(ecs::ntw_action::MOD_ENTITY, [&reg](ecs::protocol &msg) {
        if (std::holds_alternative<ecs::ntw::movement>(msg.data)) {
            reg.get_component<ecs::component::position>(reg.get_local_entity().at(msg.shared_entity_id)).value() =
                std::get<ecs::ntw::movement>(msg.data).pos;
            reg.get_component<ecs::component::velocity>(reg.get_local_entity().at(msg.shared_entity_id)).value() =
                std::get<ecs::ntw::movement>(msg.data).vel;
        }
    });
    response_handler.register_handler(ecs::ntw_action::NEW_ENTITY, [&reg](ecs::protocol &msg) {
        rts::create_missile(reg, msg);
    });
}
