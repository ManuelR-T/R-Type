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
#include "systems/colision.hpp"
#include "systems/control.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"

#include <SFML/Graphics.hpp>

static void register_components(registry &reg)
{
    reg.register_component<component::position>();
    reg.register_component<component::velocity>();
    reg.register_component<component::drawable>();
    reg.register_component<component::controllable>();
    reg.register_component<component::hitbox>();
}

static void register_systems(registry &reg, sf::RenderWindow &window, float &dt)
{
    reg.add_system([&reg]() { systems::control(reg); });
    reg.add_system([&reg, &dt]() { systems::position(reg, dt); });
    reg.add_system([&reg]() { systems::collision(reg); });
    reg.add_system([&reg, &window]() {
        window.clear();
        systems::draw(reg, window);
        window.display();
    });
}

static void create_player(registry &reg)
{
    auto player = reg.spawn_entity();
    reg.add_component(player, component::position{400.f, 300.f});

    reg.add_component(player, component::velocity{0.f, 0.f});
    reg.add_component(player, component::controllable{});
    component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Green);
    reg.add_component(player, std::move(playerDrawable));

    reg.add_component(player, component::hitbox{50.f, 50.f});
}

static void create_static(registry &reg, float x, float y)
{
    auto entity = reg.spawn_entity();
    reg.add_component(entity, component::position{x, y});

    component::drawable entityDrawable;
    entityDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    entityDrawable.shape.setFillColor(sf::Color::Red);
    reg.add_component(entity, std::move(entityDrawable));

    reg.add_component(entity, component::hitbox{50.f, 50.f});
}

static void run(registry &reg, sf::RenderWindow &window, float &dt)
{
    sf::Clock clock;

    while (window.isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        reg.run_systems();
    }
}

int main()
{
    registry reg;
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
