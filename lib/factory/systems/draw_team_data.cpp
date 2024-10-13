/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** draw_player_beam_bar
*/

#include "draw_team_data.hpp"
#include <iostream>
#include "Zipper.hpp"
#include "components/beam.hpp"
#include "components/controllable.hpp"
#include "components/health.hpp"
#include "imgui-SFML.h"

static void drawBar(
    const std::string &componentType,
    int percentageBar,
    const sf::Vector2u &pos,
    const sf::Vector2u &windowSize
)
{
    sf::Vector2u size =
        sf::Vector2u{static_cast<unsigned int>(windowSize.x * 0.1f), static_cast<unsigned int>(windowSize.y * 0.01f)};
    ImU32 main, background;

    if (componentType == "health") {
        main = IM_COL32(0, 153, 0, 255);
        background = IM_COL32(0, 153, 0, 255);
    } else if (componentType == "beam") {
        main = IM_COL32(102, 178, 255, 255);
        background = IM_COL32(0, 0, 204, 255);
    }

    ImVec2 rect1Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect1Size = ImVec2(size.x, size.y);
    ImVec2 rect2Pos = ImVec2(pos.x, pos.y);
    ImVec2 rect2Size = ImVec2(size.x * (percentageBar / 100.0f), size.y);

    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect1Pos, ImVec2(rect1Pos.x + rect1Size.x, rect1Pos.y + rect1Size.y), main, 10.f
    );
    ImGui::GetBackgroundDrawList()->AddRectFilled(
        rect2Pos, ImVec2(rect2Pos.x + rect2Size.x, rect2Pos.y + rect2Size.y), background, 10.f
    );
}

// => !reg.hasComponent<component::Parallax>(i)

void ecs::systems::drawTeamData(Registry &reg, const sf::Vector2u &windowSize)
{
    auto &healths = reg.getComponents<ecs::component::Health>();
    auto &beams = reg.getComponents<ecs::component::Beam>();

    Zipper<ecs::component::Health, ecs::component::Beam> zip(healths, beams);

    int i = 0;
    for (auto [health, beam] : zip) {
        sf::Vector2u pos = {
            static_cast<unsigned int>(windowSize.x * 0.01f),
            static_cast<unsigned int>((windowSize.y * 0.13f) + (i * (windowSize.y * 0.08)))
        };
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(pos.x, pos.y - 20), IM_COL32(255, 255, 255, 255), "PLAYER NAME");
        drawBar("health", health.currHp, pos, windowSize);
        drawBar(
            "beam", beam.power, sf::Vector2u{pos.x, pos.y + static_cast<unsigned int>(windowSize.y * 0.03)}, windowSize
        );
        ++i;
    }
}
