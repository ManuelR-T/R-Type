/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** init
*/

#include "core/constants.hpp"
#include "lobby.hpp"
#include "my_tracked_exception.hpp"

void Cli::Lobby::initLobby()
{
    if (!_font.loadFromFile("./assets/font/DroidSansMono.ttf")) {
        throw my::tracked_exception("Failed during loading of the font!\n");
    }
    addComponentLobby();
}

void Cli::Lobby::addComponentLobby()
{
    float screenWidth = ecs::constants::screen_width;
    float screenHeight = ecs::constants::screen_height;

    _movableBoxes.push_back(createMovableBox(
        sf::Vector2f{0.8f * screenWidth, 0.1f * screenHeight},
        sf::Vector2f{0.1f * screenWidth, 0.1f * screenHeight},
        sf::Color{122, 122, 122, 0}
    ));

    createStaticVisibleBox(screenWidth, screenHeight);
    createUnseenStaticBox(screenWidth, screenHeight);
}
