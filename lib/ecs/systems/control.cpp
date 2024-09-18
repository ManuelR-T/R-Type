/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** control
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include "../components/controllable.hpp"
#include "../components/velocity.hpp"
#include "../components/drawable.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"

static void spawn_missile(registry &reg)
{
    auto missile = reg.spawn_entity();
    reg.add_component(missile, component::position{0.f, 100.f});

    reg.add_component(missile, component::velocity{50.f, 50.f});
    component::drawable playerDrawable;
    playerDrawable.shape.setSize(sf::Vector2f(50.f, 50.f));
    playerDrawable.shape.setFillColor(sf::Color::Green);
    reg.add_component(missile, std::move(playerDrawable));
    // reg.add_component(player, component::hitbox{50.f, 50.f});
    reg.add_component(missile, component::missile{400.0, 400.0});
}

namespace systems {

void control(registry &reg)
{
    auto &velocities = reg.get_components<component::velocity>();
    auto &controllables = reg.get_components<component::controllable>();

    sf::Vector2f direction(0.f, 0.f);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        spawn_missile(reg);
    }
    float speed = 100.0f;

    zipper<component::velocity, component::controllable> zip(velocities, controllables);

    for (auto [vel, _] : zip) {
        vel.vx = direction.x * speed;
        vel.vy = direction.y * speed;
    }
}

} // namespace systems
