/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** staticBoxCreation
*/

#include "core/constants.hpp"
#include "lobby.hpp"

void Cli::Lobby::createStaticVisibleBox(float screenWidth, float screenHeight)
{
    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.15f * screenWidth, 0.07f * screenHeight},
        sf::Vector2f{0.7f * screenWidth, 0.77f * screenHeight},
        sf::Color{255, 0, 0, 255},
        VISIBLE_CREATE
    ));
    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.15f * screenWidth, 0.07f * screenHeight},
        sf::Vector2f{0.2f * screenWidth, 0.77f * screenHeight},
        sf::Color{255, 0, 0, 255},
        VISIBLE_READY
    ));
    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.9f * screenWidth, 0.9f * screenHeight},
        sf::Vector2f{0.05f * screenWidth, 0.05f * screenHeight},
        sf::Color{122, 122, 122, 0},
        VISIBLE
    ));

    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.8f * screenWidth, 0.8f * screenHeight},
        sf::Vector2f{0.1f * screenWidth, 0.1f * screenHeight},
        sf::Color{122, 122, 122, 0},
        VISIBLE
    ));
}

void Cli::Lobby::createUnseenStaticBox(float screenWidth, float screenHeight)
{
    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.9f * screenWidth, 0.043f * screenHeight},
        sf::Vector2f{0.05f * screenWidth, 0 * screenHeight},
        sf::Color{0, 0, 0, 255},
        NOT_VISIBLE
    ));

    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.9f * screenWidth, 0.045f * screenHeight},
        sf::Vector2f{0.05f * screenWidth, 0.05f * screenHeight},
        sf::Color{0, 0, 0, 255},
        NOT_VISIBLE
    ));

    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.9f * screenWidth, 0.045f * screenHeight},
        sf::Vector2f{0.05f * screenWidth, 0.905f * screenHeight},
        sf::Color{0, 0, 0, 255},
        NOT_VISIBLE
    ));

    _staticBoxes.push_back(createStaticBox(
        sf::Vector2f{0.9f * screenWidth, 0.05f * screenHeight},
        sf::Vector2f{0.07f * screenWidth, 0.957f * screenHeight},
        sf::Color{0, 0, 0, 255},
        NOT_VISIBLE
    ));
}

Cli::Lobby::StaticBox Cli::Lobby::createStaticBox(
    const sf::Vector2f &size,
    const sf::Vector2f &pos,
    const sf::Color &color,
    const BoxType &boxType
)
{
    StaticBox box;

    box.rectangle.setSize(size);
    box.rectangle.setPosition(pos);
    box.rectangle.setFillColor(color);
    box.rectangle.setOutlineColor(sf::Color(122, 122, 122));
    if (boxType != NOT_VISIBLE) {
        box.rectangle.setOutlineThickness(5);
    }
    box.type = boxType;
    return box;
}
