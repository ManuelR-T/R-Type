/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** register_ecs
*/

#include <SFML/Graphics.hpp>
#include <cstddef>
#include "GameProtocol.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/tag.hpp"
#include "components/velocity.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/response_handler.hpp"
#include "rtype_server.hpp"
#include "systems/missiles_stop.hpp"

void rts::register_components(ecs::registry &reg)
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
