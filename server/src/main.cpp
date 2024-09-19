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
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"

#include "UDPServer.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

static void register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
}

static void register_systems(ecs::registry &reg, float &dt)
{
    reg.add_system([&reg]() { ecs::systems::control(reg); });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
}

static void create_player(ecs::registry &reg)
{
    auto player = reg.spawn_entity();
    reg.add_component(player, ecs::component::position{400.f, 300.f});

    reg.add_component(player, ecs::component::velocity{0.f, 0.f});
    reg.add_component(player, ecs::component::controllable{});
    ecs::component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Green);
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

static void run(ecs::registry &reg, float &dt)
{
    sf::Clock clock;

    while (true) {
        dt = clock.restart().asSeconds();

        reg.run_systems();
    }
}

int main()
{
    /*    PART TO IMPLEMENT SERVER UDP    */
    try {
        int port = 8080;
        server::UDPServer udp_server(port);
        udp_server.register_command([](void *data, std::size_t size) {
            std::cout << "command received" << std::endl;
        });
        udp_server.run();
    } catch (std::exception &e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    /*    END PART TO IMPLEMENT SERVER UDP    */

    ecs::registry reg;
    float dt = 0.f;

    register_components(reg);
    register_systems(reg, dt);

    create_player(reg);
    for (int i = 0; i < 1000; ++i) {
        create_static(reg, 100.f * i, 100.f * i);
    }

    run(reg, dt);

    return 0;
}
