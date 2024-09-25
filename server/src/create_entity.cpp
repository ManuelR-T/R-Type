/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "core/registry.hpp"

#include <SFML/Graphics.hpp>
#include <cstring>
#include "rtype_server.hpp"

// ! It's a temporary file that will be delete when factory are setup

void rts::create_player(ecs::registry &reg, shared_entity_t shared_entity_id)
{
    auto player = reg.spawn_shared_entity(shared_entity_id);
    reg.add_component(player, ecs::component::position{400.f, 300.f});

    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Blue);
    reg.add_component(player, std::move(playerDrawable));

    reg.add_component(player, ecs::component::hitbox{50.f, 50.f});
}

void rts::create_static(ecs::registry &reg, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::drawable entityDrawable;
    entityDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    entityDrawable.shape.setFillColor(sf::Color::Red);
    reg.add_component(entity, std::move(entityDrawable));

    reg.add_component(entity, ecs::component::hitbox{50.f, 50.f});
}

void rts::create_missile(ecs::registry &reg, ecs::protocol &msg)
{
    auto missile = reg.spawn_shared_entity(msg.shared_entity_id);

    const auto &pos = std::get<ecs::ntw::movement>(msg.data).pos;
    const auto &vel = std::get<ecs::ntw::movement>(msg.data).vel;

    reg.add_component(missile, ecs::component::position{pos.x, pos.y});
    reg.add_component(missile, ecs::component::velocity{vel.vx, vel.vy});

    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(20.f, 20.f));
    playerDrawable.shape.setFillColor(sf::Color::Yellow);
    reg.add_component(missile, std::move(playerDrawable));

    // reg.add_component(player, component::hitbox{50.f, 50.f});
    reg.add_component(missile, ecs::component::missile{700.0, 700.0});
}
