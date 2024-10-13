/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_score
*/

#include "draw_score.hpp"
#include <cmath>
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
        if (score.font != nullptr) {
            ImGui::GetBackgroundDrawList()->AddText(
                score.font,
                (windowSize.y * 0.05),
                ImVec2(windowSize.x * 0.85, windowSize.y * 0.05),
                IM_COL32(255, 255, 255, 255),
                std::to_string(static_cast<int>(std::stof(score.text))).c_str()
            );
        }
    }
}
