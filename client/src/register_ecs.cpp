/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include "rtype_client.hpp"

#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/tag.hpp"
#include "components/velocity.hpp"
#include "core/constants.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"

#include "systems/ai_act.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"
#include "systems/sprite_system.hpp"
#include "systems/position.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"

void rtc::register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::sprite>();
    reg.register_component<ecs::component::animation>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
    reg.register_component<ecs::component::share_movement>();
    reg.register_component<ecs::component::shared_entity>();
    reg.register_component<ecs::component::missile>();
    reg.register_component<ecs::component::ai_actor>();
    reg.register_component<ecs::component::tag<size_t>>();
}

void rtc::register_systems(
    ecs::registry &reg,
    sf::RenderWindow &window,
    float &dt,
    client::UDPClient &udpClient,
    ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager,
    SpriteManager &sprite_manager
)
{
    tick_rate_manager.add_tick_rate(ecs::constants::movement_tick_rate);
    tick_rate_manager.add_tick_rate(10);

    reg.add_system([&reg, &input]() { ecs::systems::control_move(reg, input); });
    reg.add_system([&reg, &input, &udpClient, &sprite_manager]() {
        ecs::systems::control_special(reg, input, udpClient, sprite_manager);
    });
    reg.add_system([&reg, &dt, &tick_rate_manager]() {
        if (tick_rate_manager.need_update(10, dt)) {
            ecs::systems::ai_act(reg);
        }
    });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &udpClient, &tick_rate_manager, &dt]() {
        if (tick_rate_manager.need_update(ecs::constants::movement_tick_rate, dt)) {
            ecs::systems::share_movement(reg, udpClient);
        }
    });
    reg.add_system([&reg]() { ecs::systems::missiles_stop(reg); });
    reg.add_system([&reg, &dt, &sprite_manager]() { ecs::systems::sprite_system(reg, dt, sprite_manager); });
    reg.add_system([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
}
