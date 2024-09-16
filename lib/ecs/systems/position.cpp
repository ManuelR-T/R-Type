/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** position
*/

#include "../components/position.hpp"
#include "../components/velocity.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"

namespace systems {

void position(registry &reg, float dt) {
  auto &positions = reg.get_components<component::position>();
  auto &velocities = reg.get_components<component::velocity>();

  zipper<component::position, component::velocity> zip(positions, velocities);

  for (auto [pos, vel] : zip) {
    pos.x += vel.vx * dt;
    pos.y += vel.vy * dt;
  }
}

} // namespace systems
