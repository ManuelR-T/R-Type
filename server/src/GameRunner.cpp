/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** game_runner
*/

#include <cstddef>
#include <string>

#include "GameRunner.hpp"
#include "IndexedZipper.hpp"
#include "Logger.hpp"
#include "RTypeServer.hpp"
#include "RTypeUDPProtol.hpp"
#include "Registry.hpp"
#include "components/playerId.hpp"
#include "components/shared_entity.hpp"

using namespace ecs;

rts::GameRunner::GameRunner(int port, std::size_t stage) // ! Use the stage argument
    : _port(port), _udpServer(port),
      _responseHandler([](const rt::UDPClientPacket &packet) { return packet.header.cmd; })
{
    eng::logWarning("Selected stage: " + std::to_string(stage) + ".");

    rts::registerUdpResponse(_responseHandler, _datasToSend, _networkCallbacks);
    _udpServer.registerCommand([this](char *data, std::size_t size) {
        this->_responseHandler.handleResponse(data, size);
    });
    _udpServer.run();

    rts::registerComponents(_reg);
    rts::registerSystems(
        _reg, _window, _dt, _tickRateManager, _udpServer, _datasToSend, _networkCallbacks, _waveManager
    );
    rts::init_waves(_waveManager, _datasToSend);
}

void rts::GameRunner::killPlayer(size_t playerId)
{
    _networkCallbacks.push_back([playerId, this](ecs::Registry &reg) {
        ecs::IndexedZipper<component::Player, component::SharedEntity> zip(
            reg.getComponents<component::Player>(), reg.getComponents<component::SharedEntity>()
        );

        for (auto [e, player, shared] : zip) {
            if (player.playerId == playerId) {
                _datasToSend.push_back(rt::UDPServerPacket(
                    {.header = {.cmd = rt::UDPCommand::DEL_ENTITY}, .body = {.sharedEntityId = shared.sharedEntityId}}
                ));
                reg.killEntity(e);
                return;
            }
        }
    });
}

void rts::GameRunner::addWindow(sf::VideoMode &&videomode, const std::string &title)
{
    _window.create(videomode, title);
    _window.setFramerateLimit(60); // ! for debug
}

void rts::GameRunner::runGame(bool &stopGame)
{
    sf::Clock clock;

    while (_window.isOpen() && !stopGame) {
        _dt = clock.restart().asSeconds();

        // ! for debug
        sf::Event event;
        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                _window.close();
            }
        }
        // ! for debug
        _reg.runSystems();
    }
    if (_window.isOpen()) {
        _window.close();
    }
}
