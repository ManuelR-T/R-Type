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
#include "UDPClient.hpp"

#include <SFML/Graphics.hpp>
#include <thread>

#include "my_log.hpp"
#include "my_tracked_exception.hpp"
#include "GameProtocol.hpp"
#include <iostream>

static void register_components(ecs::registry &reg)
{
    reg.register_component<ecs::component::position>();
    reg.register_component<ecs::component::velocity>();
    reg.register_component<ecs::component::drawable>();
    reg.register_component<ecs::component::controllable>();
    reg.register_component<ecs::component::hitbox>();
}

static void register_systems(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    reg.add_system([&reg]() { ecs::systems::control(reg); });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
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

static void run(ecs::registry &reg, sf::RenderWindow &window, float &dt)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        reg.run_systems();
    }
}

int main()
{
    /*    PART TO IMPLEMENT CLIENT UDP    */
    client::UDPClient udpClient("127.0.0.1", 8080);
    std::thread receiveThread([&udpClient]() {
        udpClient.run();
    });
    while (true) {
        ecs::protocol msg = {
            .action = ecs::ntw_action::NEW_ENTITY,
            .size = 0,
            .data = 0
        };
        udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    receiveThread.join();
    /*  END PART TO IMPLEMENT CLIENT UDP    */

//    try {
//        throw my::tracked_exception("R-TYPE !!!!!!!!!!!!!");
//    } catch (const std::exception &exception) {
//        my::log::error(exception.what());
//        return 84;
//    } catch (...) {
//        my::log::error("Unknow error.");
//        return 84;
//    }
//    return 0;

    ecs::registry reg;
    float dt = 0.f;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "R-Type");

    window.setFramerateLimit(60);
    register_components(reg);
    register_systems(reg, window, dt);

    create_player(reg);
    for (int i = 0; i < 1000; ++i) {
        create_static(reg, 100.f * i, 100.f * i);
    }

    run(reg, window, dt);

    return 0;
}
