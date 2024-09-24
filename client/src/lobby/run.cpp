/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** init
*/

#include "lobby.hpp"
#include <iostream>

static bool isMouseOnBox(const sf::RectangleShape &rectangle, const sf::Vector2i &mousePos)
{
    const sf::Vector2f rectanglePos = rectangle.getPosition();
    const sf::Vector2f rectangleSize = rectangle.getSize();

    float boxLeft = rectanglePos.x;
    float boxRight = rectanglePos.x + rectangleSize.x;
    float boxTop = rectanglePos.y;
    float boxBottom = rectanglePos.y + rectangleSize.y;

    return (mousePos.x >= boxLeft && mousePos.x <= boxRight && mousePos.y >= boxTop && mousePos.y <= boxBottom);
}

void Cli::Lobby::runLobby(sf::RenderWindow &window)
{
    bool isInPlayerList = false;

    while (window.isOpen()) {
        processEvents(window, isInPlayerList);
        render(window, isInPlayerList);
    }
}

void Cli::Lobby::processEvents(sf::RenderWindow &window, bool &isInPlayerList)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        handleWindowCloseEvent(event, window);
        handleMouseScrollEvent(event);
        handleMouseClickEvent(event, window, isInPlayerList);
    }
}

void Cli::Lobby::handleWindowCloseEvent(const sf::Event &event, sf::RenderWindow &window)
{
    if (event.type == sf::Event::Closed ||
        (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
        window.close();
    }
}

void Cli::Lobby::handleMouseScrollEvent(const sf::Event &event)
{
    if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        boxScrolling(event.mouseWheelScroll.delta);
    }
}

void Cli::Lobby::handleMouseClickEvent(const sf::Event &event, sf::RenderWindow &window, bool &isInPlayerList)
{
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        handleStaticBoxClick(mousePos, isInPlayerList);
        handleMovableBoxClick(mousePos, isInPlayerList);
    }
}

void Cli::Lobby::handleStaticBoxClick(const sf::Vector2i &mousePos, bool &isInPlayerList)
{
    for (const auto &box : _staticBoxes) {
        if (box.type == VISIBLE_CREATE && isMouseOnBox(box.rectangle, mousePos)) {
            if (isInPlayerList) {
                isInPlayerList = false;
                _playersList.clear();
            } else {
                addNewRoom();
            }
        } else if (box.type == VISIBLE_READY && isMouseOnBox(box.rectangle, mousePos)) {
            std::cout << "SET TO READY\n";
        }
    }
}

void Cli::Lobby::handleMovableBoxClick(const sf::Vector2i &mousePos, bool &isInPlayerList)
{
    for (int i = 0; i < _movableBoxes.size(); ++i) {
        if (isMouseOnBox(_movableBoxes.at(i).deleteButton, mousePos)) {
            moveLeftRooms(i);
            removeRoom(i);
        } else if (isMouseOnBox(_movableBoxes.at(i).rectangle, mousePos) && !isInPlayerList) {
            fetchListPlayerFromRoom();
            isInPlayerList = true;
        }
    }
}

void Cli::Lobby::render(sf::RenderWindow &window, bool isInPlayerList)
{
    window.clear(sf::Color::Black);

    if (isInPlayerList) {
        renderPlayerList(window);
    } else {
        renderMovableBoxes(window);
    }

    renderStaticBoxes(window, isInPlayerList);
    window.display();
}

void Cli::Lobby::renderPlayerList(sf::RenderWindow &window)
{
    for (const auto &player : _playersList) {
        window.draw(player.rectangle);
        window.draw(player.nameText);
        window.draw(player.isReadyText);
    }
}

void Cli::Lobby::renderMovableBoxes(sf::RenderWindow &window)
{
    for (const auto &box : _movableBoxes) {
        window.draw(box.rectangle);
        window.draw(box.roomName);
        window.draw(box.nbPlayers);
        window.draw(box.deleteName);
        window.draw(box.deleteButton);
    }
}

void Cli::Lobby::renderStaticBoxes(sf::RenderWindow &window, bool isInPlayerList)
{
    for (const auto &box : _staticBoxes) {
        if (box.type == VISIBLE_READY) {
            if (isInPlayerList) {
                window.draw(box.rectangle);
            }
        } else {
            window.draw(box.rectangle);
        }
    }
}
