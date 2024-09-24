/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** movableBoxCreation
*/

#include "core/constants.hpp"
#include "lobby.hpp"

Cli::Lobby::MovableBox Cli::Lobby::createMovableBox(
    const sf::Vector2f &size,
    const sf::Vector2f &pos,
    const sf::Color &color
)
{
    MovableBox box;

    box.rectangle.setSize(size);
    box.rectangle.setPosition(pos);
    box.rectangle.setFillColor(color);
    box.rectangle.setOutlineColor(sf::Color(122, 122, 122));
    box.rectangle.setOutlineThickness(5);
    initializeTextBox(box, pos);
    box.deleteButton.setSize(
        sf::Vector2f{(int)(ecs::constants::screen_width * 0.1), (int)(ecs::constants::screen_height * 0.05)}
    );
    box.deleteButton.setFillColor(color);
    box.deleteButton.setOutlineColor(sf::Color(122, 122, 122));
    box.deleteButton.setOutlineThickness(5);
    return box;
}
