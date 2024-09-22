/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/shared_entity.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"

#include "GameProtocol.hpp"
#include "UDPServer.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <cstring>

static void register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
    reg.register_component<ecs::component::shared_entity>();
}

static void register_systems(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &window]() { // ! for debug
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
}

static void create_player(ecs::registry &reg, shared_entity_t shared_entity_id)
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

static void create_static(ecs::registry &reg, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, ecs::component::position{x, y});

    ecs::component::drawable entityDrawable;
    entityDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    entityDrawable.shape.setFillColor(sf::Color::Red);
    reg.add_component(entity, std::move(entityDrawable));

    reg.add_component(entity, ecs::component::hitbox{50.f, 50.f});
}

static void run(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        // ! for debug
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        // ! for debug
        reg.run_systems();
    }
}

int main()
{
    int port = 8080;
    server::UDPServer udp_server(port);


    std::thread receiveThread([&udp_server]() {
        udp_server.run();
    });

    ecs::registry reg;
    udp_server.register_command([&](char *data, std::size_t size) {
        ecs::protocol msg{};
        std::memcpy(&msg, data, sizeof(msg));
        switch (msg.action)
        {
        case ecs::ntw_action::NEW_PLAYER:
            create_player(reg, msg.shared_entity_id);
            break;
        case ecs::ntw_action::MOD_ENTITY:
            if (std::holds_alternative<ecs::ntw::movement>(msg.data)) {
                reg.get_component<ecs::component::position>(reg.get_local_entity().at(msg.shared_entity_id)).value()
                    = std::get<ecs::ntw::movement>(msg.data).pos;
                reg.get_component<ecs::component::velocity>(reg.get_local_entity().at(msg.shared_entity_id)).value()
                    = std::get<ecs::ntw::movement>(msg.data).vel;
            }
            break;
        }
    });
    float dt = 0.f;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "R-Type"); // ! for deebug

    window.setFramerateLimit(30); // ! for debug
    register_components(reg);
    register_systems(reg, window, dt);

    for (int i = 0; i < 10; ++i) {
        create_static(reg, 100.f * i, 100.f * i);
    }

    run(reg, window, dt);

    receiveThread.join();
    return 0;
}
