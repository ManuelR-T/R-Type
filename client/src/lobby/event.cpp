/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** event
*/

#include "core/constants.hpp"
#include "lobby.hpp"

void Cli::Lobby::addNewRoom()
{
    const sf::Vector2f posLastBox = _movableBoxes.back().rectangle.getPosition();

    // ! Create new room
    _movableBoxes.push_back(createMovableBox(
        sf::Vector2f{0.8f * ecs::constants::screen_width, 0.1f * ecs::constants::screen_height},
        sf::Vector2f{0.1f * ecs::constants::screen_width, posLastBox.y + 0.1f * ecs::constants::screen_height},
        sf::Color{122, 122, 122, 0}
    ));
}

void Cli::Lobby::removeRoom(const std::size_t &indexRoom)
{
    // ! Delete a room with his index
    _movableBoxes.erase(_movableBoxes.begin() + indexRoom);
}

void Cli::Lobby::updateBoxesPosition(MovableBox &box, const sf::Vector2f &pos, const float &offset = 0)
{
    box.roomName.setPosition(sf::Vector2f{
        pos.x + (int)(ecs::constants::screen_width * 0.04), pos.y + int(ecs::constants::screen_height * 0.02) + offset
    });
    box.nbPlayers.setPosition(sf::Vector2f{
        pos.x + (int)(ecs::constants::screen_width * 0.17), pos.y + int(ecs::constants::screen_height * 0.02) + offset
    });
    box.deleteName.setPosition(sf::Vector2f{
        pos.x + (int)(ecs::constants::screen_width * 0.65), pos.y + int(ecs::constants::screen_height * 0.02) + offset
    });
    box.deleteButton.setPosition(sf::Vector2f{
        pos.x + (int)(ecs::constants::screen_width * 0.65), pos.y + int(ecs::constants::screen_height * 0.02) + offset
    });
}

void Cli::Lobby::moveLeftRooms(const std::size_t &startIdxRoom)
{
    for (int i = startIdxRoom + 1; i < _movableBoxes.size(); ++i) {
        sf::Vector2f pos = _movableBoxes.at(i).rectangle.getPosition();
        _movableBoxes.at(i).rectangle.setPosition(sf::Vector2f{pos.x, pos.y - int(ecs::constants::screen_height * 0.1)}
        );
        updateBoxesPosition(_movableBoxes.at(i), _movableBoxes.at(i).rectangle.getPosition());
    }
}

void Cli::Lobby::boxScrolling(const float &delta)
{
    const float offset = (delta >= 0) ? 10.0f : -10.0f;

    for (auto &box : _movableBoxes) {
        sf::Vector2f pos = box.rectangle.getPosition();
        box.rectangle.setPosition(sf::Vector2f{pos.x, pos.y + offset});
        updateBoxesPosition(box, pos, offset);
    }
}
