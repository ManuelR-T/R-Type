/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <cstring>
#include <memory>
#include <string>
#include "Logger.hpp"
#include "RTypeClient.hpp"
#include "RTypeConst.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/System/Time.hpp"
#include "imgui-SFML.h"

void rtc::runGui(
    const std::shared_ptr<sf::RenderWindow> &window,
    const std::shared_ptr<rtc::RoomManager> &roomManager,
    std::atomic<rtc::GameState> &gameState,
    ecs::KeyBind<rt::PlayerAction, sf::Keyboard::Key> &keyBind,
    const std::shared_ptr<sf::Shader> &colorBlind
)
{
    ecs::SoundManager soundManager;
    sf::Clock dt;
    sf::Vector2u windowSize = window->getSize();
    WindowMode windowMode = rtc::WindowMode::MENU;
    int fpsLimit = rt::CLIENT_FPS_LIMIT;
    bool chatEnable = false;
    bool scoreBoardEnable = false;
    sf::Texture texture;
    if (!texture.loadFromFile("assets/menu/background.jpg")) {
        eng::logError("Failed to load background image !");
    }
    sf::Sprite background(texture);
    background.setScale(rt::SCREEN_WIDTH / float(texture.getSize().x), rt::SCREEN_HEIGHT / float(texture.getSize().y));

    while (window->isOpen() && gameState.load() == GameState::LOBBY) {
        sf::Time timeDt = dt.restart();
        sf::Event event{};
        if (timeDt.asSeconds() <= 0) {
            timeDt = sf::milliseconds(1);
        }
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            if (event.type == sf::Event::Closed) {
                gameState.store(GameState::NONE);
                window->close();
            } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RControl) {
                chatEnable = !chatEnable;
            } else if (event.type == sf::Event::Resized) {
                windowSize = window->getSize();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                soundManager.loadSoundBuffer("ui_click", "assets/uiClick.wav");
                soundManager.playSoundEffect("ui_click", 100.f, false);
            }
        }
        ImGui::SFML::Update(*window, timeDt);
        window->clear();
        window->draw(background, colorBlind.get());
        switch (windowMode) {
            case rtc::WindowMode::EXIT_MENU:
                window->close();
                break;
            case rtc::WindowMode::MENU:
                menuWindow(windowSize, windowMode);
                break;
            case rtc::WindowMode::OPTIONS:
                optionsWindow(*window, windowSize, fpsLimit, windowMode, colorBlind);
                break;
            case rtc::WindowMode::ACCESSIBILITY:
                renderAccessibility(windowSize, windowMode, keyBind);
                break;
            case rtc::WindowMode::LOBBY:
                lobbyWindow(windowSize, roomManager, scoreBoardEnable);
                break;
        }
        renderChat(roomManager, windowSize, chatEnable);
        renderScoreBoard(windowSize, scoreBoardEnable, sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.7f), 0});
        ImGui::SFML::Render(*window);
        window->display();
    }
}
