/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_selection
*/

#include <cstddef>
#include <string>
#include "../RTypeClient.hpp"
#include "../RoomManager.hpp"
#include "RTypeConst.hpp"
#include "imgui.h"

static void renderRoomTable(rtc::RoomManager &roomManager)
{
    ImGui::BeginTable("table", 4);
    ImGui::TableSetupColumn("Name");
    ImGui::TableSetupColumn("Stage");
    ImGui::TableSetupColumn("Number of players");
    ImGui::TableSetupColumn("Actions");
    ImGui::TableHeadersRow();
    ImU32 inGameRowColor = ImGui::GetColorU32(ImVec4(1, 0, 0, 0.25));

    for (const auto &[room_name, room_data] : roomManager.getRooms()) {
        ImGui::TableNextRow();
        if (!room_data.joinable) {
            ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, inGameRowColor);
        }
        ImGui::TableSetColumnIndex(0);
        if (ImGui::Button(room_name.c_str()) && room_data.joinable) {
            roomManager.askToJoinRoom(room_name);
        }
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%zu", room_data.stage);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%zu / 4", room_data.player.size());
        ImGui::TableSetColumnIndex(3);
        if (ImGui::Button((std::string("Delete##") + room_name).c_str()) && room_data.joinable &&
            room_data.player.empty()) {
            roomManager.askToDeleteRoom(room_name);
        }
    }
    ImGui::EndTable();
}

static void renderStageButton(const ImVec2 &currentRegion, const ImVec2 &currentCursor, std::size_t &selectedStage)
{
    ImGui::SetCursorPosX(currentCursor.x + 10);
    ImGui::Text("Select Stage:");
    ImGui::SetCursorPosX(currentCursor.x + 10);

    ImVec2 buttonSize(
        ((currentRegion.x * 0.5) - (ImGui::GetStyle().ItemSpacing.x * (rt::NB_STAGE - 1))) / rt::NB_STAGE,
        ImGui::GetContentRegionAvail().y * 0.8
    );
    ImVec4 buttonColor(0.2f, 0.2f, 0.2f, 1.0f);
    ImVec4 selectedColor(0.0f, 0.7f, 1.0f, 1.0f);
    ImVec4 hoveredColor(0.4f, 0.4f, 0.4f, 1.0f);
    ImVec4 selectedHoveredColor(0.0f, 0.5f, 0.8f, 1.0f);

    for (std::size_t i = 1; i <= rt::NB_STAGE; ++i) {
        ImGui::PushStyleColor(ImGuiCol_Button, (selectedStage == i) ? selectedColor : buttonColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (selectedStage == i) ? selectedHoveredColor : hoveredColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (selectedStage == i) ? selectedHoveredColor : hoveredColor);

        if (ImGui::Button(std::to_string(i).c_str(), buttonSize)) {
            selectedStage = i;
        }
        ImGui::PopStyleColor(3);
        if (i < rt::NB_STAGE) {
            ImGui::SameLine();
        }
    }
}

static void renderRoomCreation(rtc::RoomManager &roomManager)
{
    static char roomName[rt::MAX_ROOM_NAME_SIZE + 1] = {0};
    static std::size_t selectedStage = 1;
    bool isInputNotEmpty = roomName[0];
    bool canCreateRoom = roomName[0] && !std::isspace(roomName[0]) && !roomManager.getRooms().contains(roomName);
    ImVec2 currentRegion = ImGui::GetContentRegionAvail();
    ImVec2 currentCursor = ImGui::GetCursorPos();

    if (!canCreateRoom && isInputNotEmpty) {
        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    }
    ImGui::SetCursorPos(ImVec2(currentCursor.x + 10, currentCursor.y + (currentRegion.y * 0.1)));
    ImGui::Text("Enter Room Name:");
    ImGui::SetCursorPos(ImVec2(currentCursor.x + 10, ImGui::GetCursorPos().y));
    ImGui::SetNextItemWidth(currentRegion.x * 0.5);
    if (ImGui::InputText("##roomNameInput", roomName, sizeof(roomName) - 1, ImGuiInputTextFlags_EnterReturnsTrue)) {
        if (canCreateRoom) {
            roomManager.askToCreateRoom(roomName, selectedStage);
            selectedStage = 1;
            std::memset(roomName, 0, sizeof(roomName));
        }
        ImGui::SetKeyboardFocusHere(-1);
    }
    if (!canCreateRoom && isInputNotEmpty) {
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();
    }

    renderStageButton(currentRegion, currentCursor, selectedStage);

    ImVec2 buttonSize(currentRegion.x * 0.25, currentRegion.y * 0.8);
    ImVec2 buttonPos = ImVec2(
        ImGui::GetCursorPosX() + currentRegion.x - buttonSize.x - 10,
        currentCursor.y + ((currentRegion.y - buttonSize.y) / 2.0f)
    );
    ImGui::SetCursorPos(buttonPos);

    if (!canCreateRoom) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("Create", buttonSize) && canCreateRoom) {
        roomManager.askToCreateRoom(roomName, selectedStage);
        selectedStage = 1;
        memset(roomName, 0, sizeof(roomName));
    }
    if (!canCreateRoom) {
        ImGui::EndDisabled();
    }
}

void rtc::renderLobbyWindow(rtc::RoomManager &roomManager, const sf::Vector2u &windowSize)
{
    ImGui::SetNextWindowSize(windowSize);
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::Begin("Lobby", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

    ImGui::BeginChild("RoomList", ImVec2(0, windowSize.y * 0.7), true);
    renderRoomTable(roomManager);
    ImGui::EndChild();

    renderRoomCreation(roomManager);
    ImGui::End();
}
