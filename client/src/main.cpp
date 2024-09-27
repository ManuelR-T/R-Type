/*
** EPITECH PROJECT, 2024
** R-type
** File description:
** main
*/

#include <SFML/Graphics.hpp>
#include <memory>
#include <thread>
#include "GameProtocol.hpp"
#include "UDPClient.hpp"
#include "argParser.hpp"
#include "components/animation.hpp"
#include "components/controllable.hpp"
#include "components/drawable.hpp"
#include "components/hitbox.hpp"
#include "components/missile.hpp"
#include "components/position.hpp"
#include "components/sprite.hpp"
#include "components/velocity.hpp"
#include "core/SpriteManager.hpp"
#include "core/constants.hpp"
#include "core/entity.hpp"
#include "core/registry.hpp"
#include "systems/collision.hpp"
#include "systems/draw.hpp"
#include "systems/position.hpp"
#include "components/ai_actor.hpp"
#include "components/share_movement.hpp"
#include "components/shared_entity.hpp"
#include "core/input_manager.hpp"
#include "core/shared_entity.hpp"
#include "core/tick_rate_manager.hpp"
#include "game_manager.hpp"
#include "my_log.hpp"
#include "rtype_client.hpp"
#include "systems/ai_act.hpp"
#include "systems/control_move.hpp"
#include "systems/control_special.hpp"
#include "systems/missiles_stop.hpp"
#include "systems/share_movement.hpp"
#include "systems/sprite_system.hpp"

void rtc::run(ecs::registry &reg, const std::shared_ptr<sf::RenderWindow> &window, float &dt, ecs::input_manager &input)
{
    sf::Clock clock;

    while (window->isOpen()) {
        dt = clock.restart().asSeconds();

        sf::Event event;
        while (window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window->close();
            }
            input.update(event);
        }
        reg.run_systems();
    }
}

int main(int argc, const char *argv[])
{
    ArgParser argParser;
    argParser.addArgument("ip", "i", ArgParser::ArgType::STRING, true, "Server IP address");
    argParser.addArgument("port", "p", ArgParser::ArgType::INT, true, "Server port");
    argParser.addArgument("player_name", "pn", ArgParser::ArgType::STRING, true, "Player name");
    argParser.addArgument("help", "h", ArgParser::ArgType::BOOL, false, "Print this help message");

    if (!argParser.parse(argc, argv)) {
        argParser.printHelp();
        return 84;
    }
    if (argParser.getValue<bool>("help")) {
        argParser.printHelp();
        return 0;
    }

    auto ip = argParser.getValue<std::string>("ip");
    auto port = argParser.getValue<int>("port");
    auto player_name = argParser.getValue<std::string>("player_name");

    rtc::game_manager game(ip, port, player_name);

    game.run_game();
    return 0;
}
