/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** control
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../components/controllable.hpp"
#include "../components/velocity.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"

namespace systems {

void control(registry &reg)
{
    auto &velocities = reg.get_components<ecs::component::velocity>();
    auto &controllables = reg.get_components<ecs::component::controllable>();

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

    float speed = 100.0f;

    zipper<ecs::component::velocity, ecs::component::controllable> zip(velocities, controllables);

    for (auto [vel, _] : zip) {
        vel.vx = direction.x * speed;
        vel.vy = direction.y * speed;
    }
}

} // namespace systems
