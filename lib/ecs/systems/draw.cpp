/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** draw
*/

#include "../components/drawable.hpp"
#include "../components/position.hpp"
#include "../core/registry.hpp"
#include "../core/zipper.hpp"
#include <SFML/Graphics.hpp>

namespace systems {

void draw(registry &reg, sf::RenderWindow &window) {
  auto &positions = reg.get_components<component::position>();
  auto &drawables = reg.get_components<component::drawable>();

  zipper<component::position, component::drawable> zip(positions, drawables);

  for (auto [pos, draw] : zip) {
    draw.shape.setPosition(pos.x, pos.y);
    window.draw(draw.shape);
  }
}

} // namespace systems