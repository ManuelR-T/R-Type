/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** room_manager
*/

#pragma once

#include <map>
#include <utility>
#include "tcp/TCPClient.hpp"

namespace rtc {

class RoomManager {
    public:
    struct RoomContent {
        std::map<std::string, bool> player;
        bool joinable = true;
    };

    private:
    std::map<std::string, RoomContent> _rooms;
    std::string _currentRoom;
    client::TCPClient &_tcpClient;
    std::size_t _userId;
    std::string _userName;

    public:
    RoomManager(client::TCPClient &tcpClient, std::size_t userId, std::string userName)
        : _tcpClient(tcpClient), _userId(userId), _userName(std::move(userName))
    {
    }

    const std::string &getCurrentRoom() const;
    std::string &getCurrentRoom();

    const std::map<std::string, bool> &getCurrentRoomPlayer() const;

    const std::map<std::string, RoomContent> &getRooms() const;

    std::map<std::string, RoomContent> &getRooms();

    void askToCreateRoom(const std::string &roomName);
    void askToDeleteRoom(const std::string &roomName);
    void askToJoinRoom(const std::string &roomName);
    void askToLeaveRoom();
    void askToBeReady();
    void askToBeNotReady();
};

} // namespace rtc
