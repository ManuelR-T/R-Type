/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** control_move
*/

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "InputManager.hpp"
#include "Registry.hpp"
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/velocity.hpp"
#include "control_move.hpp"

namespace ecs::systems {

void controlMove(Registry &reg, ecs::InputManager &input)
{
    auto &velocities = reg.getComponents<ecs::component::Velocity>();
    auto &controllables = reg.getComponents<ecs::component::Controllable>();

    sf::Vector2f direction(0.f, 0.f);
    if (input.isKeyPressed(sf::Keyboard::Up)) {
        direction.y -= 1.f;
    }
    if (input.isKeyPressed(sf::Keyboard::Down)) {
        direction.y += 1.f;
    }
    if (input.isKeyPressed(sf::Keyboard::Left)) {
        direction.x -= 1.f;
    }
    if (input.isKeyPressed(sf::Keyboard::Right)) {
        direction.x += 1.f;
    }

    float speed = 100.0f;

    Zipper<ecs::component::Velocity, ecs::component::Controllable> zip(velocities, controllables);

    for (auto [vel, _] : zip) {
        vel.vx = direction.x * speed;
        vel.vy = direction.y * speed;
    }
}

} // namespace ecs::systems
