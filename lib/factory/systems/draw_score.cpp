/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_score
*/

#include "draw_score.hpp"
#include <iostream>
#include "Zipper.hpp"
#include "components/controllable.hpp"
#include "components/score.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

void ecs::systems::drawScore(Registry &reg, sf::RenderWindow &window, const sf::Vector2u &windowSize)
{
    auto &scores = reg.getComponents<ecs::component::Score>();

    Zipper<ecs::component::Score> zip(scores);

    for (auto [score] : zip) {
        score.text.setFont(score.font);
        score.text.setPosition(sf::Vector2f(windowSize.x * 0.9, windowSize.y * 0.1));
        const sf::Vector2f posText = score.text.getPosition();
        score.text.setCharacterSize(windowSize.y * 0.05);
        score.text.setString(std::to_string(score.value));
        window.draw(score.text);
    }
}
