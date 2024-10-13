/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#include "draw_player_beam_bar.hpp"
#include "Zipper.hpp"
#include "components/beam.hpp"
#include "imgui.h"

static void drawBar(int percentageBar, const sf::Vector2u &windowSize)
{
    sf::Vector2u pos =
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.32f), static_cast<unsigned int>(windowSize.y * 0.95f)};
    sf::Vector2u size =
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.4f), static_cast<unsigned int>(windowSize.y * 0.02f)};

    ImVec2 rect1Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect1Size = ImVec2(size.x, size.y);
    ImVec2 rect2Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect2Size = ImVec2(size.x * (percentageBar / 100.0f), size.y);

    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect1Pos, ImVec2(rect1Pos.x + rect1Size.x, rect1Pos.y + rect1Size.y), IM_COL32(102, 178, 255, 255), 10.f
    );
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect2Pos, ImVec2(rect2Pos.x + rect2Size.x, rect2Pos.y + rect2Size.y), IM_COL32(0, 0, 204, 255), 10.f
    );
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(rect2Pos.x, rect2Pos.y - 20), IM_COL32(255, 255, 255, 255), "Beam");
}

void ecs::systems::drawPlayerBeamBar(Registry &reg, const sf::Vector2u &windowSize)
{
    auto &beams = reg.getComponents<ecs::component::Beam>();

    Zipper<ecs::component::Beam> zip(beams);

    for (auto [beam] : zip) {
        drawBar(beam.power, windowSize);
    }
}
