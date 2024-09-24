/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** playersList
*/

#include "core/constants.hpp"
#include "lobby.hpp"

void Cli::Lobby::createPlayerLine(const std::string &namePlayer, const bool &isReady)
{
    Player player;

    const std::size_t sizePlayerList = _playersList.size() + 1;
    sf::Vector2f pos = {ecs::constants::screen_width * 0.1f, (ecs::constants::screen_width * 0.1f) * sizePlayerList};

    player.rectangle.setPosition(pos);
    player.rectangle.setSize(sf::Vector2f{ecs::constants::screen_width * 0.8, ecs::constants::screen_height * 0.1});
    player.rectangle.setFillColor(sf::Color{0, 0, 0, 0});
    player.rectangle.setOutlineColor(sf::Color{122, 122, 122, 255});
    player.rectangle.setOutlineThickness(5);

    player.nameText.setString(namePlayer);
    player.nameText.setCharacterSize(10);
    player.nameText.setFont(_font);
    player.nameText.setPosition(sf::Vector2f{pos.x + 50, pos.y + 30});

    player.isReadyText.setString(player.isReady ? "Ready" : "Not Ready");
    player.isReadyText.setCharacterSize(10);
    player.isReadyText.setFont(_font);
    player.isReadyText.setPosition(sf::Vector2f{pos.x + 250, pos.y + 30});

    _playersList.push_back(player);
}

void Cli::Lobby::fetchListPlayerFromRoom()
{
    // ! Fetch nb of player in the list

    createPlayerLine("Jean", true);
    createPlayerLine("Marc", false);
    createPlayerLine("Henri", false);
    createPlayerLine("Jeanne", true);
}
