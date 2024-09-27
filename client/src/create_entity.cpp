/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** create_entity
*/

#include "rtype_client.hpp"
#include "GameProtocol.hpp"

void rtc::create_player(ecs::registry &reg, client::UDPClient &udpClient)
{
    auto player = reg.spawn_shared_entity(ecs::generate_shared_entity_id());
    reg.add_component(player, ecs::component::position{400.f, 300.f});

    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    reg.add_component(player, ecs::component::controllable{});
    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Green);
    reg.add_component(player, std::move(playerDrawable));

    reg.add_component(player, ecs::component::hitbox{50.f, 50.f});
    reg.add_component(player, ecs::component::share_movement{});

    // ! will be changed with lobby
    ecs::protocol msg = {
        .action = ecs::ntw_action::NEW_PLAYER,
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(player).value().shared_entity_id};
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    // ! will be changed with lobby
}

void rtc::create_static(ecs::registry &reg, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::drawable entityDrawable;
    entityDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    entityDrawable.shape.setFillColor(sf::Color::Red);
    reg.add_component(entity, std::move(entityDrawable));

    reg.add_component(entity, ecs::component::hitbox{50.f, 50.f});
}
