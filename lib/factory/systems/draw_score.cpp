/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_score
*/

#include "draw_score.hpp"
#include <cmath>
#include "Zipper.hpp"
#include "components/score.hpp"
#include "imgui.h"
#include "components/self_player.hpp"
#include "imgui-SFML.h"

void ecs::systems::drawScore(Registry &reg, sf::RenderWindow &window, const sf::Vector2u &windowSize)
{
    auto &score = reg.getComponents<ecs::component::Score>();
    auto &selfPlayer = reg.getComponents<ecs::component::SelfPlayer>();

    Zipper<ecs::component::Score, ecs::component::SelfPlayer> zip(score, selfPlayer);

    for (auto [score, _] : zip) {
        if (score.font != nullptr) {
            ImGui::GetBackgroundDrawList()->AddText(
                score.font.get(),
                (windowSize.y * 0.05),
                ImVec2(windowSize.x * 0.85, windowSize.y * 0.05),
                IM_COL32(255, 255, 255, 255),
                std::to_string(static_cast<int>(std::stof(score.text))).c_str()
            );
        }
    }
}
