/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** control
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "components/controllable.hpp"
#include "components/velocity.hpp"
#include "control.hpp"
#include "core/registry.hpp"
#include "core/zipper.hpp"

namespace ecs::systems {

void control(registry &reg, ecs::input_manager &input)
{
    auto &velocities = reg.get_components<ecs::component::velocity>();
    auto &controllables = reg.get_components<ecs::component::controllable>();

    sf::Vector2f direction(0.f, 0.f);
    if (input.is_key_pressed(sf::Keyboard::Up)) {
        direction.y -= 1.f;
    }
    if (input.is_key_pressed(sf::Keyboard::Down)) {
        direction.y += 1.f;
    }
    if (input.is_key_pressed(sf::Keyboard::Left)) {
        direction.x -= 1.f;
    }
    if (input.is_key_pressed(sf::Keyboard::Right)) {
        direction.x += 1.f;
    }

    float speed = 100.0f;

    zipper<ecs::component::velocity, ecs::component::controllable> zip(velocities, controllables);

    for (auto [vel, _] : zip) {
        vel.vx = direction.x * speed;
        vel.vy = direction.y * speed;
    }
}

} // namespace ecs::systems
