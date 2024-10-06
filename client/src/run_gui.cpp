/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** run_gui
*/

#include <SFML/System/Vector2.hpp>
#include <cstring>
#include "RTypeClient.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

static void renderInsideRoom(const std::string &name, rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    // ! Window
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin(
        name.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
    );

    // ! Table
    ImGui::BeginTable("playersTable", 2);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Status");
    ImGui::TableHeadersRow();
    ImU32 selfRowColor = ImGui::GetColorU32(ImVec4(0.5, 1, 0.5, 0.25));

    // ! Action with table
    for (const auto &[id, player] : roomManager.getCurrentRoomPlayer()) {
        ImGui::TableNextRow();
        if (id == roomManager.getSelfId()) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, selfRowColor);
        }
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%s", player.name.c_str());
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", player.ready ? "ready" : "not ready");
    }
    ImGui::EndTable();

    // ! Back to lobby button
    {
        ImVec2 buttonSize(200, 50);
        ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
        ImVec2 buttonPos =
            ImVec2(windowContentRegionMax.x - buttonSize.x - 20, windowContentRegionMax.y - buttonSize.y - 20);
        ImGui::SetCursorPos(buttonPos);
        if (ImGui::Button("Back to lobby", buttonSize)) {
            // ! send leave
            roomManager.askToLeaveRoom();
        }
    }

    // ! ready button
    ImVec2 buttonSize(200, 50);
    ImVec2 windowContentRegionMax = ImGui::GetWindowContentRegionMax();
    ImVec2 buttonPos = ImVec2(20, windowContentRegionMax.y - buttonSize.y - 20);
    ImGui::SetCursorPos(buttonPos);
    if (!roomManager.getCurrentRoomPlayer().at(roomManager.getSelfId()).ready) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
        if (ImGui::Button("Ready", buttonSize)) {
            roomManager.askToBeReady();
        }
    } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
        if (ImGui::Button("Not Ready", buttonSize)) {
            roomManager.askToBeNotReady();
        }
    }
    ImGui::PopStyleColor(3);

    ImGui::End();
}

void rtc::runGui(const std::shared_ptr<sf::RenderWindow> &window, rtc::RoomManager &roomManager, bool &inLobby)
{
    if (!ImGui::SFML::Init(*window)) {
        throw std::runtime_error("IMGUI Window init failed");
    }
    sf::Clock dt;

    while (window->isOpen() && inLobby) {
        sf::Event event;
        while (window->pollEvent(event)) {
            ImGui::SFML::ProcessEvent(*window, event);
            if (event.type == sf::Event::Closed) {
                window->close();
            }
        }
        ImGui::SFML::Update(*window, dt.restart());
        window->clear();

        if (roomManager.getCurrentRoom().empty()) {
            rtc::renderLobbyWindow(roomManager, window->getSize());
        } else {
            renderInsideRoom(roomManager.getCurrentRoom(), roomManager, window->getSize());
        }

        ImGui::SFML::Render(*window);
        window->display();
    }
}
