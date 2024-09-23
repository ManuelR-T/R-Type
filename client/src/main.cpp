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
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/registry.hpp"
#include "core/shared_entity.hpp"
#include "core/input_manager.hpp"
#include "core/constants.hpp"
#include "core/tick_rate_manager.hpp"
#include "systems/collision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "systems/share_movement.hpp"
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
    reg.register_component<ecs::component::share_movement>();
    reg.register_component<ecs::component::shared_entity>();
}

static void register_systems(ecs::registry &reg, sf::RenderWindow &window,
    float &dt, client::UDPClient &udpClient, ecs::input_manager &input,
    ecs::tick_rate_manager &tick_rate_manager)
{
    tick_rate_manager.add_tick_rate(ecs::constants::movement_tick_rate);

    reg.add_system([&reg, &input]() { ecs::systems::control(reg, input); });
    reg.add_system([&reg, &dt]() { ecs::systems::position(reg, dt); });
    reg.add_system([&reg]() { ecs::systems::collision(reg); });
    reg.add_system([&reg, &window]() {
        window.clear();
        ecs::systems::draw(reg, window);
        window.display();
    });
    reg.add_system([&reg, &udpClient, &tick_rate_manager, &dt]() {
        if (tick_rate_manager.need_update(ecs::constants::movement_tick_rate, dt)) {
            ecs::systems::share_movement(reg, udpClient);
        }
    });
}

static void create_player(ecs::registry &reg, client::UDPClient &udpClient)
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
        .shared_entity_id = reg.get_component<ecs::component::shared_entity>(player).value().shared_entity_id
    };
    udpClient.send(reinterpret_cast<const char *>(&msg), sizeof(msg));
    // ! will be changed with lobby
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

static void run(ecs::registry &reg, sf::RenderWindow &window,
    float &dt, client::UDPClient &udpClient, ecs::input_manager &input)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            input.update(event);
        }
        reg.run_systems();
    }
}

int main()
{
   try {
        sf::RenderWindow window(sf::VideoMode(1280, 720), "R-Type");
        window.setFramerateLimit(ecs::constants::fps_limit);

        std::vector<sf::RectangleShape> boxesList;

        // rectangle.setSize(sf::Vector2f{200, 200});
        // rectangle.setFillColor(sf::Color{112, 122, 122});
        // while (window.isOpen()) {
        //     sf::Event event;
        //     while (window.pollEvent(event)) {
        //         if (event.type == sf::Event::Closed ||
        //             (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        //             window.close();
        //         }
        //     }

        //     window.clear(sf::Color::Black);

        //     window.draw(rectangle);

        //     window.display();
        // }

    return 0;
        // Setup assets menu (RectangleShape)
        // Loop menu

        // Affichage Tableau
            // 1) Create New Room
            // 2) Can scrollate
            // 3) Room X / Nb players / Delete

        // Join room
        // Status des joueurs -> "Waiting" or "Ready"
        // Screen with "Starting in X seconds" -> After leave infinite loop

        client::UDPClient udpClient("127.0.0.1", 8080);
        std::thread receiveThread([&udpClient]() {
            udpClient.run();
        });

        ecs::registry reg;
        float dt = 0.f;
        ecs::input_manager input_manager;
        ecs::tick_rate_manager tick_rate_manager;

        register_components(reg);
        register_systems(reg, window, dt, udpClient, input_manager, tick_rate_manager);

        create_player(reg, udpClient);

        for (int i = 0; i < 10; ++i) {
            create_static(reg, 100.f * i, 100.f * i);
        }

        run(reg, window, dt, udpClient, input_manager);

        receiveThread.join();
    } catch (const std::exception &exception) {
        my::log::error(exception.what());
        return 84;
    } catch (...) {
        my::log::error("Unknow error.");
        return 84;
    }
    return 0;
}
