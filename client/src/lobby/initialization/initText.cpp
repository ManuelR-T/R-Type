/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** createBox
*/

#include "core/constants.hpp"
#include "lobby.hpp"

static void initializeText(
    sf::Text &text,
    const sf::Font &font,
    int characterSize,
    const std::string &content,
    const sf::Vector2f &position
)
{
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setString(content);
    text.setPosition(position);
}

void Cli::Lobby::initializeTextBox(MovableBox &box, const sf::Vector2f &pos)
{
    const sf::Vector2f roomNamePosition = {
        pos.x + ecs::constants::screen_width * 0.04f, pos.y + ecs::constants::screen_height * 0.02f
    };
    const sf::Vector2f nbPlayersPosition = {
        pos.x + ecs::constants::screen_width * 0.17f, pos.y + ecs::constants::screen_height * 0.02f
    };
    const sf::Vector2f deleteNamePosition = {
        pos.x + ecs::constants::screen_width * 0.65f, pos.y + ecs::constants::screen_height * 0.02f
    };

    initializeText(box.roomName, _font, 30, "Room " + std::to_string(_nbRoom++), roomNamePosition);
    initializeText(box.nbPlayers, _font, 30, "0/4", nbPlayersPosition);
    initializeText(box.deleteName, _font, 30, "Delete", deleteNamePosition);
    box.deleteButton.setPosition(deleteNamePosition);
}
